#include "TinyTimber.h"
#include "musicPlayer.h"
#include "brotherJohn.h"
#include "toneGenerator.h"
#include <stdlib.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////
//
// Global variables
musicPlayerObj musicPlay = initmusicPlayer();

/////////////////////////////////////////////////////////////////
//
// TEMPO CONTROLS

// Return the current tempo of the system
int getTempo(musicPlayerObj* self, int unused)
{    
	return self->tempo;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setTempo(musicPlayerObj* self, int tempo)
{
    if((tempo > MIN_TEMPO - 1) && (tempo < MAX_TEMPO+1))
	{
		self->tempo = tempo;
		SYNC(&toneGenerator, updateTempo, self->tempo);
		return tempo;
    }
    else
	{
		self->tempo = self->tempo; // Line may not be necessary
		return -1;
	}
}

////////////////////////////////////////////////////////////////////////
//
// KEY CONTROLS
//
//

// Return the current key of the system
int getKey(musicPlayerObj* self, int unused)
{    
	return self->key;
}

// Set the key of the system, if it is within key constraints.
// If within constraints, return the new key.
// If not, do not change the key, and return a -1.

int setKey(musicPlayerObj* self, int key)
{
    if((key > MIN_KEY-1) && (key < MAX_KEY+1))
	{
		self->key = key;
		SYNC(&toneGenerator, updateKey, self->key);
		return key;
    }
    else
	{
		self->key = self->key; // Line may not be necessary
		return MAX_KEY+1;
	}
}

void nextNote(musicPlayerObj* self, int unused)
{
	// Try to break the nextNote loop if the system is not playing
	
	if (self->i < SONG_LENGTH)
	{
		self->i += 1;
	}
	else
	{
		self->i = 0;
	}
		// Note controls
		int current_note = (self->key + f0_pos + song[self->i]);
		self->notePeriod = notes[current_note][1];

		// Tempo controls
	
		// Converting bpm to microseconds
		// NOTES:
		// - Beats are doubled, to ensure that fractions aren't present
		//   i.e. half beat = 1, single beat = 2, double beat = 4.
		// - Tempo is user set: for example, 120 bpm
		// - (Beat * tempo) / 2 converts to the correct beat, and accounts
		//   for beats being double.
		// - 60s / 120bpm = 0.5s period of a sound, however can't have fractions.
		//   To prevent this, USEC_MINUTE is the amount of microseconds in
		//   a minute, 60,000,000.
		// - 60,000,000us / 120bpm = 500,000us
	
		int temp_tempo = ((beats[self->i] * self->tempo) / 2); 
		int temp_tempo_us = (MSEC_MINUTE / temp_tempo);
		int temp_tempo_minus50 = temp_tempo_us - 50;

		self->deadline = MSEC(temp_tempo_minus50);
		self->user_mute=SYNC(&toneGenerator, getUserMute, 0);
	
		BEFORE(USEC(100), &toneGenerator, updateNotePeriod, self->notePeriod);
	
	// Mutes tone generator
	// If user has already muted the program, do not mute/unmute
		if(self->user_mute== 0)
		{
			SEND(self->deadline, USEC(50),&toneGenerator, mute, 0);
		}
		
		self->playingMsg = SEND(self->deadline, USEC(100) ,self, silence, 0);
	// This maths is very funky: so need to think of a better way to perform this
}


void silence(musicPlayerObj* self, int unused)
{
	// Will stop continous looping if user has stopped playing
	if(self->startStop == 0)
	{
		if(self->user_mute == 0)
		{
			SEND(MSEC(SILENCE_TIME), USEC(200) , &toneGenerator, unmute, NULL);
		}
		self->playingMsg = SEND(MSEC(SILENCE_TIME), USEC(100), self, nextNote, 0);
	}
}

int startStopPlayer(musicPlayerObj* self, int state)
{
	ASYNC(&toneGenerator, mute, 0);
	self->startStop = state;

	// If system is started, continue the nextNote looping
	if(self->startStop == 0 && self->notePeriod == 0)
	{
		self->playingMsg = ASYNC(self, silence, 0);
	}
	
	// Else break the loop, and mute the toneGenerator
	else
	{
		// Prevent overlapping of ASYNC statements when 
		// rapidly pressing Start/Stop
		ABORT(self->playingMsg);
		
		// Reinitialise values
		self->i = -1;
		self->notePeriod = 0;
		self->deadline = 0;
		
	}
	// Return status of startStop
	return self->startStop;
}

int getPlayerStatus(musicPlayerObj* self, int unusued)
{
	return self->startStop;
}

long getDeadline(musicPlayerObj* self, int unused)
{    
	return USEC_OF(self->deadline);
}

int getI(musicPlayerObj* self, int unusued)
{
	return self->i;
}
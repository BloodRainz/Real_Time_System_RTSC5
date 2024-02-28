/*
 * Part 2 Section 2: Music generator
 * 
 * RTS-C5:	Emil Johansson (emiuo@chalmers.se) 
 * 			Kavineshver Sivaraman Kathiresan (kavkat@chalmers.se) 
 * 			Joshua Geraghty (joshuag@chalmers.se)
 * 
 * Verified by Lab TA
 * 
 * Current bugs:
 * 
 * 1. toneGenerator.c skips the first iteration of Brother John upon
 *    the first loop of the program. 
 *    - Seems like its bypassing the if statements upon initialisation
 *    - Possibly change the if-else statements within toneGenerator.c
 *    - Initialiasng i to -1 works as a 'bandage' solution: but doesn't really fix problem
 * 2. Deadline arithmetic doesn't seem to calculate correctly:
 *    - Double check the maths, and possible change the order of operations
 *    - Turn it to microseconds to get a higher resolution?
 *    - SOLVED: Looking at it again, I think its correct?
 * 3. Find a better way to understand the timing requirements of the system.
 *    - Derive WCET of player at extreme conditions
 */
 
#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

#include "toneGenerator.h"

#include "musicPlayer.h"



////////////////////////////////////////////////////////////////////////
//
// KEY CONTROLS
//
//

// Return the current key of the system
int getKey(MusicPlayerObj* self, int unused)
{    
	return self->key;
}

// Set the key of the system, if it is within key constraints.
// If within constraints, return the new key.
// If not, do not change the key, and return a -1.

int setKey(MusicPlayerObj* self, int key)
{
    if((key > MIN_KEY-1) && (key < MAX_KEY+1))
	{
		self->key = key;
		return key;
    }
    else
	{
		self->key = self->key; // Line may not be necessary
		return MAX_KEY+1;
	}
}

/////////////////////////////////////////////////////////////////
//
// TEMPO CONTROLS

// Return the current tempo of the system
int getTempo(MusicPlayerObj* self, int unused)
{    
	return self->tempo;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setTempo(MusicPlayerObj* self, int tempo)
{
    if((tempo > MIN_TEMPO - 1) && (tempo < MAX_TEMPO+1))
	{
		self->tempo = tempo;
		return tempo;
    }
    else
	{
		self->tempo = self->tempo; // Line may not be necessary
		return -1;
	}
}



//void nextNote(MusicPlayerObj* self, int i)
//{
	// Note controls
//	int current_note = (self->key + f0_pos + song[self->i]);
//	self->notePeriod = notes[current_note][1];
	
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
	
//	int temp_tempo = ((beats[self->i] * self->tempo) / 2); 
//	int temp_tempo_us = (MSEC_MINUTE / temp_tempo);
//	int temp_tempo_minus50 = temp_tempo_us - 50;
	
//	self->deadline = MSEC(temp_tempo_minus50);
	
	// This maths is very funky: so need to think of a better way to perform this
//}


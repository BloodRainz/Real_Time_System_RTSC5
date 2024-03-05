#ifndef _NOTES_
#define _NOTES_

#define SONG_LENGTH 32

#define f0_pos 10


#define a 2		// Full beat
#define b a/2	// Double beat
#define c a*2	// half beat

const int song[SONG_LENGTH] = {0, 2, 4, 0, 0, 2, 4, 0, 4, 5, 7, 4, 5, 7, 7, 9, 7, 5, 4, 0, 7, 9, 7, 5, 4, 0, 0, -5, 0, 0, -5, 0};

// All beats got doubled: prevents us from having to use 0.5 values, which cause issues when declared as an int
const int beats[SONG_LENGTH] = {a, a, a, a, a, a, a, a, a, a, b, a, a, b, c, c, c, c, a, a, c, c, c, c, a, a, a, a, b, a, a, b};

const int notes[25][2] = {
 // frequency left and period right(in us)  
	{ 247, 2025 }, // f-10 B new 0
	{ 262, 1911 }, // f-9 C
	{ 277, 1804 }, // f-8 C#
	{ 294, 1703 }, // f-7 D
	{ 311, 1607 }, // f-6 D#
	{ 330, 1517 }, // f-5 E
	{ 349, 1431 }, // f-4 F
	{ 370, 1351 }, // f-3 F#
	{ 392, 1275 }, // f-2 G
	{ 415, 1204 }, // f-1 G#
	{ 440, 1136 }, // f0 A new 10
	{ 466, 1073 }, // f1 A#
	{ 494, 1012 }, // f2 B
	{ 523, 956 }, // f3 C
	{ 554, 902 }, // f4 C#
	{ 587, 851 }, // f5 D
	{ 622, 803 }, // f6 D#
	{ 659, 758 }, // f7 E
	{ 698, 716 }, // f8 F
	{ 740, 676 }, // f9 F#
	{ 784, 638 }, // f10 G
	{ 831, 602 }, // f11 G#
	{ 880, 568 }, // f12 A
	{ 932,  536}, // f 13 A#
	{ 988,  506}, // f 14 B new 24

};

// Sets a reference to 440Hz A position within notes array


#endif
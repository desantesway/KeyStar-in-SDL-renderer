#ifndef PIANO_H
#define PIANO_H

#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

class PianoKeyboard
{
public:

	PianoKeyboard();
	~PianoKeyboard();
	void SetOctave(int octave);
	int GetOctave();
	void SetKey(int key);
	int GetKey();
	void SetPedal(float pedal);
	float GetPedal();

private:

	int octave;
	int key;
	float pedal;

	RtMidiIn* midiin;
	RtMidiOut* midiout;

	// var for events
};

#endif
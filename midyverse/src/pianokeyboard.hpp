#ifndef PIANO_H
#define PIANO_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "RtMidi.h"
#include "libs.hpp"

class PianoKeyboard
{
public:

	PianoKeyboard(int midiinPort, int midioutPort);
	PianoKeyboard();
	~PianoKeyboard();
	void SetOctave(int octave);
	int GetOctave();
	void SetKeyNum(int keyNum);
	int GetKeyNum();
	void SetPedal(bool pedal);
	bool GetPedal();

	void SetMidiinPort(int in);
	int GetMidiinPort();
	void SetMidioutPort(int out);
	int GetMidioutPort();

	void SetPianoTexture(SDL_Texture* texture, std::string location);
	SDL_Texture* GetPianoTexture();
	int GetPianoTextureHeight();
	void SetPianoTextureHeight(int h);
	int GetPianoTextureWidth();
	void SetPianoTextureWidth(int w);

	void DisplayInPorts();
	void DisplayOutPorts();

	bool StartMidi();

	bool StartMidiIn();
	bool StartMidiOut();
	void StopMidiIn();
	void StopMidiOut();	

	void DetectKeys();

private:

	int octave;
	int keyNum;
	bool pedal;

	int midiinPort;
	int midioutPort;

	RtMidiIn* midiin;
	RtMidiOut* midiout;

	SDL_Texture* pianoTexture;
	int pianoTextureHeight;
	int pianoTextureWidth;
};

#endif
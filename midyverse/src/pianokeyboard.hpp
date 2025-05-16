#ifndef PIANO_H
#define PIANO_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "RtMidi.h"
#include "libs.hpp"

struct KeyTexture
{
	SDL_Texture* tex;
	std::string location;
	int h;
	int w;
};

struct Note
{	
	bool played;
	int velocity;
};

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

	KeyTexture LoadKeyTex(KeyTexture key, SDL_Texture*& texture, std::string location);

	KeyTexture GetRWhiteKey();
	KeyTexture GetLWhiteKey();
	KeyTexture GetMidWhiteKey();
	KeyTexture GetRoundWhiteKey();
	KeyTexture GetWhiteKeyShadow();
	KeyTexture GetBlackKey();
	KeyTexture GetBlackKeyShadow();
	void SetRWhiteKey(KeyTexture tex);
	void SetLWhiteKey(KeyTexture tex);
	void SetMidWhiteKey(KeyTexture tex);
	void SetRoundWhiteKey(KeyTexture tex);
	void SetWhiteKeyShadow(KeyTexture tex);
	void SetBlackKey(KeyTexture tex);
	void SetBlackKeyShadow(KeyTexture tex);

	std::map<int, Note> GetNotesPlayed();
	void RemoveNote(int key_pos);

	void DestroyTextures();

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

	KeyTexture rWhiteKeyTex;
	KeyTexture lWhiteKeyTex;
	KeyTexture midWhiteKeyTex;
	KeyTexture roundWhiteKeyTex;
	KeyTexture whiteKeyShadowTex;
	KeyTexture blackKeyTex;
	KeyTexture blackKeyShadowTex;

	std::map<int, Note> notesPlayed;
};

#endif
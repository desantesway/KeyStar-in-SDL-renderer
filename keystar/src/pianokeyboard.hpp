#ifndef PIANO_H
#define PIANO_H

#include "libs.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "RtMidi.h"
#include "ChordRecognizer/chordnamer.cpp"

struct KeyTexture
{
	SDL_Texture* tex;
	std::string location;
	int h;
	int w;
};

struct KeyTextures {
	KeyTexture rWhiteKeyTex;
	KeyTexture lWhiteKeyTex;
	KeyTexture midWhiteKeyTex;
	KeyTexture roundWhiteKeyTex;
	KeyTexture blackKeyTex;
	KeyTexture blackKeyShadowTex;
	KeyTexture blackBlendKeyTex;
};

struct Note
{	
	bool played;
	bool pedal;
	bool pressed;
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
	bool AreNotesUpdated();

	void SetMidiinPort(int in);
	int GetMidiinPort();
	void SetMidioutPort(int out);
	int GetMidioutPort();

	KeyTexture LoadKeyTex(KeyTexture key, SDL_Texture*& texture, std::string location);

	KeyTexture GetRWhiteKey();
	KeyTexture GetLWhiteKey();
	KeyTexture GetMidWhiteKey();
	KeyTexture GetRoundWhiteKey();
	KeyTexture GetBlackKey();
	KeyTexture GetBlackKeyShadow();
	KeyTexture GetBlackBlendKey();
	void SetRWhiteKey(KeyTexture tex);
	void SetLWhiteKey(KeyTexture tex);
	void SetMidWhiteKey(KeyTexture tex);
	void SetRoundWhiteKey(KeyTexture tex);
	void SetBlackKey(KeyTexture tex);
	void SetBlackKeyShadow(KeyTexture tex);
	void SetBlackBlendKey(KeyTexture tex);

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

	void SetPedalNotes();
	void RemovePedalNotes();
	void DetectKeys();

	std::string GetChordPlayed();

private:

	int octave;
	int keyNum;
	bool pedal;

	int midiinPort;
	int midioutPort;

	RtMidiIn* midiin;
	RtMidiOut* midiout;

	KeyTextures keyTextures;

	std::map<int, Note> notesPlayed;
	bool notesUpdated;
};

#endif
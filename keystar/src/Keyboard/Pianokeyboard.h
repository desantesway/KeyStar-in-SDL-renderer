#ifndef PIANO_H
#define PIANO_H

#include "../Utils/Libs.h"  
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "RtMidi.h"
#include "../ChordRecognizer/chordnamer.cpp"
#include "../Engine/SDL_Textures.h"
#include "../Engine/Animations.h"
#include "../Engine/Shaders/Shaders.h"

struct KeyTexture
{
	SDL_Texture* tex;
	std::string location;
	int h;
	int w;
};

struct KeyTextures {
	std::tuple <KeyTexture, KeyTexture> rWhiteKeyTex;
	std::tuple <KeyTexture, KeyTexture> lWhiteKeyTex;
	std::tuple <KeyTexture, KeyTexture> midWhiteKeyTex;
	std::tuple <KeyTexture, KeyTexture> roundWhiteKeyTex;
	std::tuple <KeyTexture, KeyTexture> blackKeyTex;
	KeyTexture blackKeyShadowTex;
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

	bool StartMidi();

	void DetectKeys();

	bool LoadPianoTextures(SDL_Renderer* renderer, int height);
	bool RenderPiano(SDL_Renderer* renderer, Animations* animations, bool isPiano, int width, int height);
	bool ChordsText(SDL_Renderer* renderer, Scene* scene, TTF_Font* font);

private:

	KeyTexture GetRWhiteKey(bool pos);
	KeyTexture GetLWhiteKey(bool pos);
	KeyTexture GetMidWhiteKey(bool pos);
	KeyTexture GetRoundWhiteKey(bool pos);
	KeyTexture GetBlackKey(bool pos);
	KeyTexture GetBlackKeyShadow();
	void SetRWhiteKey(std::tuple<KeyTexture, KeyTexture> tex);
	void SetLWhiteKey(std::tuple<KeyTexture, KeyTexture> tex);
	void SetMidWhiteKey(std::tuple<KeyTexture, KeyTexture> tex);
	void SetRoundWhiteKey(std::tuple<KeyTexture, KeyTexture> tex);
	void SetBlackKey(std::tuple<KeyTexture, KeyTexture> tex);
	void SetBlackKeyShadow(KeyTexture tex);

	std::map<int, Note> GetNotesPlayed();
	void RemoveNote(int key_pos);

	void DestroyTextures();

	void DisplayInPorts();
	void DisplayOutPorts();

	bool StartMidiIn();
	bool StartMidiOut();
	void StopMidiIn();
	void StopMidiOut();

	void SetPedalNotes();
	void RemovePedalNotes();

	std::string GetChordPlayed();

	KeyTexture LoadKeyTex(SDL_Texture*& texture, std::string location);
	std::tuple<KeyTexture, KeyTexture> LoadKeyTex(SDL_Renderer* renderer, SDL_Texture*& texture, std::string location);

	// variables

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
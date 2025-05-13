
#include "pianokeyboard.hpp"

// oh yeah
PianoKeyboard::PianoKeyboard(){
	this->octave = 0;
	this->key = 0;
	this->pedal = 0.0f;
}
PianoKeyboard::~PianoKeyboard() {}

void PianoKeyboard::SetOctave(int octave) { this->octave = octave; }
int PianoKeyboard::GetOctave() { return this->octave; }
void PianoKeyboard::SetKey(int key) { this->key = key; }
int PianoKeyboard::GetKey() { return this->key; }
void PianoKeyboard::SetPedal(float pedal) { this->pedal = pedal; }
float PianoKeyboard::GetPedal() { return this->pedal; }
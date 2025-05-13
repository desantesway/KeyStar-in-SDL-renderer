#include "piano.hpp"

Piano::Piano(){
	this->octave = 0;
	this->key = 0;
	this->pedal = 0.0f;
}
Piano::~Piano() {}

void Piano::SetOctave(int octave) { this->octave = octave; }
int Piano::GetOctave() { return this->octave; }
void Piano::SetKey(int key) { this->key = key; }
int Piano::GetKey() { return this->key; }
void Piano::SetPedal(float pedal) { this->pedal = pedal; }
float Piano::GetPedal() { return this->pedal; }
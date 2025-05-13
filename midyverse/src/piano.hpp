#ifndef PIANO_H
#define PIANO_H

class Piano
{
public:

	Piano();
	~Piano();
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

	// var for events
};

#endif
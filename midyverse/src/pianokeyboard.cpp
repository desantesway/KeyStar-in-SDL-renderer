
#include "pianokeyboard.hpp"

PianoKeyboard::PianoKeyboard(int midiinPort, int midioutPort) {
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;
	this->midiinPort = midiinPort;
	this->midioutPort = midioutPort;

	this->pianoTexture = NULL;
	this->pianoTextureWidth = 0;
	this->pianoTextureHeight = 0;
}

PianoKeyboard::PianoKeyboard(){
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;

	this->midiinPort = -1;
	this->midioutPort = -1;

	this->pianoTexture = NULL;
	this->pianoTextureWidth = 0;
	this->pianoTextureHeight = 0;
}

PianoKeyboard::~PianoKeyboard() {
	StopMidiIn();
	StopMidiOut();

	SDL_DestroyTexture(this->pianoTexture);
	this->pianoTexture = NULL;
}

void PianoKeyboard::SetOctave(int octave) { this->octave = octave; }
int PianoKeyboard::GetOctave() { return this->octave; }
void PianoKeyboard::SetKeyNum(int key) { this->keyNum = key; }
int PianoKeyboard::GetKeyNum() { return this->keyNum; }
void PianoKeyboard::SetPedal(bool pedal) { this->pedal = pedal; }
bool PianoKeyboard::GetPedal() { return this->pedal; }

void PianoKeyboard::SetMidiinPort(int in) { this->midiinPort = in; }
int PianoKeyboard::GetMidiinPort() { return this->midiinPort; }
void PianoKeyboard::SetMidioutPort(int out) { this->midioutPort = out; }
int  PianoKeyboard::GetMidioutPort() { return this->midioutPort; }

void PianoKeyboard::SetPianoTexture(SDL_Texture* texture, std::string location) { 
	unsigned int width;
	unsigned int height;
	unsigned char buf[8];

	std::ifstream in(location);
	in.seekg(16);
	in.read(reinterpret_cast<char*>(&buf), 8);

	width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
	height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
	SetPianoTextureWidth(width);
	SetPianoTextureHeight(height);
	this->pianoTexture = texture; 

}
SDL_Texture* PianoKeyboard::GetPianoTexture() { return this->pianoTexture; }

int PianoKeyboard::GetPianoTextureHeight() { return this->pianoTextureHeight; }
void PianoKeyboard::SetPianoTextureHeight(int h) { this->pianoTextureHeight = h; }
int PianoKeyboard::GetPianoTextureWidth() { return this->pianoTextureWidth; }
void PianoKeyboard::SetPianoTextureWidth(int w) { this->pianoTextureWidth = w; }

bool PianoKeyboard::StartMidi() {

	CHECK_RESULT_RET(StartMidiIn(),"Error starting midi in");
	CHECK_RESULT_RET(StartMidiOut(), "Error starting midi out");

	return true;
}

// !!!!!!!!!!! setup saved midi in and out
bool PianoKeyboard::StartMidiIn() {
	if (this->midiin == NULL) {
		try {
			this->midiin = new RtMidiIn();
		}
		catch (RtMidiError& error) {
			error.printMessage();
			return false;
		}
	}
	return true;
}

bool PianoKeyboard::StartMidiOut() {
	if(this->midiout == NULL) {
		try {
			this->midiout = new RtMidiOut();
		}
		catch (RtMidiError& error) {
			error.printMessage();
			return false;
		}
	}
	return true;
}

void PianoKeyboard::StopMidiIn() {
	delete this->midiin;
	this->midiin = NULL;
	this->midiinPort = -1;
}

void PianoKeyboard::StopMidiOut() {
	delete this->midiout;
	this->midiout = NULL;
	this->midioutPort = -1;
}

void PianoKeyboard::DisplayInPorts() {
	unsigned int nPorts = midiin->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
	std::string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = midiin->getPortName(i);
		}
		catch (RtMidiError& error) {
			error.printMessage();
			StopMidiIn();
		}
		std::cout << "  Input Port #" << i + 1 << ": " << portName << '\n';
	}
}

void PianoKeyboard::DisplayOutPorts() {
	unsigned int nPorts = midiout->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
	std::string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = midiout->getPortName(i);
		}
		catch (RtMidiError& error) {
			error.printMessage();
			StopMidiOut();
		}
		std::cout << "  Output Port #" << i + 1 << ": " << portName << '\n';
	}
	std::cout << '\n';
}

void PianoKeyboard::DetectKeys() {
	if (GetMidiinPort() == -1){
		DisplayInPorts();
		SetMidiinPort(0);
		midiin->openPort(GetMidiinPort());
	}
	else if (GetMidioutPort() == -1) {
		DisplayOutPorts();
		SetMidioutPort(4);
	}
	else {
		std::vector<unsigned char> message;
		int nBytes, i;
		double stamp;

		midiin->ignoreTypes(false, false, false);

		stamp = midiin->getMessage(&message);
		nBytes = message.size();
		for (i = 0; i < nBytes; i++)
			std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
		if (nBytes > 0)
			std::cout << "stamp = " << stamp << std::endl;
	}
}
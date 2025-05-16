
#include "pianokeyboard.hpp"

PianoKeyboard::PianoKeyboard(int midiinPort, int midioutPort) {
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;
	this->midiinPort = midiinPort;
	this->midioutPort = midioutPort;
}

PianoKeyboard::PianoKeyboard(){
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;

	this->midiinPort = -1;
	this->midioutPort = -1;
}


PianoKeyboard::~PianoKeyboard() {
	StopMidiIn();
	StopMidiOut();

	DestroyTextures();
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

void PianoKeyboard::DestroyTextures() {
	SDL_DestroyTexture(this->rWhiteKeyTex.tex);
	this->rWhiteKeyTex.tex = NULL;
	SDL_DestroyTexture(this->lWhiteKeyTex.tex);
	this->lWhiteKeyTex.tex = NULL;
	SDL_DestroyTexture(this->midWhiteKeyTex.tex);
	this->midWhiteKeyTex.tex = NULL;
	SDL_DestroyTexture(this->roundWhiteKeyTex.tex);
	this->roundWhiteKeyTex.tex = NULL;
	SDL_DestroyTexture(this->whiteKeyShadowTex.tex);
	this->whiteKeyShadowTex.tex = NULL;
	SDL_DestroyTexture(this->blackKeyTex.tex);
	this->blackKeyTex.tex = NULL;
	SDL_DestroyTexture(this->blackKeyShadowTex.tex);
	this->blackKeyShadowTex.tex = NULL;
}

KeyTexture PianoKeyboard::LoadKeyTex(KeyTexture key, SDL_Texture*& texture, std::string location) {
	
	key.location = location;

	unsigned char buf[8];

	std::ifstream in(key.location);
	in.seekg(16);
	in.read(reinterpret_cast<char*>(&buf), 8);

	key.w = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
	key.h = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
	key.tex = texture; 

	return key;
}

KeyTexture PianoKeyboard::GetRWhiteKey() { return this->rWhiteKeyTex; }
KeyTexture PianoKeyboard::GetLWhiteKey() { return this->lWhiteKeyTex; }
KeyTexture PianoKeyboard::GetMidWhiteKey() { return this->midWhiteKeyTex; }
KeyTexture PianoKeyboard::GetRoundWhiteKey() { return this->roundWhiteKeyTex; }
KeyTexture PianoKeyboard::GetWhiteKeyShadow() { return this->whiteKeyShadowTex; }
KeyTexture PianoKeyboard::GetBlackKey() { return this->blackKeyTex; }
KeyTexture PianoKeyboard::GetBlackKeyShadow() { return this->blackKeyShadowTex; }

void PianoKeyboard::SetRWhiteKey(KeyTexture tex) { this->rWhiteKeyTex = tex; }
void PianoKeyboard::SetLWhiteKey(KeyTexture tex) { this->lWhiteKeyTex = tex; }
void PianoKeyboard::SetMidWhiteKey(KeyTexture tex) { this->midWhiteKeyTex = tex; }
void PianoKeyboard::SetRoundWhiteKey(KeyTexture tex) { this->roundWhiteKeyTex = tex;}
void PianoKeyboard::SetWhiteKeyShadow(KeyTexture tex) { this->whiteKeyShadowTex = tex;}
void PianoKeyboard::SetBlackKey(KeyTexture tex) { this->blackKeyTex = tex;}
void PianoKeyboard::SetBlackKeyShadow(KeyTexture tex) { this->blackKeyShadowTex = tex;}

std::map<int, Note> PianoKeyboard::GetNotesPlayed() { return this->notesPlayed; }
void PianoKeyboard::RemoveNote(int key_pos) {
	auto it = this->notesPlayed.find(key_pos);
	if (it != this->notesPlayed.end()) {
		this->notesPlayed.erase(it);
	}
}

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
		
		if (message.size() > 0) {
			if ((int)message[0] == 144) {
				this->notesPlayed.insert({ (int)message[1], {false, (int)message[2]} });
			}
			else if ((int)message[0] == 128) {
				RemoveNote((int)message[1]);
			}
		}
	}
}
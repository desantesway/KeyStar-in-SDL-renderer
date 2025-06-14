
#include "pianokeyboard.h"

PianoKeyboard::PianoKeyboard(int midiinPort, int midioutPort) {
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;
	this->midiinPort = midiinPort;
	this->midioutPort = midioutPort;
	this->notesUpdated = false;
}

PianoKeyboard::PianoKeyboard(){
	this->midiin = NULL;
	this->midiout = NULL;

	this->octave = 0;
	this->keyNum = 49;
	this->pedal = false;

	this->midiinPort = -1;
	this->midioutPort = -1;
	this->notesUpdated = false;
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
bool PianoKeyboard::AreNotesUpdated() { return this->notesUpdated; }

void PianoKeyboard::SetMidiinPort(int in) { this->midiinPort = in; }
int PianoKeyboard::GetMidiinPort() { return this->midiinPort; }
void PianoKeyboard::SetMidioutPort(int out) { this->midioutPort = out; }
int  PianoKeyboard::GetMidioutPort() { return this->midioutPort; }

void deleteTuples(std::tuple<KeyTexture, KeyTexture>& tuple) {
    SDL_DestroyTexture(std::get<0>(tuple).tex);
    SDL_DestroyTexture(std::get<1>(tuple).tex);
    tuple = std::make_tuple(KeyTexture{ NULL, "", 0, 0 }, KeyTexture{ NULL, "", 0, 0 }); // Reset the tuple to empty KeyTextures
}

void PianoKeyboard::DestroyTextures() {
	deleteTuples(this->keyTextures.rWhiteKeyTex);
	deleteTuples(this->keyTextures.lWhiteKeyTex);
	deleteTuples(this->keyTextures.midWhiteKeyTex);
	deleteTuples(this->keyTextures.roundWhiteKeyTex);
	deleteTuples(this->keyTextures.blackKeyTex);

	SDL_DestroyTexture(this->keyTextures.blackKeyShadowTex.tex);
	this->keyTextures.blackKeyShadowTex.tex = NULL;
}

SDL_Texture* CreateGradientTexture(SDL_Renderer* renderer, int width, int height, SDL_Color top, SDL_Color bottom) {
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	// Set as render target
	SDL_SetRenderTarget(renderer, texture);

	// Clear to transparent
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Draw gradient
	for (int y = 0; y < height; y++) {
		float ratio = (float)y / height;
		Uint8 r = top.r + (bottom.r - top.r) * ratio;
		Uint8 g = top.g + (bottom.g - top.g) * ratio;
		Uint8 b = top.b + (bottom.b - top.b) * ratio;
		Uint8 a = top.a + (bottom.a - top.a) * ratio;

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderLine(renderer, 0, y, width - 1, y);
	}

	// Reset render target
	SDL_SetRenderTarget(renderer, NULL);

	return texture;
}

SDL_Texture* MaskTexture(SDL_Renderer* renderer, SDL_Texture*& mask, SDL_Texture*& diffuse)
{
	SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
	SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND); // blend - mod - none - none
	SDL_SetRenderTarget(renderer, result);
	SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_MOD);
	SDL_SetTextureBlendMode(diffuse, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, diffuse, NULL, NULL);
	SDL_RenderTexture(renderer, mask, NULL, NULL);
	SDL_SetRenderTarget(renderer, NULL); SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderClear(renderer);
	return result;
}

std::tuple<KeyTexture, KeyTexture> PianoKeyboard::LoadKeyTex(SDL_Renderer* renderer, SDL_Texture*& texture, std::string location) {
    unsigned char buf[8];

    std::ifstream in(location);
    in.seekg(16);
    in.read(reinterpret_cast<char*>(&buf), 8);

    int h = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
    int w = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);

    static SDL_Color topColor = TOP_PIANO_GRADIENT_COLOR;
    static SDL_Color bottomColor = BOT_PIANO_GRADIENT_COLOR;

    SDL_Texture* gradient = CreateGradientTexture(renderer, w, h, topColor, bottomColor);

	SDL_Texture* pianoTexture = IMG_LoadTexture(renderer, StrToPtr(location));

    SDL_Texture* blendedTexture = MaskTexture(renderer, gradient, texture);

    SDL_DestroyTexture(gradient);
	SDL_DestroyTexture(texture);

	gradient = NULL;
	texture = NULL;

    return std::make_tuple(KeyTexture{ pianoTexture, location, h, w }, KeyTexture{ blendedTexture , location, h, w });
}

KeyTexture PianoKeyboard::LoadKeyTex(SDL_Texture*& texture, std::string location) {
	
	KeyTexture key;

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

KeyTexture GetKey(std::tuple<KeyTexture, KeyTexture> tuple, bool pos) {
	if (pos) { 
		return std::get<0>(tuple); 
	}
	else { 
		return std::get<1>(tuple); 
	}
}

KeyTexture PianoKeyboard::GetRWhiteKey(bool pos) { return GetKey(this->keyTextures.rWhiteKeyTex, pos); }
KeyTexture PianoKeyboard::GetLWhiteKey(bool pos) { return GetKey(this->keyTextures.lWhiteKeyTex, pos); }
KeyTexture PianoKeyboard::GetMidWhiteKey(bool pos) { return GetKey(this->keyTextures.midWhiteKeyTex, pos); }
KeyTexture PianoKeyboard::GetRoundWhiteKey(bool pos) { return GetKey(this->keyTextures.roundWhiteKeyTex,pos);  }
KeyTexture PianoKeyboard::GetBlackKey(bool pos) { return GetKey(this->keyTextures.blackKeyTex,pos);  }
KeyTexture PianoKeyboard::GetBlackKeyShadow() { return this->keyTextures.blackKeyShadowTex; }

void PianoKeyboard::SetRWhiteKey(std::tuple<KeyTexture, KeyTexture> tex) { this->keyTextures.rWhiteKeyTex = tex; }
void PianoKeyboard::SetLWhiteKey(std::tuple<KeyTexture, KeyTexture> tex) { this->keyTextures.lWhiteKeyTex = tex; }
void PianoKeyboard::SetMidWhiteKey(std::tuple<KeyTexture, KeyTexture> tex) { this->keyTextures.midWhiteKeyTex = tex; }
void PianoKeyboard::SetRoundWhiteKey(std::tuple<KeyTexture, KeyTexture> tex) { this->keyTextures.roundWhiteKeyTex = tex;}
void PianoKeyboard::SetBlackKey(std::tuple<KeyTexture, KeyTexture> tex) { this->keyTextures.blackKeyTex = tex;}
void PianoKeyboard::SetBlackKeyShadow(KeyTexture tex) { this->keyTextures.blackKeyShadowTex = tex;}

std::map<int, Note> PianoKeyboard::GetNotesPlayed() { return this->notesPlayed; }

bool PianoKeyboard::StartMidi() {

	CHECK_RESULT_RET(StartMidiIn(),"Error starting midi in");
	CHECK_RESULT_RET(StartMidiOut(), "Error starting midi out");

	return true;
}

// !!!!!!!!!!! setup saved midi in and out
bool PianoKeyboard::StartMidiIn() {
	if (this->midiin == NULL) {
		try {
			this->midiin = new RtMidiIn(RtMidi::UNSPECIFIED, CAT(NAME, "MidiIn"));
			midiin->ignoreTypes(false, false, false);

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
	if (!midiin) {  // Critical check
		std::cerr << "MIDI input not initialized!" << std::endl;
		return;
	}
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
	
	//midiout->openVirtualPort(MIDI_NAME); //!!!!!not working on windows

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

void PianoKeyboard::RemoveNote(int key_pos) {
	auto it = this->notesPlayed.find(key_pos);
	if (it != this->notesPlayed.end()) {
		this->notesPlayed.erase(it);
	}
}

void PianoKeyboard::SetPedalNotes() {
	std::map<int, Note> notes;
	for (auto i : GetNotesPlayed()) {
		notes.insert({ i.first, {i.second.played, this->pedal, i.second.pressed , i.second.velocity } });
	}
	this->notesPlayed = notes;
}

void PianoKeyboard::RemovePedalNotes() {
	for (auto i : GetNotesPlayed()) {
		if (!i.second.pedal && !i.second.pressed) {
			RemoveNote(i.first);
		}
	}
}

// fix locked keys
void PianoKeyboard::DetectKeys() {
	if (GetMidiinPort() == -1){
		DisplayInPorts();
		SetMidiinPort(0); //!!!! to remove when theres a menu
		midiin->openPort(GetMidiinPort()); // handle this error
		

	}
	if (GetMidioutPort() == -1) {
		DisplayOutPorts();
		SetMidioutPort(2);//!!!! to remove when theres a menu
		midiout->openPort(GetMidioutPort());
	}
	static std::vector<unsigned char> message;
	int nBytes;

	bool done = false;
	double stamp;
	
	while (!done) {
		stamp = midiin->getMessage(&message);
		nBytes = message.size();

		if (nBytes > 0) {
			if ((int)message[0] == 144 ) {
				if (this->notesPlayed.count((int)message[1])) {
					this->notesPlayed[(int)message[1]] = { false, false, true, (int)message[2] };
				}
				else {
					this->notesPlayed.insert({ (int)message[1], {false, false, true, (int)message[2]} });
				}
			}
			else if ((int)message[0] == 128) {
				if (!this->pedal) {
					RemoveNote((int)message[1]);
					
				}
				else {
					this->notesPlayed[(int)message[1]].pressed = false;
				}
			}
			else if ((int)message[0] == 176) {
				if ((int)message[1] == 64) {
					this->pedal = (bool)message[2];
					SetPedalNotes();
					RemovePedalNotes();
				}
			}
			this->notesUpdated = true; // for chord detection text
			midiout->sendMessage(&message);
		}
		else {
			done = true;
		}
	}
	
	
}

std::string PianoKeyboard::GetChordPlayed() {
	std::string notes[12] = {"C","Db","D","Eb","E","F","Gb","G","Ab","A","Bb","B" };
	int root = 120;
	std::vector<size_t> chord;
	for (auto i : GetNotesPlayed()) {
		chord.push_back(i.first);
		root = std::min(root, i.first);
	}
	this->notesUpdated = false;
	int chord_size = (int)chord.size();
	if (chord_size > 1) {
		std::string name; // do to detect all chords
		std::string original_root;
		auto chords = name_that_chord(chord);
		bool once = true;
		for (const auto& chord : chords) {
			if (notes[chord.root] == notes[(root % 12)] && ((chord_size >= 6 && chord.num_accidentals <=1) || (chord.num_accidentals <= 0))) {
				name = chord.base_name;
				original_root = notes[chord.root];
				break;
			}
			if (once && chord.omitted_tones.size() <= 1) {
				name = chord.base_name;
				original_root = notes[chord.root];
				once = false;
			}

		}
		if (original_root == notes[(root % 12)]) {
			return CAT(CAT(original_root, " "), name);
		}
		
		return CAT(CAT(original_root, " "), CAT(name, CAT("/", notes[(root % 12)])));
	}else if (chord_size == 1) {
		return CAT(notes[(root % 12)], std::to_string((int)std::floor(root/12) -1));
	} 
	else {
		return " ";
	}
	
}
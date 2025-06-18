
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

// destroys the textures in the tuple and resets them to empty KeyTextures
void deleteTuples(std::tuple<KeyTexture, KeyTexture>& tuple) {
    SDL_DestroyTexture(std::get<0>(tuple).tex);
    SDL_DestroyTexture(std::get<1>(tuple).tex);
    tuple = std::make_tuple(KeyTexture{ NULL, "", 0, 0 }, KeyTexture{ NULL, "", 0, 0 }); // Reset the tuple to empty KeyTextures
}

// Destroys all textures in the keyTextures struct and resets them to empty KeyTextures
void PianoKeyboard::DestroyTextures() {
	deleteTuples(this->keyTextures.rWhiteKeyTex);
	deleteTuples(this->keyTextures.lWhiteKeyTex);
	deleteTuples(this->keyTextures.midWhiteKeyTex);
	deleteTuples(this->keyTextures.roundWhiteKeyTex);
	deleteTuples(this->keyTextures.blackKeyTex);

	SDL_DestroyTexture(this->keyTextures.blackKeyShadowTex.tex);
	this->keyTextures.blackKeyShadowTex.tex = NULL;
}

// Loads a texture from a file and returns it as a KeyTexture tuple with the gradient applied (for when pressed)
std::tuple<KeyTexture, KeyTexture> PianoKeyboard::LoadKeyTex(SDL_Renderer* renderer, SDL_Texture*& texture, std::string location, bool key_type) {
    unsigned char buf[8];

    std::ifstream in(location);
    in.seekg(16);
    in.read(reinterpret_cast<char*>(&buf), 8);

    int h = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
    int w = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);

	SDL_Texture* pianoTexture = IMG_LoadTexture(renderer, StrToPtr(location));

	SDL_Color topColor, bottomColor;

	if (key_type) { // if white else black
		topColor = WTOP_PIANO_GRADIENT_COLOR;
		bottomColor = WBOT_PIANO_GRADIENT_COLOR;
	}
	else {
		topColor = BTOP_PIANO_GRADIENT_COLOR;
		bottomColor = BBOT_PIANO_GRADIENT_COLOR;
	}

    SDL_Texture* gradient = GenerateGradientTexture(renderer, w, h, topColor, bottomColor);

    SDL_Texture* blendedTexture = MaskTexture(renderer, gradient, texture);

    SDL_DestroyTexture(gradient);
	SDL_DestroyTexture(texture);

	gradient = NULL;
	texture = NULL;

    return std::make_tuple(KeyTexture{ pianoTexture, location, h, w }, KeyTexture{ blendedTexture , location, h, w });
}

// same as above but without the gradient applied - for the black shadow
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

// Helper function to get the KeyTexture from a tuple based on the position (true for left, false for right)
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

// Adds a note to the notesPlayed map, or updates it if it already exists
void PianoKeyboard::RemoveNote(int key_pos) {
	auto it = this->notesPlayed.find(key_pos);
	if (it != this->notesPlayed.end()) { // If the note exists, remove it
		this->notesPlayed.erase(it);
	}
}

// Sets the pedal notes to the notes that are currently played, so that they can be removed when the pedal is released
void PianoKeyboard::SetPedalNotes() {
	std::map<int, Note> notes;
	for (auto i : GetNotesPlayed()) {
		notes.insert({ i.first, {i.second.played, this->pedal, i.second.pressed , i.second.velocity } });
	}
	this->notesPlayed = notes;
}

// Removes notes that are not pressed and are not pedal notes
void PianoKeyboard::RemovePedalNotes() {
	for (auto i : GetNotesPlayed()) {
		if (!i.second.pedal && !i.second.pressed) {
			RemoveNote(i.first);
		}
	}
}

// Detects keys pressed on the MIDI input and updates the notesPlayed map accordingly
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

// Returns a string representation of the chord played based on the notes in the notesPlayed map
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

// Loads the piano textures with all the info needed for rendering
bool PianoKeyboard::LoadPianoTextures(SDL_Renderer * renderer, int height) {
	SDL_Texture* pianoTexture = NULL;
	std::string pianoFolderPath = CAT(ASSETS_IMAGES_PATH,
		CAT(PIANO_FOLDER_PATH,
			CAT(std::to_string(height), "p_")
		)
	);

	std::string location = CAT(pianoFolderPath, RWHITE_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, location);
	CHECK_RESULT(pianoTexture, "Error loading piano RWhiteKey texture: ");
	this->SetRWhiteKey(this->LoadKeyTex(renderer, pianoTexture, location, true));

	location = CAT(pianoFolderPath, LWHITE_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, location);
	CHECK_RESULT(pianoTexture, "Error loading piano LWhiteKey texture: ");
	this->SetLWhiteKey(this->LoadKeyTex(renderer, pianoTexture, location, true));

	location = CAT(pianoFolderPath, WHITE_MID_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, location);
	CHECK_RESULT(pianoTexture, "Error loading piano MidWhiteKey texture: ");
	this->SetMidWhiteKey(this->LoadKeyTex(renderer, pianoTexture, location, true));

	location = CAT(pianoFolderPath, WHITE_ROUNDMID_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, location);
	CHECK_RESULT(pianoTexture, "Error loading piano RoundWhiteKey texture: ");
	this->SetRoundWhiteKey(this->LoadKeyTex(renderer, pianoTexture, location, true));

	location = CAT(pianoFolderPath, BLACK_KEY_PATH);
	std::string blend_location = CAT(pianoFolderPath, BLACK_BLEND_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, blend_location);
	CHECK_RESULT(pianoTexture, "Error loading piano BlackKey texture: ");
	this->SetBlackKey(this->LoadKeyTex(renderer, pianoTexture, location, false));

	location = CAT(pianoFolderPath, BLACK_SHADOW_PATH);
	pianoTexture = LoadTexture(renderer, pianoTexture, location);
	CHECK_RESULT(pianoTexture, "Error loading piano BlackKeyShadow texture: ");
	this->SetBlackKeyShadow(this->LoadKeyTex(pianoTexture, location));

	pianoTexture = NULL;

	return true;
}

// Renders the piano keyboard on the screen based on the current state of the quantity of notes, notes played and the textures loaded
bool PianoKeyboard::RenderPiano(SDL_Renderer* renderer, Animations* animations, bool isPiano, int widt, int heigh) {
	if (!isPiano) return true;

	// DO A 88 KEY SETTING
	int total_keys = this->GetKeyNum();
	int full_octaves = total_keys / 12;
	int middle_c = 60; // MIDI note number for middle C (C4)

	int start_key = middle_c - total_keys / 2 + 12 * this->GetOctave();

	int remaining_keys = total_keys % 12;
	int white_keys = full_octaves * 7 + std::min(remaining_keys, 7);
	int black_keys = full_octaves * 5 + std::max(remaining_keys - 7, 0);
	double width = static_cast<double>(widt);
	double height = static_cast<double>(heigh);
	double shadow_key_width = width / white_keys;
	double white_key_width = width / white_keys;
	double white_key_height = this->GetRoundWhiteKey(true).h;
	double white_shadow_height = (height * (double)WHITE_SHADOW_HEIGHT) / ASSESTS_RES;
	double shadow_black_key_width = this->GetBlackKeyShadow().w * 49 / this->GetKeyNum();
	double shadow_black_key_height = this->GetBlackKeyShadow().h;
	double wblack_height = ((height * (double)WHITEB_KEY_HEIGHT) / ASSESTS_RES) + white_shadow_height;
	double black_key_width = this->GetBlackKey(true).w * 49 / this->GetKeyNum();
	double black_key_height = this->GetBlackKey(true).h;
	double black_height = (height * (double)BLACK_SHADOW_HEIGHT) / ASSESTS_RES;

	SDL_SetRenderDrawColor(renderer, 229, 229, 229, 255);
	SDL_RenderFillRect(renderer, GenerateFRect(0, height - white_shadow_height * 2, width, white_shadow_height * 2));

	// Render all keys in two passes: one for white, one for black (shadows and keys together)
	// Pass 1: White keys and their shadows
	for (int i = 0; i < white_keys; ++i) {

		// White key
		int octave = i / 7;
		int note_in_octave = i % 7;
		int key_pos = start_key + octave * 12 +
			(note_in_octave == 0 ? 0 :
				note_in_octave == 1 ? 2 :
				note_in_octave == 2 ? 4 :
				note_in_octave == 3 ? 5 :
				note_in_octave == 4 ? 7 :
				note_in_octave == 5 ? 9 : 11);

		bool pressed = this->GetNotesPlayed()[key_pos].pressed; // make option for if they in pedal aka this->piano->GetNotesPlayed().count(key_pos) > 0 (on black keys too)
		float wx = static_cast<float>(i * white_key_width);
		float ww = (i == white_keys - 1) ? static_cast<float>(width - wx) : static_cast<float>(shadow_key_width);

		SDL_Texture* white_key_texture = NULL;
		std::string key_name = CAT("Key", std::to_string(key_pos));
		if (pressed) {
			animations->StartAnimation(key_name, 0.025, Animations::AnimationCurve::EaseOut);

			white_key_texture =
				(i == white_keys - 1) ? this->GetRoundWhiteKey(false).tex :
				(i % 7 == 0) ? this->GetLWhiteKey(false).tex :
				(i % 7 == 6) ? this->GetRWhiteKey(false).tex :
				this->GetMidWhiteKey(false).tex;
		}
		else {
			animations->ReverseAnimation(CAT("Key", std::to_string(key_pos)), Animations::AnimationCurve::EaseOut);
			white_key_texture =
				(i == white_keys - 1) ? this->GetRoundWhiteKey(true).tex :
				(i % 7 == 0) ? this->GetLWhiteKey(true).tex :
				(i % 7 == 6) ? this->GetRWhiteKey(true).tex :
				this->GetMidWhiteKey(true).tex;
		}
		RenderTexture(renderer, white_key_texture, wx, height - white_key_height - white_shadow_height, ww, white_key_height + white_shadow_height * animations->AnimationState(key_name), false);
	}
	SDL_Texture* white_key_texture = NULL;

	double mult = white_key_width - shadow_black_key_width * 0.5;
	// Pass 2: Black key shadows and black keys
	for (int i = 0; i < black_keys; ++i) {
		// Black key shadow
		float sx = static_cast<float>(mult);
		float sw = static_cast<float>(shadow_black_key_width);
		RenderTexture(renderer, this->GetBlackKeyShadow().tex, sx, height - shadow_black_key_height - wblack_height, sw, shadow_black_key_height, false);

		// Black key
		int octave = i / 5;
		int note_in_octave = i % 5;
		int key_pos = start_key + octave * 12 +
			(note_in_octave == 0 ? 1 :
				note_in_octave == 1 ? 3 :
				note_in_octave == 2 ? 6 :
				note_in_octave == 3 ? 8 : 10);
		bool pressed = this->GetNotesPlayed()[key_pos].pressed;

		float bx = static_cast<float>(mult);
		float bw = static_cast<float>(black_key_width);

		SDL_Texture* black_key_texture = NULL;

		std::string key_name = CAT("Key", std::to_string(key_pos));
		if (pressed) { // do a white version of the black 
			animations->StartAnimation(key_name, 0.025, Animations::AnimationCurve::EaseOut);
			black_key_texture = this->GetBlackKey(false).tex;
		}
		else {
			animations->ReverseAnimation(CAT("Key", std::to_string(key_pos)), Animations::AnimationCurve::EaseOutBounce);
			black_key_texture = this->GetBlackKey(true).tex;
		}

		RenderTexture(renderer, black_key_texture, bx, height - black_key_height - black_height - wblack_height, bw, black_key_height + black_height * 1.03 * animations->AnimationState(key_name), false);

		int keynum = ((i + 1) % 5 == 2 || (i + 1) % 5 == 0) ? 2 : 1;
		mult += white_key_width * keynum;
	}
	SDL_Texture* black_key_texture = NULL;
	return true;
}

// Renders the chord recognition text on the screen if the scene is set to detect keys
bool PianoKeyboard::ChordsText(SDL_Renderer* renderer, Scene* scene, TTF_Font* font) {
	if (scene->GetDetectKeys()) {
		static SDL_FRect chord_rect;
		if (this->AreNotesUpdated()) {

			if (scene->GetChordReconTex()) {
				SDL_DestroyTexture(scene->GetChordReconTex());
				scene->SetChordReconTex(NULL);
			}

			SDL_Surface* chord_surface = TTF_RenderText_Blended(font, StrToPtr(this->GetChordPlayed()), 0, SDL_COLOR_FPS); // CHANGE TO OTHER FONT SIZE
			CHECK_RESULT(chord_surface, "Error rendering Chord Recognition surface: ", SDL_GetError());

			if (chord_surface) {
				chord_rect.y = 50.0f;
				chord_rect.x = 50.0f;
				chord_rect.w = (float)chord_surface->w;
				chord_rect.h = (float)chord_surface->h;
			}
			scene->SetChordReconTex(SDL_CreateTextureFromSurface(renderer, chord_surface));

			SDL_DestroySurface(chord_surface);
			chord_surface = NULL;

			CHECK_RESULT(scene->GetChordReconTex(), "Error rendering piano texture: ", SDL_GetError());
			//VOID_CHECK_RESULT(SDL_SetTextureScaleMode(this->FpsTexture, SDL_SCALEMODE_NEAREST), "Error setting FPS texture scale mode: ", SDL_GetError());
		}

		if (chord_rect.w != 0 && chord_rect.h != 0) {
			SDL_RenderTexture(renderer, scene->GetChordReconTex(), NULL, &chord_rect);
		}


	}
	return true;
}
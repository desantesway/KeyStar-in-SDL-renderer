#ifndef CHORD_H
#define CHORD_H

#include <array>
#include <vector>
#include <string>

// Chord structure to represent musical chords
struct Chord {
	unsigned short root = 0; // Root note of the chord (0-11 for C to B)
	std::string base_name; // Base name of the chord (e.g., "C", "Gm", etc.)
	std::vector<unsigned short> extra_tones; // Additional tones in the chord (e.g., 3rd, 5th, etc.)
	std::vector<unsigned short> omitted_tones; // Tones that are omitted in the chord (e.g., 7th, 9th, etc.)
    unsigned num_accidentals = 0; 
                
    bool operator<(const Chord& other) const {
        return num_accidentals < other.num_accidentals;
    }

};

#endif
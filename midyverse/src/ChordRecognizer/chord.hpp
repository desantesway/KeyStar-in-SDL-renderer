#ifndef CHORD_H
#define CHORD_H

#include <array>
#include <vector>
#include <string>

struct Chord {
    unsigned short root;
    std::string base_name;
    std::vector<unsigned short> extra_tones;
    std::vector<unsigned short> omitted_tones;
    unsigned num_accidentals;
                
    bool operator<(const Chord& other) const {
        return num_accidentals < other.num_accidentals;
    }

};

#endif
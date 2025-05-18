#pragma once
#include <set>
#include <vector>
#include <string>

const std::vector<std::pair<std::string, std::set<unsigned short>>> chord_db = {

    // Basic triads and most common chords first
    {"maj", {4, 7}},
    {"min", {3, 7}},
    {"dim", {3, 6}},
    {"aug", {4, 8}},
    {"sus2", {2, 7}},
    {"sus4", {5, 7}},

    // Dominant 7th and its variations
    {"7", {4, 7, 10}},
    {"min7", {3, 7, 10}},
    {"dim7", {3, 6, 9}},
    {"min7b5", {3, 6, 10}},
    {"aug7", {4, 8, 10}},

    // Major 7th family
    {"maj 7", {4, 7, 11}},
    {"minMaj 7", {3, 7, 11}},

    // Sixth chords
    {"6", {4, 7, 9}},
    {"min6", {3, 7, 9}},

    // Suspended variations
    {"7sus4", {5, 7, 10}},
    {"maj7sus4", {5, 7, 11}},
    {"9sus4", {5, 7, 10, 2}},

    // Ninth chords
    {"9", {4, 7, 10, 2}},
    {"min9", {3, 7, 10, 2}},
    {"maj9", {4, 7, 11, 2}},
    {"minMaj9", {3, 7, 11, 2}},

    // Special chords and intervals
    {"Octave", {12}},
    {"Perfect 5th", {7}},
    {"Major 3rd", {4}},
    {"minor 3rd", {3}},
    {"Perfect 4th", {5}},
    {"Tritone", {6}},


    // Single intervals
    {"Major 2nd", {2}},
    {"minor 2nd", {1}},
    {"minor 6th", {8}},
    {"Major 6th", {9}},
    {"minor 7th", {10}},
    {"Major 7th", {11}},

    // Extended chords (11th, 13th)
    {"11", {4, 7, 10, 2, 5}},
    {"min11", {3, 7, 10, 2, 5}},
    {"13", {4, 7, 10, 2, 5, 9}},

    // Altered dominants
    {"7b9", {4, 7, 10, 1}},
    {"7#9", {4, 7, 10, 3}},
    {"7alt", {4, 7, 10, 1, 6, 8}},

    // Less common chords follow
    {"6/9", {4, 7, 9, 2}},
    {"min6/9", {3, 7, 9, 2}},
    {"add9", {4, 7, 2}},
    {"madd9", {3, 7, 2}},
    {"add11", {4, 7, 5}},
    {"madd11", {3, 7, 5}},

    // Diminished extensions
    {"dim9", {3, 6, 9, 2}},
    {"dim11", {3, 6, 9, 2, 5}},

    // Augmented extensions
    {"aug9", {4, 8, 10, 2}},
    {"augMaj7", {4, 8, 11}},

    // Other extended chords
    {"maj11", {4, 7, 11, 2, 5}},
    {"minMaj11", {3, 7, 11, 2, 5}},
    {"maj13", {4, 7, 11, 2, 5, 9}},
    {"minMaj13", {3, 7, 11, 2, 5, 9}},
    {"9#11", {4, 7, 10, 2, 6}},
    {"13#11", {4, 7, 10, 2, 6, 9}},

    // Very specialized chords
    {"7b5", {4, 6, 10}},
    {"7#5", {4, 8, 10}},
    {"9b5", {4, 6, 10, 2}},
    {"9#5", {4, 8, 10, 2}},
    {"11b9", {4, 7, 10, 1, 5}},
    {"13b9", {4, 7, 10, 1, 5, 9}},
    {"13#9", {4, 7, 10, 3, 5, 9}},
    {"7#5#9", {4, 8, 10, 3}},
    {"7#5b9", {4, 8, 10, 1}},
    {"maj7b5", {4, 6, 11}},
    {"maj7#11", {4, 7, 11, 6}},
    {"sus2sus4", {2, 5, 7}},
    {"7sus2", {2, 7, 10}},
    {"maj7sus2", {2, 7, 11}},
    {"maj9sus4", {5, 7, 11, 2}}

    
};

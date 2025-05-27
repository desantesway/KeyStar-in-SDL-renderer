#include "chord.hpp"
#include "chord_db.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <tuple>

// Scoring function for chord match precision
inline int score_chord(const Chord& chord) {
    int score = 0;
    // Fewer accidentals is better
    score -= static_cast<int>(chord.num_accidentals) * 10;
    // Penalize for omitted root or third (assuming 0 and 4/3 are root/third intervals)
    for (auto omitted : chord.omitted_tones) {
        if (omitted == 0 || omitted == 3 || omitted == 4) // minor/major third
            score -= 20;
    }
    // Bonus for no omitted tones
    if (chord.omitted_tones.empty())
        score += 10;
    // Bonus for no extra tones
    if (chord.extra_tones.empty())
        score += 10;
    return score;
}

inline void insert_chords(const unsigned short root, const std::set<unsigned short>& intervals,
    std::multiset<Chord>& res) {
    std::vector<std::pair<Chord, int>> candidates;
    for (const auto& chord_entry : chord_db) {
        const auto& name = std::get<0>(chord_entry);
        const auto& notes = std::get<1>(chord_entry);

        Chord candidate;
        candidate.root = root;
        candidate.base_name = name;
        candidate.omitted_tones.clear();
        candidate.extra_tones.clear();

        std::set_difference(notes.begin(), notes.end(), intervals.begin(), intervals.end(),
            std::back_inserter(candidate.omitted_tones));
        std::set_difference(intervals.begin(), intervals.end(), notes.begin(), notes.end(),
            std::back_inserter(candidate.extra_tones));
        candidate.num_accidentals = candidate.extra_tones.size() + candidate.omitted_tones.size();

        int score = score_chord(candidate);
        candidates.emplace_back(candidate, score);
    }

    // Find the best score
    int best_score = std::numeric_limits<int>::min();
    for (const auto& pair : candidates) {
        if (pair.second > best_score)
            best_score = pair.second;
    }
    // Insert all chords with the best score
    for (const auto& pair : candidates) {
        if (pair.second == best_score)
            res.insert(pair.first);
    }
}

inline unsigned short get_note_distance(unsigned short root, unsigned short other) {
    return (other + 12 - root) % 12;
}

inline std::multiset<Chord> name_that_chord(const std::vector<size_t>& indices) {
    std::set<unsigned short> notes;
    for (auto index : indices) {
        notes.insert(static_cast<unsigned short>(index % 12));
    }
    std::multiset<Chord> result;
    for (const auto root : notes) {
        std::set<unsigned short> intervals;
        for (const auto other : notes) {
            if (other == root)
                continue;
            intervals.insert(get_note_distance(root, other));
        }
        insert_chords(root, intervals, result);
    }
    return result;
}
    
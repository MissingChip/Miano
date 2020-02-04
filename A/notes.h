#pragma once

#include <string>
#include <vector>

using namespace std;

namespace note_lookup{
    typedef const int offset;
    const static char note_letters[] = {'C','D','D','E','E','F','G','G','A','A','B','B'};
    /*-----------------------A,B,C,D,E,F,G*/
    offset note_offsets[] = {0,2,3,5,7,8,10};
    offset major_offsets[] = {0, 2, 4, 5, 7, 9, 11, 12}; //offset from root in semitones
    offset minor_dif[] = {0, 0, -1, 0, 0, -1, -1, 0}; //difference from major chord offsets
    offset harmonic_minor_dif[] = {0, 0, -1, 0, 0, -1, 0, 0}; //difference from major chord offsets
    offset ascending_melodic_minor_dif[] = {0, 0, -1, 0, 0, 0, 0, 0}; //difference from major chord offsets
    offset descending_melodic_minor_dif[] = {0, 0, -1, 0, 0, -1, -1, 0}; //difference from major chord offsets
    /*offsets from root in semitones*/
    offset half_step = 1, semitone = 1;
    offset whole = 2, whole_step = 2, step = 2;
    offset minor_third = 3;
    offset major_third = 4;
    offset fourth = 5;
    offset diminished_fifth = 6;
    offset fifth = 7;
    offset minor_sixth = 8;
    offset major_sixth = 9;
    offset diminished_seventh = 10;
    offset seventh = 11;
    offset octave = 12;
}

int note(char* note_str);

class MidiNote{
public:
    MidiNote(char midi);
    MidiNote(int midi);
    MidiNote(char* eng);
    string to_string();
    int value();
private:
    char midi;//a number 0-127
    char letter;//a leter A-G
    char accidental;//['b', 'n', '#']
    char octave;//a number 0-9
};

class NoteGroup{
public:
    NoteGroup();
    NoteGroup(MidiNote);
    NoteGroup(vector<MidiNote>);
private:
    vector<MidiNote> notes;
};


#include "notes.h"

using namespace note_lookup;

MidiNote::MidiNote(char midi){
    this->midi = midi;
    int l = midi%12;
    letter = note_letters[l];
    if(l == 1 || l == 3 || l == 6 || l == 8 || l == 10){
        accidental = 'b';
    }
    else{
        accidental = 'n';
    }
    octave = (midi-12)/12;
}

MidiNote::MidiNote(int midi) : MidiNote((char)midi){}
MidiNote::MidiNote(char* eng) : MidiNote(note(eng)){}

string MidiNote::to_string(){
    string o;
    o.push_back(letter);
    if(accidental != 'n'){
        o.push_back(accidental);
    }
    o.push_back(octave+'0');
    return o;
}

int MidiNote::value(){
    return (int)midi;
}

int note(char* note_str){
    /*
     * return MIDI note value from english note string
     * Invalid string input is undefined behavior
     * */
    int o = note_offsets[*note_str++-'A']+21;//start with 'A' at MIDI 21
    if(*note_str == 'b'){//check for flat
        note_str++;
        o--;//change note accordingly
    }
    else if(*note_str == '#'){//check for sharp
        note_str++;
        o++;//change note accordingly
    }
    o += (*note_str)*12;//offset by # of octaves (i.e. A4 is 4 octaves above A0), 12 notes/octave
    return o;
}

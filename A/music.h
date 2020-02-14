#pragma once

#include <vector>
#include <portaudio.h>

#include "samples.h"
#include "list.h"

namespace music {
    static string sample_locations[128];
}

struct NoteInstruction{
    /*
     * 0: type
     * 1: instrument
     * 2: note
     * 3: ???
     */
    short type;
    short instrument;
    short note;
    short modifiers;
    /*starting frame*/
    ulong start;
    /*note duration*/
    uint duration;
};

class Music{
public:
    Music();
    unsigned int fill(float* buffer, unsigned int frames);
    ulong now();
    void jump(ulong frames);
    void seek(ulong frames);
    void reset();
    bool done();
    NoteInstruction add_note(NoteInstruction i);
    NoteInstruction add_note(char instrument, char note, ulong start, uint duration);
    NoteInstruction add_note_sec(char instrument, char note, double start, uint duration);
    int samplerate;
    
    void go();
    void stop();
    void write_file(FILE* fptr);
    
    static Music read_file(FILE* fptr);
    static ulong default_sample(float* out, char instrument, MidiNote n, ulong frames, ulong start = 0);
private:
    ulong (*sample)(float* out, char instrument, MidiNote n, ulong frames, ulong start);
    
    ulong head;
    unsigned int oldest;
    unsigned int on;
    unsigned int ch;
    
    List<NoteInstruction> tape;
    List<bool> done_v;
    
    PaStream* stream;
};

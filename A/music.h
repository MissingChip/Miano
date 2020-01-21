#pragma once

#include <vector>

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
    char info[4];
    /*starting frame*/
    ulong start;
    /*note duration*/
    uint duration;
};

class Music{
public:
    Music();
    unsigned int fill(float* buffer, unsigned int frames);
    ulong sample(float* out, char instrument, MidiNote n, ulong frames, ulong start = 0);
    ulong now();
    bool done();
    NoteInstruction add_note(NoteInstruction i);
    NoteInstruction add_note(char instrument, char note, ulong start, uint duration);
//private:
    ulong head;
    unsigned int oldest;
    unsigned int on;
    unsigned int ch;
    
    List<NoteInstruction> tape;
    List<bool> done_v;
};

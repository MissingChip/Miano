#pragma once

#include <vector>

#include "samples.h"

namespace music {
    static string sample_locations[128];
}

struct NoteInstruction{
    short type;
    char instrument;
    char note;
    union{
        float volume;
        unsigned int duration;
        int value;
    };
};

class Music{
public:
    Music();
    unsigned int fill(float* buffer, unsigned int frames);
    unsigned int sample(float* out, char instrument, MidiNote n, unsigned int frames, unsigned int start = 0);
//private:
    unsigned long head;
    unsigned int idx;
    unsigned int onsample;
    unsigned int ch;
    
    vector<NoteInstruction> tape;
    vector<NoteInstruction> active;
    vector<int> playtime;
};

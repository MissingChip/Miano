#pragma once

#include "notes.h"

unsigned int sample_aiff(float* out, MidiNote n, string file_base, unsigned int frames, unsigned int offset = 0);
unsigned int sample_piano(float* out, MidiNote n, unsigned int frames, unsigned int start = 0);

struct NoteSample{
    float samples[44100*4];
    short note;
    short instrument;
};


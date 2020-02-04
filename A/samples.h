#pragma once

#include "notes.h"

ulong sample_aiff(float* out, MidiNote n, string file_base, ulong frames, ulong offset = 0);
ulong sample_piano(float* out, MidiNote n, ulong frames, ulong start = 0);

struct NoteSample{
    float samples[44100*4];
    short note;
    short instrument;
};


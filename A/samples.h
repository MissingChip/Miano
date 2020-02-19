#pragma once

#include "notes.h"

ulong sample_aiff(float* out, MidiNote n, string file_base, string file_end,ulong frames, ulong offset = 0);
ulong sample_piano(float* out, MidiNote n, ulong frames, ulong start = 0);
ulong sample_flute(float* out, MidiNote n, ulong frames, ulong start = 0);
ulong sample_violin(float* out, MidiNote n, ulong frames, ulong start = 0, int sul = -1);

struct NoteSample{
    float samples[44100*4];
    short note;
    short instrument;
};

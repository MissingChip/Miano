#include <stdio.h>
#include <sndfile.h>
#include <string.h>

#include "samples.h"

unsigned int sample_aiff(float* out, MidiNote n, string file_base, unsigned int frames, unsigned int offset){    
    SF_INFO info;
    SNDFILE* audio_file = sf_open((file_base + n.to_string() + ".aiff").c_str(), SFM_READ, &info);
    sf_seek(audio_file, offset, SEEK_SET);
    unsigned int o = sf_readf_float(audio_file, out, frames);
    sf_close(audio_file);
    return o;
}

unsigned int sample_piano(float* out, MidiNote n, unsigned int frames, unsigned int start){
    return sample_aiff(out, n, "./Audio/Piano.ff.", frames, start);
}

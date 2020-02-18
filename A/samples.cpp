#include <stdio.h>
#include <sndfile.h>
#include <string.h>

#include "samples.h"

ulong sample_aiff(float* out, MidiNote n, string file_base, string file_end,ulong frames, ulong offset){    
    SF_INFO info;
    SNDFILE* audio_file = sf_open((file_base + n.to_string() + file_end).c_str(), SFM_READ, &info);
    sf_seek(audio_file, offset, SEEK_SET);
    ulong o = sf_readf_float(audio_file, out, frames);
    sf_close(audio_file);
    
    return o;
}

ulong sample_piano(float* out, MidiNote n, ulong frames, ulong start){    
    return sample_aiff(out, n, "../AudioSamples/Piano.ff.", ".aiff",frames, start);
}

ulong sample_flute(float* out, MidiNote n, ulong frames, ulong start){    
    return sample_aiff(out, n, "../AudioSamples/Flute.vib.ff.stereo/Flute.vib.ff.", ".stereo.aif",frames, start);
}

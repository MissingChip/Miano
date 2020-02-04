
#include <sndfile.h>
#include "music_write.h"


#define CHUNK 1024
ulong music_aiff(string file, Music music, ulong frames)
{
    //printf("writing to %s\n", file.c_str());
    SF_INFO info;
    info.frames = frames;
    info.samplerate = 44100;
    info.channels = 2;
    info.format = 0x20002;
    info.sections = 1;
    info.seekable = 1;
    SNDFILE* audio_file = sf_open(file.c_str(), SFM_WRITE, &info);
    long done = CHUNK;
    long written = 0;
    float write_b[CHUNK*2];
    while(done == CHUNK && written < frames){
        music.fill(write_b, CHUNK);
        done = sf_writef_float(audio_file, write_b, CHUNK);
        written += done;
    }
    sf_close(audio_file);
    return written;
}

ulong music_wav(string file, Music music, ulong frames)
{
    SF_INFO info;
    info.channels = 2;
    info.frames = frames;
    info.samplerate = 44100;
    info.channels = 2;
    info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    info.sections = 1;
    info.seekable = 1;
    SNDFILE* audio_file = sf_open(file.c_str(), SFM_WRITE, &info);
    long done = CHUNK;
    long written = 0;
    float write_b[CHUNK*2];
    while(done == CHUNK && written < frames){
        music.fill(write_b, CHUNK);
        done = sf_writef_float(audio_file, write_b, CHUNK);
        written += done;
    }
    sf_close(audio_file);
    return written;
}

#pragma once

#include <portaudio.h>
#include "music.h"

#define FPB 256

static int paCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    Music* m = (Music*)userData;
    float* o = (float*)outputBuffer;
    m->fill(o, framesPerBuffer);
    return 0;
}

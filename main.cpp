#include <stdio.h>
#include <portaudio.h>

#include "A/music.h"

#define FPB 256
#define SAMPLE_RATE 44100
using namespace music;

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
int main(int argc, char **argv) {
    Music m;
    m.tape.push_back(NoteInstruction{1, 0, 69});
    NoteInstruction a {0, 1, 0};
    a.duration = 44100*4;
    m.tape.push_back(a);
    PaStream* stream;
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream,
        0, /*no input*/
        2, /*2 output channels*/
        paFloat32,
        SAMPLE_RATE,
        FPB, /*frames per buffer*/
        paCallback,
        &m
    );
    Pa_StartStream( stream );
    Pa_Sleep( 4000 );
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
    Pa_Terminate();
    return 0;
}

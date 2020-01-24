
#include "music_play.h"


#define FPB 256
#define SAMPLE_RATE 44100

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

void play(Music m){
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
    int i=0;
    while(!m.done()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //m.test();
    }
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
}

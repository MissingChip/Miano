#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <chrono>
#include <thread>

#include "A/music.h"

#define FPB 256
#define SAMPLE_RATE 44100
using namespace music;
using namespace note_lookup;

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
    char note = 69;
    long beat = 44100/2;
    srand(245235);
    for(int i=0;i<20;i++)
    {
        int o = rand()%2*2;
        note += rand()%2*2-1;
        m.add_note(0, note+o, i*beat, 41000*4);
        if(i%2 == 1){
            m.add_note(0, note+o-major_third, i*beat, 44100*12);
        }
        if(i%4 == 1){
            m.add_note(0, note+o+minor_third, i*beat+beat/2, 44100*12);
        }
    }
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
    while(!m.done()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    //Pa_Sleep(10000);
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
    Pa_Terminate();
    return 0;
}

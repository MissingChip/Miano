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
    char note_max = 83;
    char note_min = 60;
    long beat = 44100/3;
    srand(5234);
    int wait = 0;
    for(int i=0;i<1080;i++)
    {
        int o = rand()%2*2;
        if(i%2 == 0 && wait<3){
            m.add_note(0, note, i*beat*2, 41000*12);
        }
        if(i%8 == 0){
            char p = rand()%(note_max-note_min);
            if(p<(note_max-note)*2/3){
                note += major_third;
            }
            else if(p<(note-note_min)/2){
                note -= major_third;
            }
            else if(p<(note-note_min)*4/5){
                note --;
            }
            if(note < note_min || note > note_max){
                printf("Note below min: %d\n", note);
            }
        }
        if(rand()%7 < 6 && wait == 0){
            if(i%2 == 1){
                m.add_note(0, note+o-major_third, i*beat*2, 44100*12);
            }
            if(i%2 == 1 && rand()%8 < 7){
                m.add_note(0, note-octave, i*beat*2, 44100*12);
            }
            if(i%4 == 1 && rand()%4 < 3){
                m.add_note(0, note+o+minor_third, i*beat*2+beat, 44100*12);
            }
            if(rand()%2 == 0 && i%4 == 3){
                m.add_note(0, note+octave+major_third, i*beat*2, 44100*12);
            }
        }
        else if(wait > 0){
            wait--;
        }
        else{
            wait = 3;
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
    int i=0;
    while(!m.done()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        /*
        if(i%10 == 0){
            for(int a=0;a<m.tape.size();a++){
                printf("%d", (int)m.done_v[a]);
            }
            printf("\n");
        }
        i++;*/
    }
    //Pa_Sleep(10000);
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
    Pa_Terminate();
    return 0;
}

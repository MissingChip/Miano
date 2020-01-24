#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <chrono>
#include <thread>

#include "A/music.h"
#include "A/music_play.h"
#include "A/music_write.h"

#define FPB 256
#define SAMPLE_RATE 44100
using namespace music;
using namespace note_lookup;

int main(int argc, char **argv) {
    Music m;
    char note = 69;
    char note_max = 83;
    char note_min = 60;
    long beat = 44100/3;
    srand(93876);
    int wait = 0;
    
    for(int i=0;i<1080;i++)
    {
        int o = rand()%2*2;
        if(i%2 == 0 && wait<3){
            m.add_note(0, note, i*beat*2, 41000*16);
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
                printf("Note out of range: %d\n", note);
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
            /*if(rand()%2 == 0 && i%4 == 3){
                m.add_note(0, note+octave+major_third, i*beat*2, 44100*12);
            }*/
        }
        else if(wait > 0){
            wait--;
        }
        else{
            wait = 3;
        }
            
    }
    //m.test();
    music_wav("sleepy2.wav", m, 44100*120);
    //printf("written\n");
    //m.reset();
    //printf("reset\n");
    //play(m);
    return 0;
}

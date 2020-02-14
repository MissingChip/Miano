
#include <stdlib.h>
#include "music_generator.h"

using namespace music;
using namespace note_lookup;

Music generate(int beats, float beat_length)
{
    Music m;
    char note = 69;
    char note_max = 83;
    char note_min = 60;
    long beat = 44100*beat_length;
    int wait = 0;
    
    for(int i=0;i<beats;i++)
    {
        ulong start = i*beat;
        int o = rand()%2*2;
        if(i%2 == 0 && wait<3){
            m.add_note(0, note, start, 41000*16);
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
                m.add_note(0, note+o-major_third, start, 44100*12);
            }
            if(i%2 == 1 && rand()%8 < 7){
                m.add_note(0, note-octave, start, 44100*12);
            }
            if(i%4 == 1 && rand()%4 < 3){
                m.add_note(0, note+o+minor_third, start+beat/2, 44100*12);
            }
        }
        else if(wait > 0){
            wait--;
        }
        else{
            wait = 3;
        }
    }
    return m;
}

Music generate2(int beats, float beat_length)
{
    Music m;
    long beat = 44100*beat_length;
    vector<vector<int>> pallette;
    vector<int> canvas;
    for(int i=0;i<4;i++){
        int a = rand()%3+4;
        vector<int> p;
        for(int i=0;i<a;i++){
            p.push_back(rand()%4);
        }
        pallette.push_back(p);
    }
    for(int i=0;i<beats;){
        vector<int> pp = pallette[(i/20)%4];
        for(int j0=rand()%2;j0<pp.size();j0++){
            vector<int> p = pallette[pp[j0]];
            for(int j1=0;j1<p.size();j1++){
                canvas.push_back(p[j1]);
                i++;
            }
        }
    }
    int note = 70;
    
    for(int i=0;i<beats && i<canvas.size();i++){
        int a = pallette[i%4][(i/20)%4];
        int b = a+canvas[i];
        for(int j=10; j>0; j--){
            
        }
    }
    return m;
}

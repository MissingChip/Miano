#include <cstring>

#include "music.h"

using namespace music;

enum InstructionType{
    control = 0,
    play,
    stop
};

Music::Music(){
    head = 0;
    idx = 0;
    onsample = 0;
    ch=2;
}

unsigned int Music::sample(float* out, char instrument, MidiNote n, unsigned int frames, unsigned int start){
    return sample_piano(out, n, frames, start);
}

unsigned int Music::fill(float* buffer, unsigned int frames){
    float add[frames*ch];
    float* b = buffer;
    for(int i=0;i<frames*ch;i++){
        *b++ = 0;
    }
    unsigned int done = 0;
    while(done < frames && idx<tape.size()){
        if(tape[idx].type == play){
            active.push_back(tape[idx]);
            playtime.push_back(0);
            printf("push %d %d\n", tape[idx].instrument, tape[idx].note);
            idx++;
        }
        else if((int)tape[idx].type == control && (int)tape[idx].instrument == play){
            unsigned int dur = min(tape[idx].duration-onsample, frames-done);
            for(int i=0;i<active.size();i++){
                uint amt = sample(add, active[i].instrument, active[i].note, dur, playtime[i]);
                for(int j=0;j<amt;j++){
                    buffer[i] += add[i];
                }
                if(amt < dur){
                    playtime[i] = 0;
                    active[i].type = 0;
                }
                else{
                    playtime[i] += amt;
                }
            }
            onsample += dur;
            if(onsample >= tape[idx].duration){
                onsample = 0;
                idx++;
            }
            done += dur;
        }
        else if(tape[idx].type == stop){
            int i = 0;
            while(active[i].instrument != tape[idx].instrument || active[i].note != tape[idx].note){
                i++;
                if(i > active.size()){
                    i = -1;
                }
            }
            if(i>0){
                active[i].type = 0;
                playtime[i] = 0;
            }
            idx++;
        }
    }
    head += frames;
}

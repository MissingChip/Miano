#include <cstring>
#include <math.h>

#include "music.h"

using namespace music;

enum InstructionType{
    control = 0,
    play,
    stop
};

Music::Music(){
    head = 0;
    oldest = 0;
    on = 0;
    ch=2;
}

ulong Music::now(){
    return head;
}

bool Music::done()
{
    ulong finish = 0;
    for(int i=0;i<tape.size();i++){
        finish = max(finish, tape[i].start+tape[i].duration);
    }
    return finish <= head;
}

ulong Music::sample(float* out, char instrument, MidiNote n, ulong frames, ulong start){
    return sample_piano(out, n, frames, start);
}

unsigned int Music::fill(float* buffer, unsigned int frames){
    float add[frames*ch];
    float* b = buffer;
    for(int i=0;i<frames*ch;i++){
        *b++ = 0;
    }
    uint i = oldest;

    while(tape[i].start < head+frames && i<tape.size()){
        on = max(i, on);
        NoteInstruction t = tape[i];
        ulong frames_to_play = min(t.start+t.duration-head, (ulong)frames);
        if(frames_to_play > 0){
            sample(add, t.info[1], t.info[2], frames_to_play, head-t.start);
            float* a = add;
            b = buffer;
            for(int c=0;c<frames_to_play*ch;c++){
                *b++ += *a++;
            }
        }        
        if((frames_to_play < frames) && i == oldest){
            oldest++;
        }
        i++;
    }
    head += frames;
}

NoteInstruction Music::add_note(char instrument, char note, ulong start, uint duration)
{
    NoteInstruction a;
    
    a.info[0] = 0;
    a.info[1] = instrument;
    a.info[2] = note;
    a.start = start;
    a.duration = duration;
    tape.push_back(a);
    return a;
}

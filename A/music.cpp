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
void Music::reset(){
    head = 0;
    oldest = 0;
    on = 0;
    ch=2;
    for(int i=0;i<done_v.size();i++){
        done_v[i] = 0;
    }
}

bool Music::done()
{
    for(int i=oldest;i<tape.size();i++){
        if(!done_v[i]){
            return false;
        }
    }
    return true;
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
    ulong min_frames = frames;
    ulong max_frames = 0;
    while(i<tape.size() && tape[i].start < head+frames){
        on = max(i, on);
        NoteInstruction t = tape[i];
        long frames_to_play = min((long)(t.start+t.duration)-(long)head, (long)frames);
        long sampled_amount;
        if(frames_to_play < 0){
            frames_to_play = 0;
        }
        else if(frames_to_play > 0){
            sampled_amount = sample(add, t.info[1], t.info[2], frames_to_play, head-t.start);
            float* a = add;
            b = buffer;
            long add_to = min(frames_to_play, sampled_amount)*ch;
            for(int c=0;c<add_to;c++){
                if(*a >= 1.0 || *a <= -1.0){
                    printf("Memory error at %ld note %d, value %f\n", head, t.info[1], *a);
                }
                *b++ += *a++;
            }
            if(add_to < frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest++;
                }
            }
            min_frames = min(min_frames, (ulong)sampled_amount);
            
        }
        min_frames = min(min_frames, (ulong)frames_to_play);
        max_frames = max(max_frames, (ulong)frames_to_play);
        if(frames_to_play != frames){
            done_v[i] = true;
            if(i == oldest){
                oldest++;
                tape.seek(oldest);
            }
        }
        i++;
    }
    head += frames;
    return max_frames;
}

NoteInstruction Music::add_note(char instrument, char note, ulong start, uint duration)
{
    NoteInstruction a;
    
    a.info[0] = 0;
    a.info[1] = instrument;
    a.info[2] = note;
    a.start = start;
    a.duration = duration;
    int i = tape.recent_idx();
    if(tape.size() == 0){
        tape.push_front(a);
        done_v.push_front(false);
        return a;
    }
    if(tape[i].start >= start)
    {
        tape.seek(0);
        done_v.seek(0);
        i = 0;
    }
    while(i < tape.size() && tape[i].start < start)
    {
        i++;
    }
    tape.insert(i, a);
    done_v.insert(i,false);
    return a;
}

void Music::test(){

    for(int i=on+1;i<tape.size();i++){
        if(tape[i].start <= head){
            printf("Missed %d a %lu", tape[i].info[2] ,tape[i].start);
        }
    }/*
    int prev = 0;
    int ouch = 0;
    for(int i=0;i<tape.size();i++){
        if(tape[i].start < prev){
            printf("%d %lu\n", prev, tape[i].start);
            ouch++;
        }
        prev = tape[i].start;
    }
    printf("ouch %d\n", ouch);*/
}

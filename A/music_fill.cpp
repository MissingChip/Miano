#include <math.h>

#include "music.h"

using namespace music;
namespace in=instructions;

uint Music::fill(float* buffer, unsigned int frames)
{
    if(tape_safe<tape_ptp.size()-1){
        fprintf(stderr, "Warning: running `fill()` before `initialize()` results in unwanted behavior\n");
    }
    if(frames == 0){return 0;};
    float* b = buffer;
    for(int i=0; i<frames*channels; i++){
        *b++ = 0;
    }
    //change head at beginning so now() returns a more accurate value (as a sort of thread "safety" measure)
    working_head = head;
    head += frames;
    uint i = oldest;
    uint max_frames = 0;
    //go through all viable instructions
    while(i<tape.size() && tape[i].start < working_head+frames){
        int size = 1+tape[i].modifiers;
        if(!done_v[i]){
            uint f = follow(&tape[i], buffer, frames);
            if(f<frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest+=size;
                }
            }
            max_frames = max(max_frames, f);
        }else if(i==oldest){oldest++;}
        i+=size;
    }
    return max_frames;
}
uint Music::follow(NoteInstruction* instruction, float* buffer, uint frames)
{
    // addition buffer
    float* add_buff = new float[frames*channels];
    float* a = add_buff;
    NoteInstruction i = *instruction;
    
    // frames to play
    uint p = frames_left(instruction);
    p = min(p, frames);// fill all frames if requested, otherwise fill only to end of note
    p = sample(a, i.instrument, i.note, p, working_head-i.start);
    
    // apply modifiers
    modify(instruction, a, p);
    
    // add to buffer
    for(int i=0;i<p*channels;i++){
        *buffer++ += *a++;
    }
    delete[] add_buff;
    return p;
}
uint Music::frames_left(NoteInstruction* in)
{
    NoteInstruction i = *in;
    //For potentially adding instructions which alter length (this seems unnecessary and not ideal, but who knows what will make sense)
    /*
    uint p = 0;
    for(int j=0;j<i.modifiers;j++){
        p += frames_taken(*in++);
    }
    */
    // checking for unsigned subtraction overflow
    ulong end = i.start + i.duration;
    if(end > working_head){
        return end - working_head;
    }
    else{
        return 0;
    }
}
uint Music::frames_taken(NoteInstruction i){
    return 0;
}

int Music::modify(NoteInstruction* in, float* out, uint filled)
{
    int mods = in->modifiers;
    if(mods == 0){ return 0; }
    NoteInstruction instruction = *in;
    long st = in->start;
    /*
    for(int i=0;i<mods;i++){
        if(in->type == in::volume){
            long start = in->start;
            if(in->type2 & in::from_end){
                start = st-start;
            }
            if(in->type2 == in::constant){
                float* o = out+start;
                float m = (float)in->uval1/(float)0xffff;
                for(int i=start;i<filled*channels&&i<start+in->duration;i++){
                    *o++ *= m;
                }
            }
            if(in->type2 == in::linear){
                float* o = out+start;
                float a, m;
                if(in->uval1 == 0xffff){
                    a = 1.0;
                    if(in->uval2 == 0){
                        m = (in->duration)/(float)0xffff;
                    }
                    else{
                        m = (float)(in->uval2 - in->uval1)/(float)(in->duration)/(float)0xffff;// volume slope
                    }
                }
                else{
                    a= (float)in->uval1/(float)0xffff;// starting volume
                }
                for(int i=0;i<filled&&i<in->duration;i++){
                    *o++ *= a+m*i;
                }
            }
        }
        if(in->type == in::custom){
            if(custom_modify){
                custom_modify(in, out, filled);
            }
            else{
                fprintf(stderr, "Error: custom_modify() used but undefined - doing nothing\n");
            }
        }
    }*/
    return mods; //Number of modifiers read/executed
}

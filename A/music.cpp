#include <stdio.h>
#include <math.h>

#include "music.h"

using namespace music;
namespace in=instructions;

Music::Music(){
    head = 0;
    oldest = 0;
    on = 0;
    channels=2;
    samplerate = 44100;
    sample = default_sample;
    tape_safe=-1;
}

void Music::jump(ulong frames){
    /*
     * Music::jump - skip over upcoming frames
     * */
    if(frames == 0){return;};
    //change head at beginning so now() returns a more accurate value (as a sort of thread "safety" measure
    working_head = head;
    head += frames;
    uint i = oldest;
    //go through all viable instructions
    while(i<tape.size() && tape[i].start < working_head+frames){
        NoteInstruction t = tape[i];
        //skip the note if it is of the correct type (and not done)
        if(t.type == instructions::play && !done_v[i]){
            on = max(i, on);
            //if note doesn't fill the frame, the note must be done
            if((long)(t.start+t.duration)-(long)working_head < frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest++;
                }
            }
        }
        i+=1+tape[i].modifiers;
    }
}

ulong Music::now(){
    return head;
}
void Music::reset(){
    head = 0;
    oldest = 0;
    on = 0;
    for(int i=0;i<done_v.size();i++){
        done_v[i] = 0;
    }
}
void Music::initialize()
{
    for(int i = tape_safe+1;i<tape_ptp.size();i++){
        if(i>tape.size()){
            fprintf(stderr, "Something went wrong: tape is safe to element %ld, but only has %lu elements", tape_safe, tape.size());
            i = tape.size();
        }
        if(i==tape.size()){
            tape.push_back(tape_ptp.seek(i));
        }else{
            tape[i] = tape_ptp.seek(i);
        }
    }
    if(done_v.size() != tape_ptp.size()){
        fprintf(stderr, "!!!~ done vector size != tape prototype size ~!!!\n");
        exit (1);
    }
    for(int i=tape_safe;i<done_v.size();i++){
        done_v[i] = false;
    }
    tape_safe = tape.size()-1;
}


void Music::seek(ulong frames){
    /*
     * Music::seek - set head to <frames>
     * */
    if(frames > head){
        jump(head-frames);
        return;
    }
    reset();
    jump(frames);
}

NoteInstruction Music::add_note(short instrument, short note, ulong start, uint duration)
{
    /*
     * add a note to the music object
     * starting at number of frames <start> from beginning of music
     */
    NoteInstruction a;
    //set instrution values
    a.type = instructions::play;
    a.instrument = instrument;
    a.note = note;
    a.start = start;
    a.duration = duration;
    add_note(a);
    return a;
}
NoteInstruction Music::add_note(NoteInstruction a)
{
    /*
     * put instruction in the correct location
     *  (after any notes that start at the same time
     *  as input instruction)
     */
    int i = tape_ptp.recent_idx();
    if(tape_ptp.size()>0){
        if(tape_ptp[i].start >= a.start)
        {
            tape_ptp.seek(0);
            i = 0;
        }
        while(i < tape_ptp.size() && tape_ptp[i].start <= a.start)
        {
            i+=1+tape_ptp[i].modifiers;
        }
    }
    else{
        i = 0;
    }
    FILE* f = fopen("log", "a");
    if(i == tape_ptp.size() && tape_safe == i-1){
        tape.push_back(a);
        tape_safe = i;
    }
    else if(i<=tape_safe){
        tape_safe = i-1;
    }
    fclose(f);
    tape_ptp.insert(i, a);
    done_v.push_back(false);
    return a;
}

NoteInstruction Music::add_note_sec(short instrument, short note, double start, uint duration)
{
    /*
     * add note starting at a number of seconds (rather than a number of frames)
     */
    return add_note(instrument, note, start*(ulong)samplerate, duration);
}
NoteInstruction Music::get_ins(int i)
{
    return tape[i];
}
void Music::set_ins(int i, NoteInstruction ins)
{
    if(tape[i].start != ins.start && tape[i].type == ins.type){
        fprintf(stderr, "Note instruction start or type must match for set_ins");
        return;
    }
    tape[i] = ins;
}

NoteInstruction Music::rm_ins(int i)
/* remove instruction and associated modifiers if type==play */
{
    NoteInstruction ins = tape[i];
    if(tape[i].type == in::play && tape[i].modifiers > 0){
        tape_ptp.remove(i, i+tape[i].modifiers);
        tape_safe = i-1;
    }
    else{
        tape_ptp.remove(i);
    }
    return ins;
}

ulong Music::default_sample(float* out, short instrument, MidiNote n, ulong frames, ulong start){
    switch(instrument){
        case(in::piano):
            return sample_piano(out, n, frames, start);
        case(in::flute):
            return sample_flute(out, n, frames, start);
        case(in::flute_vib):
            return sample_aiff(out, n, "../AudioSamples/Flute.vib.ff.stereo/Flute.vib.ff.", ".stereo.aif",frames, start);
        case(in::oboe):
            return sample_aiff(out, n, "../AudioSamples/Oboe/Oboe.ff.", ".stereo.aif",frames, start);
        case(in::violin):
            return sample_violin(out, n, frames, start);
    }
    return 0;
}
bool Music::done()
{
    /*
     * returns boolean of whether or not all notes have been completed
     * A note is done if its duration is up or sampled fewer values than
     * buffer required
     */
    for(int i=oldest;i<tape.size();i++){
        if(!done_v[i]){
            return false;
        }
    }
    return true;
}

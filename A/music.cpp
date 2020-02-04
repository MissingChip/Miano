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
    samplerate = 44100;
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
    /*
     * returns whether or not all notes have been completed
     */
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
    /*
     * Music::fill - fill an audio buffer with upcoming music
     * Buffer should be at least the size of frames*(# of channels)
     *      (usually frames*2)
     * */
    
    //buffer to add to [buffer]
    float add[frames*ch];
    float* b = buffer;
    //clear [buffer]
    for(int i=0;i<frames*ch;i++){
        *b++ = 0;
    }
    uint i = oldest;
    ulong min_frames = frames;
    ulong max_frames = 0;
    //go through all viable instructions
    while(i<tape.size() && tape[i].start < head+frames){
        NoteInstruction t = tape[i];
        //play the note if it is of the correct type (and not done)
        if(t.type == play && !done_v[i]){
            on = max(i, on);
            //fill the buffer if needed, or play leftover
            long frames_to_play = min((long)(t.start+t.duration)-(long)head, (long)frames);
            long sampled_amount;
            if(frames_to_play < 0){
                frames_to_play = 0;
            }
            else if(frames_to_play > 0){
                //get samples for instrument and note
                sampled_amount = sample(add, t.instrument, t.note, frames_to_play, head-t.start);
                float* a = add;
                b = buffer;
                //add as many samples as we got, at most
                long add_to = min(frames_to_play, sampled_amount)*ch;
                for(int c=0;c<add_to;c++){
                    *b++ += *a++;
                }
                //if we didn't fill the frame, the note must be done
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
            //if we didn't fill the buffer, the note is done
            if(frames_to_play != frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest++;
                    tape.seek(oldest);
                }
            }
        }
        i++;
    }
    head += frames;
    return max_frames;
}

NoteInstruction Music::add_note(char instrument, char note, ulong start, uint duration)
{
    /*
     * add a note to the music object
     */
    NoteInstruction a;
    //set instrution values
    a.type = play;
    a.instrument = instrument;
    a.note = note;
    a.start = start;
    a.duration = duration;
    
    /*
     * put instruction in the correct location
     * it's janky
     */
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

NoteInstruction Music::add_note_sec(char instrument, char note, float start, uint duration)
{
    return add_note(instrument, note, start*(ulong)samplerate, duration);
}

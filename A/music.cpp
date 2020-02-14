#include <cstring>
#include <math.h>

#include "music.h"
#include "pa_callback.h"

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
    sample = default_sample;
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

ulong Music::default_sample(float* out, char instrument, MidiNote n, ulong frames, ulong start){
    return sample_piano(out, n, frames, start);
}

unsigned int Music::fill(float* buffer, unsigned int frames){
    /*
     * Music::fill - fill an audio buffer with upcoming music
     * Buffer should be at least the size of frames*(# of channels)
     *      (usually frames*2)
     * */
    ulong h = head;
    head += frames;
    //buffer to add to [buffer]
    float add[frames*ch];
    float* b = buffer;
    //clear [buffer]
    memset(b, 0, frames*ch*sizeof(float));
    uint i = oldest;
    ulong min_frames = frames;
    ulong max_frames = 0;
    //go through all viable instructions
    while(i<tape.size() && tape[i].start < h+frames){
        NoteInstruction t = tape[i];
        //play the note if it is of the correct type (and not done)
        if(t.type == play && !done_v[i]){
            on = max(i, on);
            //fill the buffer if needed, or play leftover
            long frames_to_play;
            long sampled_amount;
            if(t.start+t.duration<h){
                frames_to_play = 0;
            }
            else{
            frames_to_play = min(t.start+t.duration-h, (ulong)frames);
                //get samples for instrument and note
                sampled_amount = sample(add, t.instrument, t.note, frames_to_play, h-t.start);
                float* a = add;
                b = buffer;
                //add as many samples as we got, at most
                long add_to = min(frames_to_play, sampled_amount)*ch;
                for(int c=0;c<add_to;c++){
                    *b++ += *a++;
                }
                //if note didn't fill the frame, the note must be done
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
            //if note didn't fill the buffer, the note is done
            if(frames_to_play != frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest++;
                    tape.seek(oldest);
                }
            }
        }
        i+=tape[i].modifiers+1;
    }
    return max_frames;
}
void Music::jump(ulong frames){
    /*
     * Music::jump - skip over upcoming frames
     * */
    //change head at beginning so now() returns a more accurate value
    ulong h = head;
    head += frames;
    if(frames == 0){return;};
    uint i = oldest;
    //go through all viable instructions
    while(i<tape.size() && tape[i].start < h+frames){
        NoteInstruction t = tape[i];
        //skip the note if it is of the correct type (and not done)
        if(t.type == play && !done_v[i]){
            on = max(i, on);
            //if note doesn't fill the frame, the note must be done
            if((long)(t.start+t.duration)-(long)h < frames){
                done_v[i] = true;
                if(i == oldest){
                    oldest++;
                    tape.seek(oldest);
                }
            }
        }
        i++;
    }
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
    add_note(a);
    return a;
}
NoteInstruction Music::add_note(NoteInstruction a)
{
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
    if(tape[i].start >= a.start)
    {
        tape.seek(0);
        done_v.seek(0);
        i = 0;
    }
    while(i < tape.size() && tape[i].start < a.start)
    {
        i++;
    }
    tape.insert(i, a);
    done_v.insert(i,false);
    return a;
}

NoteInstruction Music::add_note_sec(char instrument, char note, double start, uint duration)
{
    return add_note(instrument, note, start*(ulong)samplerate, duration);
}

void Music::write_file(FILE* fptr)
{
    for(int i=0;i<tape.size();i++){
        NoteInstruction t = tape[i];
        fprintf(fptr, "%d %d %d %d %lu %u\n", t.type, t.instrument, t.note, t.modifiers, t.start, t.duration);
    }
}
Music Music::read_file(FILE* fptr)
{
    Music m;
    NoteInstruction a;
    while(fscanf(fptr, "%hd %hd %hd %hd %lu %u\n", &(a.type), &(a.instrument), &(a.note), &(a.modifiers), &(a.start), &(a.duration)) == 6){
        m.add_note(a);
    }
    return m;
}

void Music::go()
{
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream,
        0, /*no input*/
        2, /*2 output channels*/
        paFloat32,
        samplerate,
        256, /*frames per buffer*/
        paCallback,
        this
    );
    Pa_StartStream( stream );
}

void Music::stop()
{
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
}



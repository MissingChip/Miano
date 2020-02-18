#pragma once

/*
 * system include
 */
#include <vector>
#include <portaudio.h> //move to library include?

/*
 * library include
 */
#include <list.h>

/*
 * internal include
 */
#include "samples.h"

namespace music {
    static string sample_locations[128];
    namespace instructions {
        enum { //NoteInstruction::type
            control,
            play,
            stop,
            volume,
            custom
        };
        enum VolumeType{ //NoteInstruction::type2
            /*All VolumeType instructions using uval1 or uval2
             * convert them to a float in the following manner
             *   (float)uvalx/(float)0xffff
             * i.e. a linear range from 0 to 65535 (max unsigned short)
             */
            /* constant volume equal to uval1 */
            constant,
            /*TODO constant volume equal to value_d */
            constant_double,
            /* linear envelope from uval1 to uval2 over duration */
            linear,
            exp_ramp,
            exp_land,
            from_end = 0x0100
        };
    }
}

struct NoteInstruction{
    short type;
    union{
        short instrument;
        short type2;
    };
    union{
        short note;
        ushort uval1;
    };
    union{
        short modifiers;
        ushort uval2;
    };
    /*starting frame*/
    union{
        ulong start;
        double value_d;
    };
    /*note duration*/
    union{
        uint duration;
        float value_f;
    };
};

class Music{
public:
    Music();
    uint fill(float* buffer, uint frames);
    void jump(ulong frames);
    void seek(ulong frames);
    ulong now();
    void reset();
    bool done();
    NoteInstruction add_note(NoteInstruction i);
    NoteInstruction add_note(char instrument, char note, ulong start, uint duration);
    NoteInstruction add_note_sec(char instrument, char note, double start, uint duration);
    int samplerate;
    unsigned int channels;
    
    void initialize();
    void go();
    void go_safe();
    void stop();
    void write_file(FILE* fptr);
    void write_file_pretty(FILE* fptr);
    void write_prototype_file(FILE* fptr);
    void write_prototype_file_pretty(FILE* fptr);
    
    static Music read_file(FILE* fptr);
    static ulong default_sample(float* out, char instrument, MidiNote n, ulong frames, ulong start = 0);
//private:
    
    ulong (*sample)(float* out, char instrument, MidiNote n, ulong frames, ulong start);
    ulong (*custom_modify)(NoteInstruction* in, float* out, uint filled);
    
    uint follow(NoteInstruction* instruction, float* buffer, uint frames);
    uint frames_left(NoteInstruction* in);
    int modify(NoteInstruction* in, float* out, uint filled);
    static uint frames_taken(NoteInstruction i);
    
    ulong head;
    ulong working_head;
    unsigned int oldest;
    unsigned int on;
    
    long tape_safe;
    List<NoteInstruction> tape_ptp;
    vector<NoteInstruction> tape;
    vector<bool> done_v;
    
    PaStream* stream;
    
};

/*
 * i/o
 */
extern const char* semitones;
extern const char* standard;

void play(Music& m);
void play_forever(Music& m);
void play_interactive(Music& m, const char* keys = semitones);
void stop(Music& m);


ulong music_aiff(string file, Music music, ulong frames);
ulong music_wav(string file, Music music, ulong frames);

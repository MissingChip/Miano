#include <thread>

#include <ncurses.h>
#include <sndfile.h>
#include <portaudio.h>
#include <pa_callback.h>

#include "music.h"

void Music::write_file(FILE* fptr)
{
    initialize();
    for(int i=0;i<tape.size();i++){
        NoteInstruction t = tape[i];
        fprintf(fptr, "%d %d %d %d %lu %u\n", t.type, t.instrument, t.note, t.modifiers, t.start, t.duration);
    }
}
void Music::write_prototype_file(FILE* fptr)
{
    for(int i=0;i<tape.size();i++){
        NoteInstruction t = tape_ptp[i];
        fprintf(fptr, "%d %d %d %d %lu %u\n", t.type, t.instrument, t.note, t.modifiers, t.start, t.duration);
    }
}
void Music::write_file_pretty(FILE* fptr)
{
    initialize();
    for(int i=0;i<tape.size();i++){
        NoteInstruction t = tape[i];
        if(t.type == music::instructions::play){
            fprintf(fptr, "%s at %.1fs for %.1fs\n", MidiNote(t.note).to_string().c_str(), (float)t.start/samplerate, (float)t.duration/samplerate);
        }
    }
}
void Music::write_prototype_file_pretty(FILE* fptr)
{
    for(int i=0;i<tape.size();i++){
        NoteInstruction t = tape_ptp[i];
        if(t.type == music::instructions::play){
            fprintf(fptr, "%s at %.1fs for %.1fs\n", MidiNote(t.note).to_string().c_str(), (float)t.start/samplerate, (float)t.duration/samplerate);
        }
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
    initialize();
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

void play(Music m){
    PaStream* stream;
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream,
        0, /*no input*/
        2, /*2 output channels*/
        paFloat32,
        m.samplerate,
        FPB, /*frames per buffer*/
        paCallback,
        &m
    );
    Pa_StartStream( stream );
    while(!m.done()){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
}

void play_forever(Music m)
{
    m.go();
}
void stop(Music m)
{
    m.stop();
}
const char* top_row = "qwertyuiop[]\\789";
const char* home_row = "asdfghjkl;'456";
const char* bottom_row = "zxcvbnm,./123";
void play_interactive(Music m)
{
    m.go();
    initscr();
    noecho(); raw(); keypad(stdscr, TRUE); nodelay(stdscr, FALSE);// curs_set(FALSE);
    bool done = false;
    string view = "";
    ulong old = 0;
    ulong now = 0;
    while(!done){
        int a = wgetch(stdscr);
        MidiNote n;
        if(a == '`'){done = true; break;};
        now = m.now();
        for(int i=0; i<12; i++){
            if(a == home_row[i]){
                m.add_note(0, 72+i, now, 44100*16);
                n = MidiNote(60+i);
                break;
            }
            else if(a == bottom_row[i]){
                m.add_note(0, 72+i-12, now, 44100*16);
                n = MidiNote(60+i-12);
                break;
            }
            else if(a == top_row[i]){
                m.add_note(0, 72+i+12, now, 44100*16);
                n = MidiNote(60+i+12);
                break;
            }
        }
        if(n.value() > 0){
            if(view.size() == 0){
                old = now;
            }
            view += n.to_string() + " ";
        }
        if(now-44100/20>old && view.size()>0){
            clear();
            mvprintw(10, 10, "%s", view.c_str());
            refresh();
            old = now;
            view.clear();
        }
    } 
    //curs_set(TRUE);
    endwin();
    m.stop();
    //curs_set(TRUE);
}

#ifndef CHUNK
#define CHUNK 1024
#endif

ulong write_aiff(string file, Music music, ulong frames)
{
    //printf("writing to %s\n", file.c_str());
    SF_INFO info;
    info.frames = frames;
    info.samplerate = 44100;
    info.channels = 2;
    info.format = 0x20002;
    info.sections = 1;
    info.seekable = 1;
    SNDFILE* audio_file = sf_open(file.c_str(), SFM_WRITE, &info);
    long done = CHUNK;
    long written = 0;
    float write_b[CHUNK*2];
    while(done == CHUNK && written < frames){
        music.fill(write_b, CHUNK);
        done = sf_writef_float(audio_file, write_b, CHUNK);
        written += done;
    }
    sf_close(audio_file);
    return written;
}

ulong write_wav(string file, Music music, ulong frames)
{
    SF_INFO info;
    info.channels = 2;
    info.frames = frames;
    info.samplerate = 44100;
    info.channels = 2;
    info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    info.sections = 1;
    info.seekable = 1;
    SNDFILE* audio_file = sf_open(file.c_str(), SFM_WRITE, &info);
    long done = CHUNK;
    long written = 0;
    float write_b[CHUNK*2];
    while(done == CHUNK && written < frames){
        music.fill(write_b, CHUNK);
        done = sf_writef_float(audio_file, write_b, CHUNK);
        written += done;
    }
    sf_close(audio_file);
    return written;
}

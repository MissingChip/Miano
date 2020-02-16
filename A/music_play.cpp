
#include <ncurses.h>
#include "music_play.h"


#define FPB 256

static int paCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    Music* m = (Music*)userData;
    float* o = (float*)outputBuffer;
    m->fill(o, framesPerBuffer);
    return 0;
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
    noecho(); raw(); keypad(stdscr, TRUE); nodelay(stdscr, FALSE); curs_set(FALSE);
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
    endwin();
    m.stop();
}

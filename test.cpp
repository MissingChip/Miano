#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <chrono>
#include <thread>

#include "A/music.h"
#include "A/music_play.h"
#include "A/music_write.h"
#include "A/music_generator.h"

using namespace music;
using namespace note_lookup;

int main(int argc, char **argv) {
    srand(100);
    int gen = 400;
    double l = 1.2;
    if(argc>1){
        gen = atoi(argv[1]);
    }
    if(argc>2){
        l = atof(argv[2]);
    }
    printf("Playing %d beats of length %f\n", gen, l);
    Music m = generate(gen, l);
    //music_wav("sleepy2.wav", m, 44100*120); m.reset();
    FILE* wf = fopen("test.mia", "w");
    m.write_file(wf);
    fclose(wf);
    wf = fopen("test.mia", "r");
    m = Music::read_file(wf);
    play(m);
    return 0;
}

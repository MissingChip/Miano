
#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <math.h>

#include "A/music.h"

using namespace music;
using namespace note_lookup;

int main(int argc, char **argv) {
    float* o = new float[2048];
    sample_aiff(o, MidiNote(69), "../AudioSamples/Piano.ff.", 1024, 0);
    
    for(int i=0;i<2048;i+=2){
        printf("%5d", i);
        for(int j=0;j<(o[i]+1.0)*50;j++){
            printf("-");
        }
        printf("+ %.2f\n", o[i]);
    }
}

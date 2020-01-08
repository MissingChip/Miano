#include <sndfile.h>
#include <math.h>
#include <algorithm>
#include "notes.h"

#define NUM_SECONDS 8
#define SAMPLES NUM_SECONDS*44100*2

int fix(string base, string type){
    int i;
    for(i = 21 ; i < 108 ; i++){
        MidiNote n (i);
        string filename = base + type + n.to_string() + ".aiff";
        SF_INFO info;
        SNDFILE* file_r = sf_open(filename.c_str(), SFM_READ, &info);
        SNDFILE* file_w = sf_open((base+"new/"+ type + n.to_string() + ".aiff").c_str(), SFM_WRITE, &info);
        printf("%s\n", filename.c_str());
        float content[SAMPLES];
        sf_read_float(file_r, content, SAMPLES);
        int j;
        for(j=100; j<SAMPLES; j++){
            if(abs(content[j]-content[j-2]) > 0.002){
                j = max(j-(int)0.01*info.samplerate*info.channels, 0);
                break;
            }
        }
        int a = sf_write_float(file_w, content+j, min(SAMPLES/2, SAMPLES-j));
        printf("%d %d %d\n", SAMPLES-j, j, a);
        sf_close(file_r);sf_close(file_w);
    }
}

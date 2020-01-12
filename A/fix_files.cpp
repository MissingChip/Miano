#include <sndfile.h>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include "notes.h"

//samples per read
#define SAMPLES 1024

int fix(string base, string type){
    int i;
    for(i = 21 ; i < 108 ; i++){
        MidiNote n (i);
        string filename = base + type + n.to_string() + ".aiff";
        SF_INFO info;
        SNDFILE* file_r = sf_open(filename.c_str(), SFM_READ, &info);
        
        long int l = info.frames*info.channels;
        float* content = new float[l];
        long int r = 0;
        sf_count_t read = SAMPLES;
        while(read == SAMPLES){
            read = sf_read_float(file_r, content+r, min(SAMPLES, (int)(l-r)));
            r += read;
        }
        sf_close(file_r);
        filename = base+"new/"+ type + n.to_string() + ".aiff";
        SNDFILE* file_w = sf_open(filename.c_str(), SFM_WRITE, &info);
        int j;
        for(j=100; j<l; j+=2){
            if(abs(content[j]) > 0.002){
                j = max(j-(int)(0.003*info.samplerate*info.channels), 0);
                break;
            }
        }
        long int w = 0;
        while(r-j-w>0){
            w += sf_write_float(file_w, content+j+w, min(SAMPLES,(int)(l-j-w)));
        };
        sf_close(file_w);
        
        printf("%s\tstart %d\tend %ld\tread %ld\twrite %ld\twritten %ld\n", filename.c_str(), j, l, r, l-j, w);
        delete[] content;
    }
}

int main(void){
    fix("./Audio/", "Piano.ff.");
    fix("./Audio/", "Piano.mf.");
    fix("./Audio/", "Piano.pp.");
}

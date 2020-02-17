#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <string.h>
#include <chrono>
#include <thread>

#include "A/music.h"

using namespace music;
using namespace note_lookup;

int main(int argc, char **argv) {
    char* input = nullptr;
    int total = 0;
    for(int i=1;i<argc;i++){
        if(string(argv[i]).compare("-i") == 0){
            if(i+1<argc){
                input = argv[i+1];
                total++;
            }
            else{
                fprintf(stderr, "No input found after -i tag\n");
                exit (1);
            }
        }
    }
    if(total == 0){
        Music m;
        play_interactive(m);
    }
    if(input){
        FILE* f = fopen(input, "r");
        if(!f){
                fprintf(stderr, "Error: file %s not found\n", input);
                exit (1);
        }
        Music m = Music::read_file(f);
        m.go();
        
        while(!m.done()){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        m.stop();
    }
}

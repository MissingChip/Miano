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
    char* output = nullptr;
    char* wav = nullptr;
    char* aiff = nullptr;
    int play = 0;
    int compose = 0;
    int human = 0;
    int total = 0;
    const char* keys = semitones;
    for(int i=1;i<argc;i++){
        if(argv[i][0] == '-'){
            if(argv[i][1] == '-'){
                if(strcmp(argv[i], "--standard") == 0){
                    keys = standard;
                }
            }else{
                int j=1;
                while(argv[i][j] != '\0'){
                    char c = argv[i][j];
                    if(c == 'i'){
                        if(i+1<argc){
                            input = argv[i+1];
                        }
                        else{
                            fprintf(stderr, "No input found after -i tag\n");
                            exit (1);
                        }
                        total++;
                    }
                    else if(c == 'o' || c == 'h'){
                        if(i+1<argc){
                            output = argv[i+1];
                        }
                        else{
                            fprintf(stderr, "No output found after -o tag\n");
                            exit (1);
                        }
                        human = (c == 'h');
                        total++;
                    }
                    else if(c == 'p'){
                        play = 1;
                        total++;
                    }
                    else if(c == 'c'){
                        compose = 1;
                        total++;
                    }
                    else if(c == 'w'){
                        if(i+1<argc){
                            wav = argv[i+1];
                        }
                        else{
                            fprintf(stderr, "No path found after -w tag\n");
                            exit (1);
                        }
                        total++;
                    }
                    else if(c == 'a'){
                        if(i+1<argc){
                            aiff = argv[i+1];
                        }
                        else{
                            fprintf(stderr, "No path found after -a tag\n");
                            exit (1);
                        }
                        total++;
                    }
                    j++;
                }
            }
        }
    }
    Music m;
    if(input){
        FILE* f = fopen(input, "r");
        if(!f){
                fprintf(stderr, "Error: file %s not found\n", input);
                exit (1);
        }
        m = Music::read_file(f);
        if(play){
            m.go();
            
            while(!m.done()){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            m.stop();
        }
    }
    if(compose || total == 0){
        m.reset();
        play_interactive(m, keys);
    }
    if(wav){
        music_wav(string(wav), m, 0xffffffff);
    }
    if(aiff){
        music_aiff(string(aiff), m, 0xffffffff);
    }
    if(output){
        FILE* f = fopen(output, "w");
        if(!f){
                fprintf(stderr, "Error: file %s not found\n", input);
                exit (1);
        }
        if(human){
            m.write_file_pretty(f);
        }else{
            m.reset();
            m.write_file(f);
        }
    }else if(human){
        m.write_file_pretty(stdout);
    }else if(total == 1){
         m.write_file_pretty(stdout);
    }
}

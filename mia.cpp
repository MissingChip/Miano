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
    int play = 0;
    int human = 0;
    int total = 0;
    for(int i=1;i<argc;i++){
        if(argv[i][0] == '-'){
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
                }
                else if(c == 'p'){
                    play = 1;
                }
                j++;
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
    else if(total == 0 || output){
        play_interactive(m);
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
            m.write_file(f);
        }
    }else if(input && !play){
         m.write_file_pretty(stdout);
    }
}

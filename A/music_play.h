#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <chrono>
#include <thread>

#include "music.h"


void play(Music m);
void play_forever(Music m);
void play_interactive(Music m);
void stop(Music m);

static int paCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData );

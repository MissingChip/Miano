
#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <math.h>

#include "A/music.h"
#include "music_generator.h"

using namespace music;
using namespace note_lookup;

int main(int argc, char **argv) {
    srand(1000);
    Music m = generate(400, 1.5);
    play(m);
}

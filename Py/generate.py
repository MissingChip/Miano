#!/usr/bin/python3

from music import *

def generate(beats = 20, beat_length = 0.8):
    m = Music()
    m.beat_length = beat_length;
    wait = 0
    for i in range(0, beats):
        if i % 2 == 0 and wait < 3:
            m.beat(note=70, start = i, duration=41000*20)
    return m;

if __name__ == '__main__':
    music = generate()
    f = open("generated.mia", "w")
    f.write(str(music))
    f.close()

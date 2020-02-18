directory = $(sort $(dir $(wildcard ./*/.)))
objs = $(wildcard ./obj/*.o)

CC = g++ -I ./lib -L ./lib
libs =  -lportaudio -lsndfile -lpthread -lasound -lncurses

mia: modules internal mia.cpp
	$(CC) -o mia mia.cpp $(objs) $(libs)

mia_test: modules internal test.cpp
	$(CC) -o mia_test test.cpp $(objs) $(libs) music_generator.cpp

modules:
	git submodule update --init --remote --recursive
	make -C ./lib

.PHONY: modules

internal:
	make -C ./A --no-builtin-rules

.PHONY: internal

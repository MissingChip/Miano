
list_req = A/list.hpp A/list.h
main_f = main.cpp A/music.cpp A/notes.cpp A/samples.cpp

play: $(main_f) $(list_req)
	g++ -o play $(main_f) -lportaudio -lsndfile

list_test: A/list_test.cpp $(list_req)
	g++ -o list_test A/list_test.cpp

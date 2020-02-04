
music_io = A/music_play.cpp A/music_write.cpp A/music_generator.cpp
list_req = A/list.hpp A/list.h
main_d = A/music.cpp A/notes.cpp A/samples.cpp
main_f = $(main_d) $(music_io)

test: test.cpp $(main_f) $(list_req)
	g++ -o play_test test.cpp $(main_f) -lportaudio -lsndfile

list_test: A/list_test.cpp $(list_req)
	g++ -o list_test A/list_test.cpp

file_info: A/fix_files.cpp $(main_f)
	g++ -o file_info A/fix_files.cpp $(main_d) -lportaudio -lsndfile

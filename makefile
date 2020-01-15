main_files = main.cpp A/music.cpp A/notes.cpp A/samples.cpp
playpiano: $(main_files)
	g++ -o play $(main_files) -lportaudio -lsndfile

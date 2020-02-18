# MIA

A tool for creating \*beautiful\* music. Or terrible music, if that's how you roll.<br>
Right now the executable (tested only on a linux install with [portaudio](http://www.portaudio.com/) installed)

## Build
Running `make` should produce an executable 'mia'

## Running
Run mia with options `o`, `i`, `h`, `p`, `c`, `a`, `w`
```bash
# c -- compose music
#   Enters a terminal window and allows playing in a very basic capacity on keyboard (extended to numpad)
mia -c

# o -- output music info to "filename.mia"
mia -o filename.mia

# i -- input music from "filename.mia"
mia -i filename.mia

# p -- play music from input file (otherwise does nothing
mia -p -i filename.mia

# h -- output music in a human readable format to file specified by -o (or stdout if no file is specified)
mia -i filename.mia -o filename.txt -h
#   -- or -- (output to stdout)
mia -i filename.mia -h

# a -- output filename.mia to .aiff
mia -i filename.mia -a filename.aiff

# w -- output filename.mia to .wav
mia -i filename.mia -w filename.wav
```
### Combining options
Options can also be combined. For example:
```bash
mia -oc filename.mia
```
enters composition mode, then saves the work to a file<br><br>
```bash
mia -ip filename.mia
```
plays the music in filename.mia<br><br>
```bash
mia -iocp filename.mia
```
plays the music in filem=name.mia, then enters composition mode (while the music plays from filename.mia) and saves the combined track back to filename.mia

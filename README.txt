This is a program designed to take a BIN CUE and split it into separate BINs for each track. Very incomplete
I made this for my PSP, since I wanted an easier way to bulk name a bunch of songs.
I've added basic Musicbrainz DiscID support, although the base64 library I'm using is very weird and broken sometimes.
Idk what exactly is even causing it do that, since when I run it with valgrind it works every time.

How to use:
To use the program invoke it with -b 'path/to/bin' and -c 'path/to/cue'. This will create a TOC and also create a DiscID for MusicBrainz
If you want to split the tracks, you can invoke it with -o 'Output name', and it'll split each track with your input as a guide for the names.

Here are some commands you can add to the file name to individually customize the name:
	"@@" - Puts the @ symbol (@ is the command character so just putting it in by itself will break things)
	"@n" - Puts the Track Number
	"@0n" - Put the Track number with a 0 infront of single digit tracks
	"@m" - Puts the amount of minutes on a track
	"@s" - Puts the amount of seconds on a track

So inputing something like: 
	"@0n - Sgt. Pepper's @m:@s" 
will produce something like:
	"01 - Sgt. Pepper's 2:50"
Currently the MusicBrainz portion doesn't work with CD-ROMS but I want to fix that at somepoint


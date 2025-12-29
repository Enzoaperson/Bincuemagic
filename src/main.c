#include <stdio.h>
#include <sys/types.h>
#include <sha1.h>

#include "datatypes.h"
#include "constants.h"
#include "cuehandler.h"
#include "songextract.h"
#include "filenamebuilder.h"
#include "discid_gen.h"

// Global flags
char DEBUG = 0;
char MUSICBRAINZ_LOOKUP = 1;
char EXPORT_TO_WAVE = 0;

int file_status(char file[]){
	FILE* file_storage;
	file_storage = fopen(file, "rb");
	//File Doesn't exist
	if(file_storage == NULL){
		return DNE;
	}
	unsigned char file_header[2];
	fread(file_header, 1, sizeof(file_header), file_storage);
	if(DEBUG){printf("Header of %s: %c%c\n",file, file_header[0], file_header[1]);}
	//File is a .CUE
	if(file_header[0] == 'F' && file_header[1] == 'I'){
		return CUE;
	}
	printf(".CUE FAILED\n");
	//File is a .TOC
	if(file_header[0] = 'C' && file_header[0] == 'D'){
		return TOC;
	}
	printf(".TOC FAILED\n");
	fclose(file_storage);
	//File is a .BIN (or can't tell what it is)
	return BIN;
}

int main(int argc, char *argv[]){
	char *toc_file = NULL;
	char *bin_file = NULL;
	char *output_name = NULL;
	if (argc < 2){
		printf("$ a.out -c path/to/cuefile.cue -b path/to/binfile.bin -o \"path/of/output/@n - Track Name @m:@s.wav\" -w \n");
		printf("-w -> Output as a WAV file\n");
		printf("-d -> Debug mode\n");
		return 0;
	}

	for(int i = 1; i < argc; i++){
		if(DEBUG){printf("arg%d - %s\n", i, argv[i]);}
		if(argv[i][0] == '-' && i != argc){
			switch (argv[i][1]){
				case 'c': case 'C':
					toc_file = argv[i+1];
					break;
				case 'b': case 'B':
					bin_file = argv[i+1];
					break;
				case 'o': case 'O':
					output_name = argv[i+1];
					break;
				case 'd': case 'D':
					DEBUG = 1;
					i--;
					break;
				case 'w': case 'W': //Add .wav header to file outputs
					EXPORT_TO_WAVE = 1;
					i--;
					break;


			}
			i++;

		}
	
	}
	if(toc_file == NULL){
		printf("Error: No TOC File of any kind was given!\n");
		return 0;
	}
	if(bin_file == NULL){
		printf("Error: No BIN File was given!\n");
		return 0;
	}
	
	//Create TOC
	tableofcontents toc = cuehandler(toc_file);
	FILE* opened_bin_file;
	opened_bin_file = fopen(bin_file, "rb");
	fseek(opened_bin_file, 0, SEEK_END);
	toc.leadout_offset = (ftell(opened_bin_file)/BYTES_PER_SECTORS);
	fclose(opened_bin_file);
	printf("Number of tracks: %d\n\n", toc.number_of_tracks);
	for (int i = 0; i < toc.number_of_tracks; i++){
		printf("Offset %d: %d\n", i, toc.offset[i]+CD_LEADIN_OFFSET);
	}
	printf("Leadout Offset: %d\n", toc.leadout_offset+CD_LEADIN_OFFSET);
	printf("DiscID in coming:\n");
	discid_gen(toc);
	if(output_name == NULL){
		printf("No output name given. Quitting now!\n");
		return 0;
	}
	//Split Tracks
	int starting_track = 0;
	int ending_track = toc.number_of_tracks;
	for (int track = starting_track; track < ending_track; track++){
		char name[PATH_SIZE+FILE_NAME_SIZE];
		filenamebuilder(output_name, name, toc, track);
		printf("Outputting track:%d to: \"%s\"\n",track, name);
		if (track >= toc.number_of_tracks -1){
			songextract(bin_file, toc.offset[track]*BYTES_PER_SECTORS, (toc.leadout_offset-toc.offset[track])*BYTES_PER_SECTORS, name);
		}else{
			songextract(bin_file, toc.offset[track]*BYTES_PER_SECTORS, (toc.offset[track+1]-toc.offset[track])*BYTES_PER_SECTORS, name);
		}
	}
	printf("Splitting Complete!\n");
	return 0;
}

#include <stdio.h>
#include "constants.h"

extern char DEBUG;
extern char EXPORT_TO_WAVE;

char wave_header[] = {0x52, 0x49, 0x46, 0x46, 0x44, 0x9D, 0x4E, 0x03, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 
	0x00, 0x01, 0x00, 0x02, 0x00, 0x44, 0xAC, 0x00, 0x00, 0x10, 0xB1, 0x02, 0x00, 0x04, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74,
        0x61, 0x20, 0x9D, 0x4E, 0x03};

int songextract(char *bin_file_name, int starting_bytes, int length_bytes, char *output_file_name){
	if(DEBUG){printf("Starting Input: %d,%d\n", starting_bytes, length_bytes);}
	unsigned char song_storage;
	FILE* bin_file;
	FILE* output_file;
	output_file = fopen(output_file_name, "wb");
	bin_file = fopen(bin_file_name, "rb");

	if(bin_file == NULL){ //Sanity Check
		printf("Error: Somehow the .BIN file became NULL during the splitting process\n");
		return ERROR;
	}
	if(output_file == NULL){ //Permissions checks
		printf("Error: I can't write the file: %s\n Check if you have permissions to write here\n");
		return ERROR;
	}
	if(length_bytes == CD_END_OF_BIN){ //Get length_bytes offset if last track
		fseek(bin_file, 0, SEEK_END);
		length_bytes = ftell(bin_file)-starting_bytes;
	}
	if(DEBUG){printf("Fixed Input: %d,%d\n", starting_bytes, length_bytes);}
	fseek(bin_file, starting_bytes, SEEK_SET); //Getting into position
	if(EXPORT_TO_WAVE){
		if(DEBUG){printf("Adding WAV header\n");}
		for(int i = 0; i < 0x2C; i++){
			fputc(wave_header[i], output_file);
		}
	}
	for(int i = 0; i < length_bytes; i++){
		fputc(fgetc(bin_file), output_file);
	}
	fclose(bin_file);
	fclose(output_file);

	return 0;
}

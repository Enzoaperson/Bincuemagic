#include <stdio.h>
#include "constants.h"

extern char DEBUG;

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

	for(int i = 0; i < length_bytes; i++){
		fputc(fgetc(bin_file), output_file);
	}
	fclose(bin_file);
	fclose(output_file);

	return 0;
}

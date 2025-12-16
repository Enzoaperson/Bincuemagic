#include <stdio.h>
#include "datatypes.h"
#include "constants.h"
extern char DEBUG;

tableofcontents cuehandler(char *toc_file_name){
	tableofcontents built_toc;
	//Open toc file
	FILE* toc_file;
	toc_file = fopen(toc_file_name, "rb");
	if(toc_file == NULL){
		printf("Error! Somehow the Toc file has become invalid during the Table of Contents building! \n");
		built_toc.number_of_tracks = ERROR;
		return built_toc;
	}
	//Get len of file
	fseek(toc_file, 0, SEEK_END);
	int length_of_file = ftell(toc_file);
	fseek(toc_file, 0, SEEK_SET);
	//Load file into memory
	unsigned char toc_table[length_of_file];
	fread(toc_table, 1, length_of_file, toc_file);
	int time_storage;
	int track_number = 0;
	for(int i = 0; i < length_of_file; i++){
		time_storage = 0;
		if(toc_table[i] == 'I' && toc_table[i+7] == '1'){
			if (track_number > 99){
				printf("Error! Somehow too many tracks!\n");
				built_toc.number_of_tracks = ERROR;
				return built_toc;
			}
			track_number++;
		//Minute Conversions
			time_storage += (toc_table[i+7+2] - '0') * 10;
			time_storage += (toc_table[i+7+3] - '0');
			time_storage *= SECONDS_IN_MINUTE;
			if(DEBUG){printf("Time:%c%c:", toc_table[i+7+2], toc_table[i+7+3]);}
		//Second Conversions
			time_storage += (toc_table[i+7+5] - '0') * 10;
			time_storage += (toc_table[i+7+6] - '0');
			time_storage *= SECTORS_PER_SECOND;
			if(DEBUG){printf("%c%c:", toc_table[i+7+5], toc_table[i+7+6]);}
		//1/75 Conversions
			time_storage += (((toc_table[i+7+8] - '0') * 10) + (toc_table[i+7+9] - '0'));
			if(DEBUG){printf("%c%c \n", toc_table[i+7+2], toc_table[i+7+3]);}
			if(DEBUG){printf("Offset %d: %d\n\n",track_number, time_storage);}
			built_toc.offset[track_number-1] = time_storage;
		}
	}
	built_toc.number_of_tracks = track_number;
	for(int i = track_number+1; i < 100; i++){
		built_toc.offset[i] = 0;
	}

	fclose(toc_file);
	return built_toc;
}


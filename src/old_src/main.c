#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CD_SAMPLE_RATE 176400/100 //Bytes per 0.01 seconds
#define MAX_FILE_NAME_LENGTH 1000
#define MAX_TRACKS 99

enum toc_status {FIRST_FILE, SECOND_FILE, BOTH_FILE, NEITHER_FILE, F1_DNE, F2_DNE, F1F2_DNE};
/*
 How 2 Use:

 ./a.out /path/to/toc /path/to/bin "@n - Constant Text" -x --extra_cmds
*/

int toc_checker(char argv1[], char argv2[]){
	//Checks both files to find which one is the valid and are TOC files
	FILE* File1;
	FILE* File2;
	File1 = fopen(argv1, "rb");
	File2 = fopen(argv2, "rb");

	//Valid File checks
	if(File1 == NULL && File2 == NULL){
		return F1F2_DNE;
	}
	if(File1 == NULL){
		return F1_DNE;
	}
	if(File2 == NULL){
		return F2_DNE;
	}
	//Open Headers
	unsigned char File1_header[2];
	unsigned char File2_header[2];
	fread(File1_header, 1, sizeof(File1_header), File1);
	fread(File2_header, 1, sizeof(File2_header), File2);
	//File1 is TOC
	if(File1_header[0] == 0x43 && File1_header[1] == 0x44){
		fclose(File1);
		fclose(File2);
		return FIRST_FILE;
	}
	//File2 is TOC
	if(File2_header[0] == 0x43 && File2_header[1] == 0x44){
		fclose(File1);
		fclose(File2);
		return SECOND_FILE;
	}
	//Neither are TOCs
	return NEITHER_FILE;
}

int toc_parser(char *toc_file_name, unsigned int numbers[]){
	// 0x0a is new line
	unsigned char c = 0;
	int quotes_flag = 0;
	int numb_select = 0;
	int tracks = 0;
	FILE* toc_file;
	toc_file = fopen(toc_file_name, "rb");
	fseek(toc_file, 0, SEEK_END);
	int length_of_file = ftell(toc_file);
	fseek(toc_file, 0, SEEK_SET);
	unsigned char toc_table[length_of_file];
	fread(toc_table, 1, length_of_file, toc_file);
	for(int x = 0; x < length_of_file; x++){
		if(toc_table[x] == 'F' && toc_table[x+1] == 'I' && toc_table[x+2] == 'L' && toc_table[x+3] == 'E'){
			tracks++;
			quotes_flag = 0;
			for(int y = 0; toc_table[x+y] != '\n'; y++){
				//printf("%c", toc_table[x+y]);
				if(quotes_flag != 1){
					if(toc_table[x+y] == '"'){
						quotes_flag = 1;
					}else{
						if(toc_table[x+y+2] == 0x20 && toc_table[x+y] == 0x20 && toc_table[x+y+1] == '0'){
							numbers[numb_select] = 0;
							numbers[numb_select+1] = 0;
							numbers[numb_select+2] = 0;
							numb_select+=3;
						}
						if(toc_table[x+y] >= '0' && toc_table[x+y+1] >= '0' && toc_table[x+y] <= '9' && toc_table[x+y+1] <= '9'){
						numbers[numb_select] = (((toc_table[x+y])-'0')*10) + ((toc_table[x+y+1]-'0'));
						numb_select++;
						}
					}
				}else
					{
					if(toc_table[x+y] == '"'){
						quotes_flag = 0;
					}
				}

			}
		}
	}
	printf("%d Tracks\n", tracks);
	for(int x = 0; x < numb_select; x+=3){
		printf("%d:%d:%d\n", numbers[x],numbers[x+1],numbers[x+2]);
	}
	fclose(toc_file);
	return tracks;
}


int song_extract(char *bin_file_name, int starting_time, int length_time, char *output_file_name){
	int length_bytes = (length_time * CD_SAMPLE_RATE);
	int starting_offset = (starting_time * CD_SAMPLE_RATE);
	printf("%d %d \n", length_bytes, starting_offset);
	unsigned char song_storage;

	FILE* bin_file;
	FILE* output_file;
	output_file = fopen(output_file_name, "wb+");
	bin_file = fopen(bin_file_name, "rb");
	fseek(bin_file, starting_offset, SEEK_SET);

	for(int y = 0; y < length_bytes; y++){
		fputc(fgetc(bin_file), output_file);
	}
	fclose(bin_file);
	fclose(output_file);

	return 0;
}

int string_name_builder(char argv[], char name[], int track_num, int length_time){
	int x_argv = 0;
	int x_string = 0;
	while(argv[x_argv] != '\0'){
		if(argv[x_argv] == '@'){
			if(argv[x_argv+1] == '@'){ // @@ -> @
				name[x_string] = '@';
				x_argv++;
			}
			if(argv[x_argv+1] == 'n'){ // @n -> Track Number
				if(track_num < 10){
					name[x_string] = '0' + track_num;
					x_argv++;
				}
				else{
					name[x_string] = '0' + (track_num/10);
					name[x_string+1] = '0' + track_num - (track_num/10)*10;
					x_argv++;
					x_string++;
				}
			}
			if(argv[x_argv+1] == 'm'){ // @m -> How many minutes are in track
				int minute = length_time/6000;
				if (minute <= 9){
				name[x_string] = '0' + minute;
				x_argv++;
				}else{
				name[x_string] = '0' + (minute/10);
				name[x_string+1] = '0'+ minute - (minute/10)*10;
				x_argv++;
				x_string++;
				}
			}
			if(argv[x_argv+1] == 's'){
				int seconds = (length_time - (length_time/6000)*6000)/100;
				name[x_string] = '0' + (seconds/10);
				name[x_string+1] = '0' + seconds - ((seconds/10)*10);
				x_argv++;
				x_string++;
			}
			if(argv[x_argv+1] == '0' && argv[x_argv+2] == 'n'){ // @0n - > Track Num with 0 added to single digits
				name[x_string] = '0' + (track_num/10);
				name[x_string+1] = '0' + track_num - (track_num/10)*10;
				x_argv+=2;
				x_string++;
			}
		}else{
			name[x_string] = argv[x_argv];
		}
		x_argv++;
		x_string++;
	}
	name[x_string] = '\0';
	return 0;
}
int toc_builder(int *times, int *offsets){
	for(int x = 0; x < sizeof(times); x++){
		offsets[x] = ((times[x*6] * 6000) + (times[(x*6)+1] * 100) + ((times[(x*6)+2])));
		offsets[x] *= CD_SAMPLE_RATE;
	
	}
	for(int x = 0; x < sizeof(offsets); x++){
		printf("%d\n", offsets[x]);
	}
	
	return 0;
}


int cdhashlowcalc(int *times){

	return 0;
}


int main(int argc, char *argv[]){
	if(argc <= 1){
		printf("No arguments were given!\nType --Help for help\n");
		return 1;
	}
	if(argc < 4){
		printf("Not enought arguments were given!\nType --Help for help\n");
		return 1;
	}
	int valid_toc = toc_checker(argv[1], argv[2]);
	//printf("%x\n", valid_toc);

	if(valid_toc == F1F2_DNE){
		printf("Error! Neither file exists\n");
		return 0;
	}
	if(valid_toc >= BOTH_FILE){
		printf("Error! No valid TOC file has been found! \n");
		return 0;
	}
	int tracks = 0;
	unsigned int times[MAX_TRACKS*6];
	if(valid_toc == FIRST_FILE){
		tracks = toc_parser(argv[1], times);
	}else{
		tracks = toc_parser(argv[2], times);
	}
	int offsets[MAX_TRACKS];
	toc_builder(times, offsets);
	int length_time = 0;
	int start_time = 0;

	for(unsigned int x = 0; x < tracks; x++){
		start_time = ((times[x*6] * 6000) + (times[(x*6)+1] * 100) + (times[(x*6)+2]));
		length_time = (times[(x*6)+3] * 6000) + (times[(x*6)+4] * 100) + times[(x*6)+5];
		char file_name[MAX_FILE_NAME_LENGTH];
		string_name_builder(argv[3], file_name, (x)+1, length_time);
		printf("\nName: %s\nX = %d\nStarting = %d\nlength = %d\n", file_name, x, start_time, length_time);
		if(valid_toc == 0){
			song_extract(argv[2], start_time, length_time, file_name);
		}else{
			song_extract(argv[1], start_time, length_time, file_name);
		}
	}
	return 0;
}



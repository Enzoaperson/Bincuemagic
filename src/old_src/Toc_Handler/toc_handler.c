#include <stdio.h>

int toc_parser(char *toc_file_name, unsigned int starting_times[]){
	printf("File: %s\n", toc_file_name);
//Check if file exists
	FILE* toc_file;
	toc_file = fopen(toc_file_name, "rb");
	if(toc_file == NULL){
		printf("ERROR! FILE DOES NOT EXIST\n");
		return 1;
	}
	fseek(toc_file, 0, SEEK_END);
	int length_of_file = ftell(toc_file);
	printf("Length of File: %d Bytes\n", length_of_file);
	unsigned char toc_file_open[length_of_file];
	fseek(toc_file, 0, SEEK_SET);
	fread(toc_file_open, 1, length_of_file, toc_file);
	char track_number = 0;
	char quotes_flag = 0;
	for(int x = 0; x < length_of_file; x++){
		if(toc_file_open[x] == 'F' && toc_file_open[x+1] == 'I' && toc_file_open[x+2] == 'L' && toc_file_open[x+3] == 'E'){
		quotes_flag = 0;
		track_number++;
		for(int y = 0; toc_file_open[x+y] != '\n' || quotes_flag == 2; y++){
			if(toc_file_open[x+y] == '"'){
				quotes_flag ^= 1;
			}
			if(quotes_flag == 0){
				if(toc_file_open[x+y] == ' ' && toc_file_open[x+y+1] == '0' && toc_file_open[x+y+2] == ' '){
					quotes_flag == 1;
				}
				if(toc_file_open[x+y] >= '0' && toc_file_open[x+y] <= '9' && toc_file_open[x+y+1] >= '0' && toc_file_open[x+y+1] <= '9'){
					
					printf("%c%c ", toc_file_open[x+y], toc_file_open[x+y+1]);
				}
			}
		}
		printf("\n");
			
	}
	}
	






	return 0;	
}

int main(int argc, char *argv[]){
	if(argc <= 1){
		printf("No arguments were given!\n");
		return 1;	
	}
	unsigned int starting_times[100];
	toc_parser(argv[1], starting_times);
	return 0;
}

#include <stdio.h>
#include "datatypes.h"

extern char DEBUG; 

/*
 * TODO:
 *  *Split appart the path and actual file name
 *  *Then impose the 255 general Unix file limit
 *  	*Path can be 4096 characters
 *  	*Name can be 255 characters
*/

int filenamebuilder(char input_name[], char output_name[], tableofcontents toc, int track_num){
	int x_argv = 0;
	int x_string = 0;
	int start_of_name = 0;

	for(int i = 0; input_name[i] != '\0'; i++){ //path ignorer
		output_name[i] = input_name[i];
		if(input_name[i] == '/'){
			if(DEBUG){printf("Subdirectory found at character %d\n", i);}
			x_argv = i;
			x_string = i;
		}
	};

	while(input_name[x_argv] != '\0'){
		if(input_name[x_argv] == '@'){
			if(input_name[x_argv+1] == '@'){ // @@ -> @
				output_name[x_string] = '@';
				x_argv++;
			}
			if(input_name[x_argv+1] == 'n'){ // @n -> Track Number
				if(track_num <= 8){
					output_name[x_string] = '0' + (track_num+1);
					x_argv++;
				}
				else{
					output_name[x_string] = '0' + ((track_num+1)/10);
					output_name[x_string+1] = '0' + (track_num+1) - ((track_num+1)/10)*10;
					x_argv++;
					x_string++;
				}
			}
			if(input_name[x_argv+1] == 'm'){ // @m -> How many minutes are in track
				int minute = 0;
				if(DEBUG){printf("tracknum: %d/%d\n", track_num, toc.number_of_tracks);}
				if (track_num == toc.number_of_tracks-1){
					minute = ((toc.leadout_offset - toc.offset[track_num])/75)/60;
				}else{
					minute = (((toc.offset[track_num+1]-toc.offset[track_num])/75))/60;
				}
				if (minute <= 9){
				output_name[x_string] = '0' + minute;
				x_argv++;
				}else{
				output_name[x_string] = '0' + (minute/10);
				output_name[x_string+1] = '0'+ minute - (minute/10)*10;
				x_argv++;
				x_string++;
				}
			}
			if(input_name[x_argv+1] == 's'){
				int seconds;
				if (track_num == toc.number_of_tracks-1){
					seconds = ((toc.leadout_offset-toc.offset[track_num])/75);
					seconds -= (((toc.leadout_offset-toc.offset[track_num])/75)/60)*60;
				}else{
					seconds = ((toc.leadout_offset-toc.offset[track_num])/75);
					seconds -= (((toc.leadout_offset-toc.offset[track_num])/75)/60)*60;
				}
				output_name[x_string] = '0' + (seconds/10);
				output_name[x_string+1] = '0' + seconds - ((seconds/10)*10);
				x_argv++;
				x_string++;
			}
			/*if(input_name[x_argv+1] == 'u'){
				int micro_seconds = ((toc.offset[track_num+1]-toc.offset[track_num])/75);
				printf("%d\n\n", micro_seconds);
				micro_seconds -= (toc.offset[track_num+1]-toc.offset[track_num]);
				printf("%d\n\n", micro_seconds);
				output_name[x_string] = '0' + (micro_seconds/10);
				output_name[x_string+1] = '0' + micro_seconds - ((micro_seconds/10)*10);
			}*/
			if(input_name[x_argv+1] == '0' && input_name[x_argv+2] == 'n'){ // @0n - > Track Num with 0 added to single digits
				output_name[x_string] = '0' + ((track_num+1)/10);
				output_name[x_string+1] = '0' + (track_num+1) - ((track_num+1)/10)*10;
				x_argv+=2;
				x_string++;
			}
			/*if(input_name[x_argv+1] == 'i' && input_name[x_argv+2] == 'd'){
				for(int i = 0; i < BASE64_OUTPUT_SIZE; i++){
					printf("%x", toc.discid[0]);
					output_name[x_string] = toc.discid[i];
					x_string++;
				}
				x_argv+=2;
			}*/
		}else{
			output_name[x_string] = input_name[x_argv];
		}
		x_argv++;
		x_string++;
	}
	output_name[x_string] = '\0';
	return 0;
}

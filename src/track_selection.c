#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//example argument -e 1,2,3:5

char *track_selection(char *argument){
	int argu_length = strlen(argument);
	char *tracks =  (char *)malloc(99);
	char t_index = 1;
	unsigned short temp_track_num = 0;
	for(int i = 0; i <= argu_length; i++){
		if((argument[i] == ',' || i == argu_length) && (temp_track_num)){
			tracks[t_index] = temp_track_num;
			t_index++;
			temp_track_num = 0;
		}
		if(argument[i] == ':' && temp_track_num){
			unsigned short ending_num = 0;
			int n = i+1;
			while((argument[n] >= '0' && argument[n] <= '9')){
				ending_num *= 10;
				ending_num += (argument[n] - '0');
					if(ending_num > 99){
						printf("Error! Track number: %d is invalid\n", ending_num);
						return NULL;
					}
				n++;
				}
			i = n;
			printf("Got spanner number: %d\n", ending_num);
			for(int numb = temp_track_num; numb <= ending_num; numb++){
				tracks[t_index] = numb;
				t_index++;
			}
			temp_track_num = 0;
		}
		if(argument[i] >= '0' && argument[i] <= '9'){
			temp_track_num *= 10;
			temp_track_num += (argument[i] - '0');
			if(temp_track_num > 99){
				printf("Error! Track number: %d is invalid\n", temp_track_num);
				return NULL;
			}
		}
	}
	//printf("%d\n", temp_track_num);
	tracks[0] = t_index;
	//printf("Length = %d\n", tracks[0]);
	//for(int i = 1; i < t_index; i++){
	//	printf("%d: %d\n", i, tracks[i]);
	//}
	return tracks;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENABLE 1
#define DISABLE 0

//example argument: -e 1,2,4:8
//Should preduce tracks 1 2 4 5 6 7 8 

char *track_selection(char *argument){
	printf("argument: %s\n", argument);
	int argu_length = strlen(argument);
	//Bits are set to 0 by default and setting it anything else marks it for extraction
	char *tracks =  (char *)calloc(99, sizeof(char)); 
	char temp_num = 0;
	for(int i = 0; i < argu_length; i++){
		//Is character a number?
		if(argument[i] >= '0' && argument[i] <= '9'){
			temp_num *= 10;
			temp_num += (argument[i] - '0');
			//Check if the temp number is greater than 99 (Max Number of Tracks)
			if(temp_num > 99){
				printf("Error! Track %d is invalid\n", temp_num);
				return NULL;
			}
		} else if(argument[i] == ',' || argument[i] == ' '){ //Is the character a comma or space?
			//Make Sure number is greater than 0
			if(!temp_num){
				printf("Error! Track 0 doesn't exist, or a number hasn't been specified\n");
				free(tracks);
				return NULL;
			}
			//Check if it was already enabled?
			if(tracks[temp_num-1] == ENABLE){
				printf("Error! Track %d was already eanbled. Recheck your CMD arguments!\n", temp_num);
				free(tracks);
				return NULL;
			}
			tracks[temp_num-1] = ENABLE;
			temp_num = 0;
		} else if(argument[i] == ':'){ //Is the character a em-dash?
			//Find the next number
			int next_num = 0;
			while(i < argu_length){
				//printf("building");
				i++;
				if(argument[i] >= '0' && argument[i] <= '9'){
					next_num *= 10;
					next_num += (argument[i] - '0');
					if(next_num > 99){
						printf("Error! Track %d is invalid in span\n", next_num);
						free(tracks);
						return NULL;
					}
				}
				else{
					break;
				}
			}
			if(next_num <= temp_num){
				printf("Error! %d cannot span to %d\n", temp_num, next_num);
				free(tracks);
				return NULL;
			}
			printf("BREAK\n");
			//Enable all the tracks
			for(char i2 = (temp_num-1); i2 < (next_num); i2++){
				tracks[i2] = ENABLE;
			}
			//Clear temp num
			temp_num = 0;
			
		}else{
			printf("Error! I don't know what %c means\n", argument[i]);
			free(tracks);
			return NULL;
		}
	}
	//Add the final number
	tracks[temp_num-1] = ENABLE;
	temp_num = 0;
	#ifdef DEBUG_ON
	printf("                     1\n   1 2 3 4 5 6 7 8 9 0");
	for(char col = 0; col <= 9; col++){
		printf("\n%d  ", col);
		for(char row = 0; row < 10; row++){
			printf("%d ", tracks[(col*10)+row]);
		}
	}
	printf("\n");
	#endif
	return tracks;
}

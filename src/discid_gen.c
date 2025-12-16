#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sha1.h>
#include <b64/cencode.h>
#include "datatypes.h"
#include "constants.h"
extern char DEBUG;

char discid_gen(tableofcontents toc){
//--- Calculates the MusicBrain DiscID and adds it to the TableOfContents struct
//Sha1 hashing
	SHA1_CTX sha;
	SHA1Init(&sha);
	unsigned char *sha1_output = (unsigned char*)malloc(SHA1_DIGEST_LENGTH);
	unsigned char buffer[20];
	int length;
	length = sprintf(buffer, "%02X", 1);                                     //First track number
	SHA1Update(&sha, (unsigned char*)buffer, length);
	length = sprintf(buffer, "%02X", toc.number_of_tracks);                  //Last track number
	SHA1Update(&sha, (unsigned char*)buffer, length);
	length = sprintf(buffer, "%08X", toc.leadout_offset + CD_LEADIN_OFFSET); //Lead Out track offset
	SHA1Update(&sha, (unsigned char*)buffer, length);
	for(int i = 0; i < toc.number_of_tracks; i++){                           //The used tracks
		length = sprintf(buffer, "%08X", toc.offset[i] + CD_LEADIN_OFFSET);
		SHA1Update(&sha, (unsigned char*)buffer, length);

	}
	for(int i = 0; i < (MAX_AMOUNT_TRACKS - toc.number_of_tracks); i++){     //Unused tracks
		length = sprintf(buffer, "%08X", 0);
		SHA1Update(&sha, (unsigned char*)buffer, length);
	}
	SHA1Final(sha1_output, &sha);
	if(DEBUG){
		for(int i = 0; i < SHA1_DIGEST_LENGTH; i++){ //Print SHA1
			printf("%x", sha1_output[i]);
		}
		printf("\n");
	}

//Base64 Encoding
	base64_encodestate s;
	unsigned char *base64_output = (unsigned char*)malloc(BASE64_OUTPUT_SIZE);
	unsigned char *base64_counter = base64_output;
	char base64_end = 0;
	base64_end = base64_encode_block(sha1_output, SHA1_DIGEST_LENGTH, base64_counter, &s);
	base64_counter += base64_end;
	base64_end = base64_encode_blockend(base64_counter, &s);
	base64_counter += base64_end;
	*base64_counter = 0;
	printf("https://https://musicbrainz.org/cdtoc/");
	for(int i = 0; i < BASE64_OUTPUT_SIZE; i++){ //This puts it in the proper MusicBrains format
		switch (base64_output[i]){
			case '+':
				base64_output[i] = '.';
				break;
			case '/':
				base64_output[i] = '_';
				break;
			case '=':
				base64_output[i] = '-';
				break;
		}
		printf("%c", base64_output[i]);
		toc.discid[i] = base64_output[i]; 
		if(DEBUG){printf("- %d - %x\n", i, base64_output[i]);}
	}
	free(base64_output);
	base64_output = base64_counter = NULL;
	free(sha1_output);
	printf("\n");
	return 0;
}




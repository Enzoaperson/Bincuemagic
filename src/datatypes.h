#ifndef DATATYPES
#define DATATYPES
#include "constants.h"

typedef struct{
	int number_of_tracks;
	int offset[MAX_AMOUNT_TRACKS];
	int leadout_offset;
	char discid[BASE64_OUTPUT_SIZE];
}tableofcontents;

#endif

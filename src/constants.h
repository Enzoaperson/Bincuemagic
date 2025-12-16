#ifndef CONSTANTS
#define CONSTANTS

//Errors
#define ERROR -1

//CD Conversions & Conventions
#define SECTORS_PER_SECOND 75
#define SECONDS_IN_MINUTE 60
#define BYTES_PER_SECTORS 2352
#define CD_LEADIN_OFFSET 150
#define CD_END_OF_BIN -1

//File Status
enum file_status {BIN, CUE, TOC, DNE};

//Min, Max & Sizes
#define PATH_SIZE 4096
#define FILE_NAME_SIZE 255
#define BASE64_OUTPUT_SIZE 28
#define MAX_AMOUNT_TRACKS 99



#endif

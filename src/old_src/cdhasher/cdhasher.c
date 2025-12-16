#include <stdio.h>

int main(){
	int tracks = 11;
	unsigned int one = 1;
	unsigned int two = 11;
	unsigned int leadout = 187872;
	unsigned int track_99[11] = {150,16192,30905,47575,58180,74865,96602,113395,134612,152945,171232};
	//printf("%d", sizeof(leadout));
	printf("\n%02X\n", one);
	printf("%02X\n", two);
	printf("%08X\n", leadout);
	for(int x = 0; x < 11; x++){
		printf("%08X", track_99[x]);
		//printf("%d ", track_99[x]);
	}
	for(int x = 0; x < 99 - 11; x++){
		printf("%08X", 0);
	}

	return 0;
}

#include <stdio.h>
#define MAX_LENGTH 500

//Tests on figuring out how to build a string
int main(int argc, char *argv[]){
	if (argv[1] == NULL){
		printf("No argument Given! >:(\n");
		return 0;
	}
	char new_name[MAX_LENGTH];
	int x = 0;
	while(argv[1][x] != '\0'){
		new_name[x] = argv[1][x];
		printf("%c", argv[1][x]);
		x++;
	}
	new_name[x] = '\0';
	printf("\n%s\n", new_name);

	return 0;
}

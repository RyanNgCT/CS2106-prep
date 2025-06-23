#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	pid_t pid;
	int x = 1;
	
	pid = fork();

	if (pid == 0) {
		printf("Child: x = %d\n", ++x);
		exit(0);
	}
	printf("Parent: x = %d\n", --x);
	exit(0);
	
	return 0;
}
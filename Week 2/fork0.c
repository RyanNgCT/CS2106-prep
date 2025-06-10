#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("I am ONE\n");
    fork();
    printf("I am seeing DOUBLE\n");

    return 0;
}

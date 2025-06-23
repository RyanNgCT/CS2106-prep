#define READ_END 0
#define WRITE_END 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int main() {
    int pipeFd[2], pid, len;
    char buf[100], *str = "Hello There!";

    pipe(pipeFd);
    if ((pid = fork()) > 0) { /* parent */
        close(pipeFd[READ_END]);
        write(pipeFd[WRITE_END], str, strlen(str) + 1);
        close(pipeFd[WRITE_END]);
    } else { /* child */
        close(pipeFd[WRITE_END]);
        len = read(pipeFd[READ_END], buf, sizeof buf);
        printf("Proc %d read: %s\n", pid, buf);
        close(pipeFd[READ_END]);
    }
}

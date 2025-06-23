#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void myOwnHandler( int signo )
{    
    if (signo == SIGSEGV){        
        printf("Memory access blows up!\n");
        exit(1);    
    }
}

int main(){    
    int *ip = NULL;    

    if (signal(SIGSEGV, myOwnHandler) == SIG_ERR)
        printf("Failed to register handler\n");
        
    *ip = 123;  

    return 0;
}

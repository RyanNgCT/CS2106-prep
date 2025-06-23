#include <pthread.h>
#include <stdio.h>
void* sayHello(void* arg) {
	printf("Just to say hello!\n");
	pthread_exit(NULL);
}

int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, sayHello, NULL);
	printf("Thread created with tid %lu\n", tid);
	return 0;
}

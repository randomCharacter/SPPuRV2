#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 1000

void* print (void *pParam) {
    char c = *(char*)pParam;
    int i;

    for(i = 0; i < N; i++) {
        printf("%c", c);
        fflush(stdout);
    }

    return 0;
}

int main (void) {
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    char c;
    /* Ovako ne treba raditi. */
    c = '1';
    pthread_create(&hPrint1, NULL, print, (void*)&c);
    c = '2';
    pthread_create(&hPrint2, NULL, print, (void*)&c);
    c = '3';
    pthread_create(&hPrint3, NULL, print, (void*)&c);

    getchar();

    return 0;
}

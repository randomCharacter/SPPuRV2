#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* Telo niti 1. */
void* print1 (void *pParam) {
    int i;

    for (i = 0 ; i < 1000 ; i++) {
        printf("1");
        fflush(stdout);
    }

    return 0;
}

/* Telo  niti 2. */
void* print2 (void *pParam) {
    int i;

    for (i = 0 ; i < 1000 ; i++) {
        printf("2");
        fflush(stdout);
        }

    return 0;
}

/* Telo niti 3. */
void* print3 (void *pParam) {
    int i;

    for (i = 0 ; i < 1000 ; i++) {
        printf("3");
        fflush(stdout);
    }

    return 0;
}

int main (void) {
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    /* Formiranje niti. */
    pthread_create(&hPrint1, NULL, print1, 0);
    pthread_create(&hPrint2, NULL, print2, 0);
    pthread_create(&hPrint3, NULL, print3, 0);

    getchar();

    return 0;
}

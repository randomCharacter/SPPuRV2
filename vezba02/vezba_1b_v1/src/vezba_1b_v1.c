#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t cs_mutex;


void* print1 (void *pParam) {
    int i;
    for (i = 0 ; i < 100 ; i++) {
        pthread_mutex_lock(&cs_mutex);

            printf("Zvezda");
            printf(" je");
            printf(" prvak");
            printf(" sveta\n");

        pthread_mutex_unlock(&cs_mutex);
    }

    return 0;
}

void* print2 (void *lpParam) {
    int i;
    for (i = 0 ; i < 100 ; i++) {
        pthread_mutex_lock(&cs_mutex);

            printf("Priprema");
            printf(" vezbe");
            printf(" iz ");
            printf(" Sistemske");
            printf(" Programske");
            printf(" Podrske\n");

        pthread_mutex_unlock(&cs_mutex);
    }

    return 0;
}

void* print3 (void *lpParam) {
    int i;
    for(i = 0 ; i < 100 ; i++) {
        pthread_mutex_lock(&cs_mutex);

            printf("Danas");
            printf(" je");
            printf(" lep");
            printf(" i");
            printf(" suncan");
            printf(" dan \n");

        pthread_mutex_unlock(&cs_mutex);
    }

    return 0;
}

int main (void) {
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    pthread_mutex_init(&cs_mutex, NULL);

    pthread_create(&hPrint1, NULL, print1, 0);
    pthread_create(&hPrint2, NULL, print2, 0);
    pthread_create(&hPrint3, NULL, print3, 0);

    getchar();

    pthread_mutex_destroy(&cs_mutex);

    return 0;
}

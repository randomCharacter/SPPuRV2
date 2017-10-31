#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphores[3];

void* print (void *pParam) {
    int n = (int)pParam;
    int i;

    for (i = 0; i < 1000; i++) {
        /* Svaka nit ceka signalizaciju na svom semaforu i
        signalizira semafor sledecoj niti. */
        sem_wait(&semaphores[n]);
        printf("%d", n+1);
        fflush(stdout);
        /* Signaliziraj narednu nit. */
        sem_post(&semaphores[(n + 1) % 3]);
    }
    return 0;
}

int main (void) {
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    /* Formiranje semafora za niti;
    Svakoj niti je namenjen jedan semafor;
    Semafor prve niti je inicijalno postavljen
    na jedinicu da bi niti otpocele sa radom. */
    sem_init(&semaphores[0], 0, 1);
    sem_init(&semaphores[1], 0, 0);
    sem_init(&semaphores[2], 0, 0);

    /* Formiranje niti;
    Sve niti imaju isto telo funkcije. */
    pthread_create(&hPrint3, NULL, print, (void*)2);
    pthread_create(&hPrint2, NULL, print, (void*)1);
    pthread_create(&hPrint1, NULL, print, (void*)0);

    pthread_join(hPrint3, NULL);
    pthread_join(hPrint2, NULL);
    pthread_join(hPrint1, NULL);

    sem_destroy(&semaphores[0]);
    sem_destroy(&semaphores[1]);
    sem_destroy(&semaphores[2]);

    return 0;
}

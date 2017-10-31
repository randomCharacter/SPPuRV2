#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t semaphore;

static int counter;

char getch(void);

/* Telo niti. */
void* counter_thread (void *pParam) {
    while (1) {
        /* Cekaj na signal da je pritisnut taster. */
        sem_wait(&semaphore);

        /* Povecaj brojac i ispisi vrednost. */
        counter++;
        printf("\r counter = %d\n", counter);
        fflush(stdout);
    }
}

int main (void) {
    pthread_t hThread;

    sem_init(&semaphore, 0, 0);
    pthread_create(&hThread, NULL, counter_thread, 0);

    /* Glavna petlja programa, zavrsava se pritiskom na 'q'. */
    while (1) {
        /* Preuzmi znak. Ako je preuzeti znak 'q', zavrsi program. */
        if (getch() == 'q') {
             break;
        }

        /* Obavesti nit da je pritisnut taster. */
        sem_post(&semaphore);
    }

    sem_destroy(&semaphore);

    return 0;
}

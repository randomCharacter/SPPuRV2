#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t startSemaphore1;
static sem_t startSemaphore2;
static sem_t startSemaphore3;

static pthread_t hThread1;
static pthread_t hThread2;
static pthread_t hThread3;

void* thread1 (void *pParam) {
    sem_wait(&startSemaphore1);

    /* Prikljuci se na nit 2. */
    pthread_join(hThread2, NULL);

    printf("Pozdrav iz niti 1.\n");

    return 0;
}

void* thread2 (void *pParam) {
    sem_wait(&startSemaphore2);

    /* Prikljuci se na nit 3. */
    pthread_join(hThread3, NULL);

    printf("Pozdrav iz niti 2.\n");

    return 0;
}

void* thread3 (void *pParam) {
    sem_wait(&startSemaphore3);

    printf("Pozdrav iz niti 3.\n");

    return 0;
}

int main (void) {
    sem_init(&startSemaphore1, 0, 0);
    sem_init(&startSemaphore2, 0, 0);
    sem_init(&startSemaphore3, 0, 0);

    pthread_create(&hThread1, NULL, thread1, 0);
    pthread_create(&hThread2, NULL, thread2, 0);
    pthread_create(&hThread3, NULL, thread3, 0);

    /* Signaliziranje nitima da mogu da pocnu sa radom.
    Ovim se osigurava da nijedna nit ne dodje do funkcije pthread_join
    a da nit koju zeli da prikljuci nije pokrenuta.
    */
    sem_post(&startSemaphore1);
    sem_post(&startSemaphore2);
    sem_post(&startSemaphore3);

    /* Prikljuci se na nit 1. */
    pthread_join(hThread1, NULL);

    sem_destroy(&startSemaphore1);
    sem_destroy(&startSemaphore2);
    sem_destroy(&startSemaphore3);

    return 0;
}

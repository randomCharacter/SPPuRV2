#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "timer_event.h"
#include "ring_buffer.h"

char getch(void);
void resetTermios(void);

/* Globalne promenljive. */
static struct RingBuffer ring =
{0, 0, {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'}};

static sem_t semEmpty;
static sem_t semFull;
// static sem_t semFinishSignal;
static sem_t semPrintSignal;

static pthread_mutex_t bufferAccess;
static pthread_mutex_t counterAccess;

int counter = 5;

/* Nit proizvodjaca. */
void* producer (void *param) {
    char c;

    while (1) {
        sem_wait(&semEmpty);

        /* Promena tipa ponistivosti niti proizvodjaca. */
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
        c = getch();
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

        pthread_mutex_lock(&bufferAccess);
        ringBufPutChar(&ring, c);
        pthread_mutex_unlock(&bufferAccess);

        sem_post(&semFull);
    }

    return 0;
}

/* Nit potrosaca. */
void* consumer (void *param) {
    char c;

    while (1) {


        pthread_mutex_lock(&counterAccess);
        if (counter == 0) {
            pthread_mutex_unlock(&counterAccess);
            break;
        }
        pthread_mutex_unlock(&counterAccess);


        if (sem_trywait(&semFull) == 0) {
            /* Postavljanje brojaca na pocetnu vrednost. */
            counter = 5;

            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&bufferAccess);
            c = ringBufGetChar(&ring);
            pthread_mutex_unlock(&bufferAccess);

            /* Ukoliko je unet karakter q ili Q nit treba da se zavrsi. */
            if (c == 'q' || c == 'Q') {
                break;
            }

            printf("Consumer: %c\n",c);
            fflush(stdout);
            sem_post(&semEmpty);
        }

    }

    // sem_post(&semFinishSignal);

    return 0;
}

/* Nit za ispis vrednosti karaktera iz kruznog bafera. */
void* print_state (void *param) {
    int i;

    while (1) {
        // if (sem_trywait(&semFinishSignal) == 0) {
        //     break;
        // }

        if (sem_trywait(&semPrintSignal) == 0) {
            /* Pristup kruznom baferu. */
            printf("------------\n");
            pthread_mutex_lock(&bufferAccess);
            for (i = 0 ; i < RING_SIZE ; i++) {
                printf("%c/", ring.data[i]);
            }
            printf("\n");
            pthread_mutex_unlock(&bufferAccess);
            printf("------------\n");
            fflush(stdout);
            pthread_testcancel();
        }
    }
    return 0;
}

/* Funkcija vremenske kontrole koje se poziva na svake dve sekunde. */
void* print_state_timer (void *param) {
    sem_post(&semPrintSignal);

    return 0;
}

void* decrement_counter(void *param) {
    pthread_mutex_lock(&counterAccess);
    counter--;
    pthread_mutex_unlock(&counterAccess);

    return 0;
}

int main (void) {
    pthread_t hProducer;
    pthread_t hConsumer;
    pthread_t hPrintState;

    /* Promenljiva koja predstavlja vremensku kontrolu. */
    timer_event_t hPrintStateTimer;
    timer_event_t hCounter;

    sem_init(&semEmpty, 0, RING_SIZE);
    sem_init(&semFull, 0, 0);
    // sem_init(&semFinishSignal, 0, 0);
    sem_init(&semPrintSignal, 0, 0);

    pthread_mutex_init(&bufferAccess, NULL);
    pthread_mutex_init(&counterAccess, NULL);

    pthread_create(&hProducer, NULL, producer, 0);
    pthread_create(&hConsumer, NULL, consumer, 0);
    pthread_create(&hPrintState, NULL, print_state, 0);

    /* Formiranje vremenske kontrole za funkciju print_state_timer. */
    timer_event_set(&hPrintStateTimer, 2000, print_state_timer, 0, TE_KIND_REPETITIVE);
    timer_event_set(&hCounter, 1000, decrement_counter, 0, TE_KIND_REPETITIVE);

    pthread_join(hConsumer, NULL);
    pthread_cancel(hPrintState);
    pthread_join(hPrintState, NULL);
    pthread_cancel(hProducer);
    pthread_join(hProducer, NULL);

    /* Zaustavljanje vremenske kontrole. */
    timer_event_kill(hPrintStateTimer);
    timer_event_kill(hCounter);

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    // sem_destroy(&semFinishSignal);
    sem_destroy(&semPrintSignal);
    pthread_mutex_destroy(&bufferAccess);

    /* Nit proizvodjaca se najcesce prekida u izvrsavanju blokirajuce funkcije getch,
       koja u sebi sadrzi izmenu nacina rada terminala, a na samom kraju i vracanje
       na prethodno podesavanje terminala. Stoga je moguce da nakon prekida ove funkcije
       terminal ostatne u izmenjenom stanju. Funkcijom resetTermios obezbedjujemo da
       nakon ispravnog zavrsetka programa, terminal ostane u zatecenom stanju. */
    resetTermios();

    return 0;
}

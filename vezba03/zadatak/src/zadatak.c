#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define RING_SIZE  (16)
#define SLEEPING_TIME (400000)

char getch(void);

/*  Kruzni bafer - FIFO. */
struct RingBuffer {
    unsigned int  tail;
    unsigned int  head;
    unsigned char data[RING_SIZE];
};

/* Operacije za rad sa kruznim baferom. */
char ringBufGetChar (struct RingBuffer *apBuffer) {
    int index;
    index = apBuffer->head;
    apBuffer->head = (apBuffer->head + 1) % RING_SIZE;
    return apBuffer->data[index];
}

void ringBufPutChar (struct RingBuffer *apBuffer, const char c) {
    apBuffer->data[apBuffer->tail] = c;
    apBuffer->tail = (apBuffer->tail + 1) % RING_SIZE;
}

/* Globalne promenljive. */

/* Kruzni bafer. */
static struct RingBuffer inRing, outRing;

static sem_t semInEmpty;
static sem_t semInFull;
static sem_t semOutEmpty;
static sem_t semOutFull;
static sem_t semFinishSignal;

static pthread_mutex_t inBufferAccess, outBufferAccess;;

/* Funkcija programske niti proizvodjaca. */
void* input (void *param) {
    char c;

    while (1) {
        if (sem_trywait(&semFinishSignal) == 0) {
            break;
        }

        if (sem_trywait(&semInEmpty) == 0) {

            /* Funkcija za unos karaktera sa tastature. */
            c = getch();

            /* Ukoliko je unet karakter q ili Q signalizira se
               programskim nitima zavrsetak rada. */
            if (c == 'q' || c == 'Q') {
                /* Zaustavljanje niti; Semafor se oslobadja 3 puta
                da bi signaliziralo svim nitima.*/
                sem_post(&semFinishSignal);
                sem_post(&semFinishSignal);
                sem_post(&semFinishSignal);
            }

            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&inBufferAccess);
            ringBufPutChar(&inRing, c);
            pthread_mutex_unlock(&inBufferAccess);

            sem_post(&semInFull);
        }
    }

    return 0;
}

/* Funkcija programske niti potrosaca.*/
void* output (void *param) {
    char c;

    printf("Znakovi preuzeti iz izlaznog kruznog bafera:\n");

    while (1) {
        if (sem_trywait(&semFinishSignal) == 0) {
            break;
        }

        if (sem_trywait(&semOutFull) == 0) {
            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&outBufferAccess);
            c = ringBufGetChar(&outRing);
            pthread_mutex_unlock(&outBufferAccess);

            /* Ispis na konzolu. */
            printf("%c", c);
            fflush(stdout);

            /* Cekanje da bi se ilustrovalo trajanje obrade. */
            usleep(SLEEPING_TIME);

            sem_post(&semOutEmpty);
        }
    }

    return 0;
}

void* edit(void *param) {
    char c;

    while (1) {
        if (sem_trywait(&semFinishSignal) == 0) {
            break;
        }

        if (sem_trywait(&semInFull) == 0) {
            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&inBufferAccess);
            c = ringBufGetChar(&inRing);
            pthread_mutex_unlock(&inBufferAccess);

            c -= 0x20;

            /* Pisanje u drugi bafer. */
            pthread_mutex_lock(&outBufferAccess);
            ringBufPutChar(&outRing, c);
            pthread_mutex_unlock(&outBufferAccess);

            /* Cekanje da bi se ilustrovalo trajanje obrade. */
            usleep(SLEEPING_TIME);

            sem_post(&semInEmpty);
            sem_post(&semOutFull);
        }
    }

    return 0;
}

/* Glavna programska nit koja formira dve programske (proizvodjac i potrosac) niti i ceka njihovo gasenje. */
int main (void) {
    /* Identifikatori niti. */
    pthread_t hInput;
    pthread_t hOutput;
    pthread_t hEdit;

    /* Formiranje semEmpty, semFull i semFinishSignal semafora. */
    sem_init(&semInEmpty, 0, RING_SIZE);
    sem_init(&semInFull, 0, 0);
    sem_init(&semOutEmpty, 0, RING_SIZE);
    sem_init(&semOutFull, 0, 0);
    sem_init(&semFinishSignal, 0, 0);

    /* Inicijalizacija objekta iskljucivog pristupa. */
    pthread_mutex_init(&inBufferAccess, NULL);
    pthread_mutex_init(&outBufferAccess, NULL);

    /* Formiranje programskih niti: proizodjac i potrosac. */
    pthread_create(&hInput, NULL, input, 0);
    pthread_create(&hOutput, NULL, output, 0);
    pthread_create(&hEdit, NULL, edit, 0);

    /* Cekanje na zavrsetak formiranih niti. */
    pthread_join(hInput, NULL);
    pthread_join(hOutput, NULL);
    pthread_join(hEdit, NULL);

    /* Oslobadjanje resursa. */
    sem_destroy(&semInEmpty);
    sem_destroy(&semInFull);
    sem_destroy(&semOutEmpty);
    sem_destroy(&semOutFull);
    sem_destroy(&semFinishSignal);
    pthread_mutex_destroy(&inBufferAccess);
    pthread_mutex_destroy(&outBufferAccess);

    printf("\n");

    return 0;
}

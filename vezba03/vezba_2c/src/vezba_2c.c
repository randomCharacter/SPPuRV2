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
static struct RingBuffer ring;

static sem_t semEmpty;
static sem_t semFull;
static sem_t semFinishSignal;

static pthread_mutex_t bufferAccess;

/* Funkcija programske niti proizvodjaca. */
void* producer (void *param) {
    char c;

    while (1) {
        if (sem_trywait(&semFinishSignal) == 0) {
            break;
        }

        if (sem_trywait(&semEmpty) == 0) {

            /* Funkcija za unos karaktera sa tastature. */
            c = getch();

            /* Ukoliko je unet karakter q ili Q signalizira se 
               programskim nitima zavrsetak rada. */
            if (c == 'q' || c == 'Q') {
                /* Zaustavljanje niti; Semafor se oslobadja 2 puta
                da bi signaliziralo obema nitima.*/
                sem_post(&semFinishSignal);
                sem_post(&semFinishSignal);
            }

            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&bufferAccess);
            ringBufPutChar(&ring, c);
            pthread_mutex_unlock(&bufferAccess);

            sem_post(&semFull);
        }
    }

    return 0;
}

/* Funkcija programske niti potrosaca.*/
void* consumer (void *param) {
    char c;

    printf("Znakovi preuzeti iz kruznog bafera:\n");

    while (1) {
        if (sem_trywait(&semFinishSignal) == 0) {
            break;
        }

        if (sem_trywait(&semFull) == 0) {
            /* Pristup kruznom baferu. */
            pthread_mutex_lock(&bufferAccess);
            c = ringBufGetChar(&ring);
            pthread_mutex_unlock(&bufferAccess);

            /* Ispis na konzolu. */
            printf("%c", c);
            fflush(stdout);

            /* Cekanje da bi se ilustrovalo trajanje obrade. */
            usleep(SLEEPING_TIME);

            sem_post(&semEmpty);
        }
    }

    return 0;
}

/* Glavna programska nit koja formira dve programske (proizvodjac i potrosac) niti i ceka njihovo gasenje. */
int main (void) {
    /* Identifikatori niti. */
    pthread_t hProducer;
    pthread_t hConsumer;

    /* Formiranje semEmpty, semFull i semFinishSignal semafora. */
    sem_init(&semEmpty, 0, RING_SIZE);
    sem_init(&semFull, 0, 0);
    sem_init(&semFinishSignal, 0, 0);

    /* Inicijalizacija objekta iskljucivog pristupa. */
    pthread_mutex_init(&bufferAccess, NULL);

    /* Formiranje programskih niti: proizodjac i potrosac. */
    pthread_create(&hProducer, NULL, producer, 0);
    pthread_create(&hConsumer, NULL, consumer, 0);

    /* Cekanje na zavrsetak formiranih niti. */
    pthread_join(hConsumer, NULL);
    pthread_join(hProducer, NULL);

    /* Oslobadjanje resursa. */
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&semFinishSignal);
    pthread_mutex_destroy(&bufferAccess);

    printf("\n");

    return 0;
}

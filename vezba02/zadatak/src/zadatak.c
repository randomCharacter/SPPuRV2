#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t cs_mutex;

typedef struct {
  int c;
  int *next;
  pthread_mutex_t *mutex;
} st;

void* print(void *pParam) {
    int c = ((st*)pParam)->c;
    int *next = ((st*)pParam)->next;
    pthread_mutex_t *mutex = ((st*)pParam)->mutex;
    int i;

    for(i = 0; i < 1000; i++) {
        int b = 0;
        while (!b) {
            pthread_mutex_lock(mutex);
            b = (*next == c);
            pthread_mutex_unlock(mutex);
        }
        pthread_mutex_lock(mutex);
        printf("%d", c + 1);
        fflush(stdout);
        *next = (*next + 1) % 3;
        pthread_mutex_unlock(mutex);
    }

    return 0;
}

int main(void) {
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    pthread_mutex_init(&cs_mutex, NULL);

    st str[3];
    int next = 0;

    int i;
    for (i = 0; i < 3; i++) {
      str[i].c = i;
      str[i].next = &next;
      str[i].mutex = &cs_mutex;
    }

    pthread_create(&hPrint1, NULL, print, (void*)&str[0]);
    pthread_create(&hPrint2, NULL, print, (void*)&str[1]);
    pthread_create(&hPrint3, NULL, print, (void*)&str[2]);

    getchar();

    return 0;
}

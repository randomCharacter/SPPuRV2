#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_LEN 80

int file_desc;
static pthread_mutex_t led_mutex;
static char states[4] = "0000";

void* led_thread(void *param) {
    int p = *(int*) param;
    char tmp[BUF_LEN], output[BUF_LEN];
    while (1) {
        pthread_mutex_lock(&led_mutex);
        
        // Open file
        file_desc = open("/dev/gpio_driver", O_RDWR);
        if (file_desc < 0) {
            printf("Error, file not opened\n");
            exit(1);
        }

        read(file_desc, tmp, BUF_LEN);

        // Check if state of switch changed
        if (states[p] != tmp[p]) {
            states[p] = tmp[p];
            sprintf(output, "LED %d %d", p, states[p]);
            write(file_desc, output, BUF_LEN);
        }

        // Close file
        close(file_desc);

        // Unlock mutex and let other thread continue
        pthread_mutex_unlock(&led_mutex);
        usleep(100);
    }
}

int main() {
    int c0 = 0, c1 = 1, c2 = 2, c3 = 3;
    pthread_t hLed0, hLed1, hLed2, hLed3;

    pthread_create(&hLed0, NULL, led_thread, (void*)&c0);
    pthread_create(&hLed1, NULL, led_thread, (void*)&c1);
    pthread_create(&hLed2, NULL, led_thread, (void*)&c2);
    pthread_create(&hLed3, NULL, led_thread, (void*)&c3);

    pthread_join(hLed0, NULL);
    pthread_join(hLed1, NULL);
    pthread_join(hLed2, NULL);
    pthread_join(hLed3, NULL);

    pthread_mutex_destroy(&led_mutex);

    return 0;
}

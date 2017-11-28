#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_LEN 80

int main()
{
    int file_desc;
    int ret_val;
    char tmp[BUF_LEN] = {0};

    file_desc = open("/dev/gpio_driver", O_RDWR);

    if(file_desc < 0)
    {
        printf("Error, file not opened\n");
        return -1;
    }

    ret_val = read(file_desc, tmp, BUF_LEN);

    printf("%s\n", tmp);

    close(file_desc);

    return 0;
}

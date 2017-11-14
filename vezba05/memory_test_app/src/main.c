#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_LEN 80

int main() {
	int file_desc;
	int ret_val;
	char *msg = "'/dev/memory' test message!!!\n";
	char tmp[BUF_LEN];

	/* Open /dev/memory device. */
	file_desc = open("/dev/memory", O_RDWR);

	if(file_desc < 0) {
		printf("'/dev/memory' device isn't open\n");
		printf("Try:\t1) Check does '/dev/memory' node exist\n\t2)'chmod 666 /dev/ \
		        memory'\n\t3) ""insmod"" memory module\n");

		return -1;
	}
	printf("'/dev/memory' device is successfully opened!\n");

	/* Write to /dev/memory device. */
	ret_val = write(file_desc, msg, strlen(msg));
	printf("Written message: %s", msg);
	printf("ret_val: %d\n", ret_val);

	/* Reopen /dev/memory device. */
	close(file_desc);
	file_desc = open("/dev/memory", O_RDWR);
	if(file_desc < 0) {
  	printf("'/dev/memory' device isn't open\n");
		printf("Try:\t1) Check does '/dev/memory' node exist\n\t2)'chmod 666 /dev/ \
		        memory'\n\t3) ""insmod"" memory module\n");

		return -1;
	}

	/* Init temp buffer. */
	memset(tmp, 0, BUF_LEN);

	/* Read from /dev/memory device. */
	ret_val = read(file_desc, tmp, BUF_LEN);
	printf("Read message: %s", tmp);
	printf("ret_val: %d\n", ret_val);

	/* Close /dev/memory device. */
	close(file_desc);

	return 0;
}

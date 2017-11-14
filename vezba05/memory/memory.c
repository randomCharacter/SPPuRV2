#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");

#define BUF_LEN 80

/* Declaration of memory.c functions */
void memory_exit(void);
int memory_init(void);
static int memory_open(struct inode *, struct file *);
static int memory_release(struct inode *, struct file *);
static ssize_t memory_read(struct file *, char *buf, size_t , loff_t *);
static ssize_t memory_write(struct file *, const char *buf, size_t , loff_t *);

/* Structure that declares the usual file access functions. */
struct file_operations memory_fops =
{
    read    :   memory_read,
    write   :   memory_write,
    open    :   memory_open,
    release :   memory_release
};

/* Declaration of the init and exit functions. */
module_init(memory_init);
module_exit(memory_exit);


/* Global variables of the driver */

/* Major number. */
int memory_major = 60;

/* Buffer to store data. */
char *memory_buffer;

/*
 * Initialization:
 *  1. Register device driver
 *  2. Allocate buffer
 *  3. Initialize buffer
 */
int memory_init(void) {
	int result;

	/* Registering device. */
	result = register_chrdev(memory_major, "memory", &memory_fops);
	if (result < 0) {
		printk(KERN_INFO "memory: cannot obtain major number %d\n", memory_major);
		return result;
	}

	/* Allocating memory for the buffer. */
	memory_buffer = kmalloc(BUF_LEN, GFP_KERNEL);
	if (!memory_buffer) {
		result = -ENOMEM;
		goto fail;
	}
    printk(KERN_INFO "Inserting memory module\n");

	memset(memory_buffer, 0, BUF_LEN);
	return 0;

fail:
	memory_exit();
	return result;
}

/*
 * Cleanup:
 *  1. Unregister device driver
 *  2. Free buffer
 */
void memory_exit(void) {
	/* Freeing the major number. */
	unregister_chrdev(memory_major, "memory");

	/* Freeing buffer memory. */
	if (memory_buffer) {
		kfree(memory_buffer);
	}

	printk(KERN_INFO "Removing memory module\n");
}

/* File open function. */
static int memory_open(struct inode *inode, struct file *filp) {
	/* Initialize driver variables here. */

	/* Reset the device here. */

	/* Success. */
	return 0;
}

/* File close function. */
static int memory_release(struct inode *inode, struct file *filp) {
	/* Free memory allocated in memory_open here. */

	/* Success. */
	return 0;
}

/*
 * File read function
 *  Parameters:
 *   filp  - a type file structure;
 *   buf   - a buffer, from which the user space function (fread) will read;
 *   len - a counter with the number of bytes to transfer, which has the same
 *           value as the usual counter in the user space function (fread);
 *   f_pos - a position of where to start reading the file;
 *  Operation:
 *   The memory_read function transfers data from the driver buffer (memory_buffer)
 *   to user space with the function copy_to_user.
 */
static ssize_t memory_read(struct file *filp, char *buf, size_t len, loff_t *f_pos) {
	/* Size of valid data in memory - data to send in user space. */
	int data_size = 0;

	if (*f_pos == 0) {
		/* Get size of valid data. */
		data_size = strlen(memory_buffer);

		/* Send data to user space. */
		if (copy_to_user(buf, memory_buffer, data_size) != 0) {
			return -EFAULT;
		} else {
			(*f_pos) += data_size;

			return data_size;
		}
	} else {
		return 0;
	}
}

/*
 * File write function
 *  Parameters:
 *   filp  - a type file structure;
 *   buf   - a buffer in which the user space function (fwrite) will write;
 *   len - a counter with the number of bytes to transfer, which has the same
 *           values as the usual counter in the user space function (fwrite);
 *   f_pos - a position of where to start writing in the file;
 *  Operation:
 *   The function copy_from_user transfers the data from user space to kernel space.
 */
static ssize_t memory_write(struct file *filp, const char *buf, size_t len, loff_t *f_pos) {
	/* Reset memory. */
	memset(memory_buffer, 0, BUF_LEN);

	/* Get data from user space.*/
	if (copy_from_user(memory_buffer, buf, len) != 0) {
		return -EFAULT;
	} else {
		return len;
	}
}

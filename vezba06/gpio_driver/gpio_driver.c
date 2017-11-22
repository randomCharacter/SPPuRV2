#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define TEST

MODULE_LICENSE("Dual BSD/GPL");

// NOTE: Check Broadcom BCM8325 datasheet, page 91+

/* GPIO registers base address. */
#define GPIO_BASE_ADDR (0x3F200000)
//--

//000 = GPIO Pin 'x' is an input
//001 = GPIO Pin 'x' is an output
// By default GPIO pin is being used as an input
#define GPIO_DIRECTION_IN  (0)
#define GPIO_DIRECTION_OUT (1)
//--

//Handle GPIO: 0-9
/* GPIO Function Select 0. */
#define GPFSEL0_BASE_ADDR (GPIO_BASE_ADDR + 0x00000000)

//Handle GPIO: 10-19
/* GPIO Function Select 1. */
#define GPFSEL1_BASE_ADDR (GPIO_BASE_ADDR + 0x00000004)

//Handle GPIO: 20-29
/* GPIO Function Select 2. */
#define GPFSEL2_BASE_ADDR (GPIO_BASE_ADDR + 0x00000008)

//Handle GPIO: 30-39
/* GPIO Function Select 3. */
#define GPFSEL3_BASE_ADDR (GPIO_BASE_ADDR + 0x0000000C)

//Handle GPIO: 40-49
/* GPIO Function Select 4. */
#define GPFSEL4_BASE_ADDR (GPIO_BASE_ADDR + 0x00000010)

//Handle GPIO: 50-53
/* GPIO Function Select 5. */
#define GPFSEL5_BASE_ADDR (GPIO_BASE_ADDR + 0x00000014)
//--

//GPIO: 0-31
/* GPIO Pin Output Set 0. */
#define GPSET0_BASE_ADDR (GPIO_BASE_ADDR + 0x0000001C)

//GPIO: 32-53
/* GPIO Pin Output Set 1. */
#define GPSET1_BASE_ADDR (GPIO_BASE_ADDR + 0x00000020)

//--
//GPIO: 0-31
/* GPIO Pin Output Clear 0. */
#define GPCLR0_BASE_ADDR (GPIO_BASE_ADDR + 0x00000028)

//GPIO: 32-53
/* GPIO Pin Output Clear 1. */
#define GPCLR1_BASE_ADDR (GPIO_BASE_ADDR + 0x0000002C)

//--
//GPIO: 0-31
/* GPIO Pin Level 0. */
#define GPLEV0_BASE_ADDR (GPIO_BASE_ADDR + 0x00000034)

//GPIO: 32-53
/* GPIO Pin Level 1. */
#define GPLEV1_BASE_ADDR (GPIO_BASE_ADDR + 0x00000038)
//--

//GPIO: 0-53
/* GPIO Pin Pull-up/down Enable. */
#define GPPUD_BASE_ADDR (GPIO_BASE_ADDR + 0x00000094)

//GPIO: 0-31
/* GPIO Pull-up/down Clock Register 0. */
#define GPPUDCLK0_BASE_ADDR (GPIO_BASE_ADDR + 0x00000098)

//GPIO: 32-53
/* GPIO Pull-up/down Clock Register 1. */
#define GPPUDCLK1_BASE_ADDR (GPIO_BASE_ADDR + 0x0000009C)

/* PUD - GPIO Pin Pull-up/down */
#define PULL_NONE (0)
#define PULL_DOWN (1)
#define PULL_UP   (2)


/* Avilable GPIO pins. */
#define GPIO_02 (2)
#define GPIO_03 (3)
#define GPIO_04 (4)
#define GPIO_05 (5)
#define GPIO_06 (6)
#define GPIO_07 (7)
#define GPIO_08 (8)
#define GPIO_09 (9)
#define GPIO_10 (10)
#define GPIO_11 (11)
#define GPIO_12 (12)
#define GPIO_13 (13)
#define GPIO_14 (14)
#define GPIO_15 (15)
#define GPIO_16 (16)
#define GPIO_17 (17)
#define GPIO_18 (18)
#define GPIO_19 (19)
#define GPIO_20 (20)
#define GPIO_21 (21)
#define GPIO_22 (22)
#define GPIO_23 (23)
#define GPIO_24 (24)
#define GPIO_25 (25)
#define GPIO_26 (26)
#define GPIO_27 (27)

/* Declaration of gpio_driver.c functions */
int gpio_driver_init(void);
void gpio_driver_exit(void);
static int gpio_driver_open(struct inode *, struct file *);
static int gpio_driver_release(struct inode *, struct file *);
static ssize_t gpio_driver_read(struct file *, char *buf, size_t , loff_t *);
static ssize_t gpio_driver_write(struct file *, const char *buf, size_t , loff_t *);

/* Structure that declares the usual file access functions. */
struct file_operations gpio_driver_fops =
{
    open    :   gpio_driver_open,
    release :   gpio_driver_release,
    read    :   gpio_driver_read,
    write   :   gpio_driver_write
};

/* Declaration of the init and exit functions. */
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

/* Global variables of the driver */
static int led0 = 1;
static int led1 = 1;
static int led2 = 1;
static int led3 = 1;

/* Module params */
module_param(led0, int, 1);
MODULE_PARM_DESC(led0, "LED0 state.");
module_param(led1, int, 1);
MODULE_PARM_DESC(led1, "LED1 state.");
module_param(led2, int, 1);
MODULE_PARM_DESC(led2, "LED2 state.");
module_param(led3, int, 1);
MODULE_PARM_DESC(led3, "LED1 state.");

/* Major number. */
int gpio_driver_major;

/* Buffer to store data. */
#define BUF_LEN 80
static char gpio_driver_buffer[BUF_LEN];


#define MS_TO_NS(x) ((x) * 1E6L)
#define TIMER_SEC    0
#define TIMER_NANO_SEC  250*1000*1000 /* 250ms */

unsigned int GetGPFSELReg(char pin) {
    unsigned int addr;

    if(pin >= 0 && pin <10)
        addr = GPFSEL0_BASE_ADDR;
    else if(pin >= 10 && pin <20)
        addr = GPFSEL1_BASE_ADDR;
    else if(pin >= 20 && pin <30)
        addr = GPFSEL2_BASE_ADDR;
    else if(pin >= 30 && pin <40)
        addr = GPFSEL3_BASE_ADDR;
    else if(pin >= 40 && pin <50)
        addr = GPFSEL4_BASE_ADDR;
    else /*if(pin >= 50 && pin <53) */
        addr = GPFSEL5_BASE_ADDR;

  return addr;
}

char GetGPIOPinOffset(char pin) {
    if(pin >= 0 && pin <10)
        pin = pin;
    else if(pin >= 10 && pin <20)
        pin -= 10;
    else if(pin >= 20 && pin <30)
        pin -= 20;
    else if(pin >= 30 && pin <40)
        pin -= 30;
    else if(pin >= 40 && pin <50)
        pin -= 40;
    else /*if(pin >= 50 && pin <53) */
        pin -= 50;

    return pin;
}

void SetInternalPullUpDown(char pin, char value) {
    unsigned int base_addr_gppud;
    unsigned int base_addr_gppudclk;
    void *addr = NULL;
    unsigned int tmp;
    unsigned int mask;

    /* Get base address of GPIO Pull-up/down Register (GPPUD). */
    base_addr_gppud = GPPUD_BASE_ADDR;

    /* Get base address of GPIO Pull-up/down Clock Register (GPPUDCLK). */
    base_addr_gppudclk = (pin < 32) ? GPPUDCLK0_BASE_ADDR : GPPUDCLK1_BASE_ADDR;

    /* Get pin offset in register . */
    pin = (pin < 32) ? pin : pin - 32;

    /* Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
       to remove the current Pull-up/down). */
    addr = ioremap(base_addr_gppud, 4);
    iowrite32(value, addr);

    /* Wait 150 cycles � this provides the required set-up time for the control signal */

    /* Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
       modify � NOTE only the pads which receive a clock will be modified, all others will
       retain their previous state. */
    addr = ioremap(base_addr_gppudclk, 4);
    tmp = ioread32(addr);
    mask = 0x1 << pin;
    tmp |= mask;
    iowrite32(tmp, addr);

    /* Wait 150 cycles � this provides the required hold time for the control signal */

    /* Write to GPPUD to remove the control signal. */
    addr = ioremap(base_addr_gppud, 4);
    iowrite32(PULL_NONE, addr);

    /* Write to GPPUDCLK0/1 to remove the clock. */
    addr = ioremap(base_addr_gppudclk, 4);
    tmp = ioread32(addr);
    mask = 0x1 << pin;
    tmp &= (~mask);
    iowrite32(tmp, addr);
}


void SetGpioPinDirection(char pin, char direction) {
    unsigned int base_addr;
    void *addr = NULL;
    unsigned int tmp;
    unsigned int mask;

    /* Get base address of function selection register. */
    base_addr = GetGPFSELReg(pin);

    /* Calculate gpio pin offset. */
    pin = GetGPIOPinOffset(pin);

    /* Set gpio pin direction. */
    addr = ioremap(base_addr, 4);
    tmp = ioread32(addr);
    if(direction) {
      //set as output: set 1
      mask = 0x1 << (pin*3);
      tmp |= mask;
    }
    else {
      //set as input: set 0
      mask = ~(0x1 << (pin*3));
      tmp &= mask;
    }
    iowrite32(tmp, addr);
}

void SetGpioPin(char pin) {
    void *addr = NULL;
    unsigned int tmp;

    /* Get base address of gpio set register. */
    addr = (pin < 32) ? (void *) GPSET0_BASE_ADDR : (void *)GPSET1_BASE_ADDR;
    pin = (pin < 32) ? pin : pin - 32;

    /* Set gpio. */
    addr = ioremap((unsigned long)addr, 4);
    tmp = 0x1 << pin;
    iowrite32(tmp, addr);
}

void ClearGpioPin(char pin) {
    void *addr = NULL;
    unsigned int tmp;

    /* Get base address of gpio clear register. */
    addr = (pin < 32) ? (void *)GPCLR0_BASE_ADDR : (void *)GPCLR1_BASE_ADDR;
    pin = (pin < 32) ? pin : pin - 32;

    /* Clear gpio. */
    addr = ioremap((unsigned long)addr, 4);
    tmp = 0x1 << pin;
    iowrite32(tmp, addr);
}

char GetGpioPinValue(char pin) {
    void *addr = NULL;
    unsigned int tmp;
    unsigned int mask;

    /* Get base address of gpio level register. */
    addr = (pin < 32) ? (void *) GPLEV0_BASE_ADDR : (void *)GPLEV1_BASE_ADDR;
    pin = (pin < 32) ? pin : pin - 32;

    /* Read gpio pin level. */
    addr = ioremap((unsigned long)addr, 4);
    tmp = ioread32(addr);
    mask = 0x1 << pin;
    tmp &= mask;

    return (tmp >> pin);
}

/*
 * Initialization:
 *  1. Register device driver
 *  2. Allocate buffer
 *  3. Initialize buffer
 */
int gpio_driver_init(void) {
    int result = -1;

    /* Registering device. */
    result = register_chrdev(0, "gpio_driver", &gpio_driver_fops);
    if (result < 0)
    {
        printk(KERN_INFO "gpio_driver: cannot obtain major number %d\n", gpio_driver_major);
        return result;
    }

    gpio_driver_major = result;
    printk(KERN_INFO "gpio_driver major number is %d\n", gpio_driver_major);

    /* Initialize data buffer. */
    memset(gpio_driver_buffer, 0, BUF_LEN);

    printk(KERN_INFO "Inserting gpio_driver module\n");

    /* Initialize high resolution timer. */
    /*hrtimer_init(&blink_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    kt = ktime_set(TIMER_SEC, TIMER_NANO_SEC);
    blink_timer.function = &blink_timer_callback;
    hrtimer_start(&blink_timer, kt, HRTIMER_MODE_REL);*/

    /* Initialize GPIO pins. */
    // LED
    SetGpioPinDirection(GPIO_06, GPIO_DIRECTION_OUT);
    SetGpioPinDirection(GPIO_13, GPIO_DIRECTION_OUT);
    SetGpioPinDirection(GPIO_19, GPIO_DIRECTION_OUT);
    SetGpioPinDirection(GPIO_26, GPIO_DIRECTION_OUT);

    // Turn LEDs on
    if (led0) {
        SetGpioPin(GPIO_06);
    } else {
        ClearGpioPin(GPIO_06);
    }
    if (led1) {
        SetGpioPin(GPIO_13);
    } else {
        ClearGpioPin(GPIO_13);
    }
    if (led2) {
        SetGpioPin(GPIO_19);
    } else {
        ClearGpioPin(GPIO_19);
    }
    if (led3) {
        SetGpioPin(GPIO_26);
    } else {
        ClearGpioPin(GPIO_26);
    }

    // Switch
    SetInternalPullUpDown(GPIO_12, PULL_UP);
    SetGpioPinDirection(GPIO_12, GPIO_DIRECTION_IN);
    SetInternalPullUpDown(GPIO_16, PULL_UP);
    SetGpioPinDirection(GPIO_16, GPIO_DIRECTION_IN);
    SetInternalPullUpDown(GPIO_20, PULL_UP);
    SetGpioPinDirection(GPIO_20, GPIO_DIRECTION_IN);
    SetInternalPullUpDown(GPIO_21, PULL_UP);
    SetGpioPinDirection(GPIO_21, GPIO_DIRECTION_IN);

    return 0;
}

/*
 * Cleanup:
 *  1. Unregister device driver
 *  2. Free buffer
 */
void gpio_driver_exit(void) {
    /* Freeing the major number. */
    unregister_chrdev(gpio_driver_major, "gpio_driver");

    printk(KERN_INFO "Removing gpio_driver module\n");

    /* Release high resolution timer. */
    //hrtimer_cancel(&blink_timer);

    /* Clear GPIO pins. */
    ClearGpioPin(GPIO_06);

    /* Set GPIO pins as inputs and disable pull-ups. */
    SetGpioPinDirection(GPIO_06, GPIO_DIRECTION_IN);
    SetInternalPullUpDown(GPIO_12, PULL_NONE);
}

/* File open function. */
static int gpio_driver_open(struct inode *inode, struct file *filp) {
    /* Initialize driver variables here. */

    /* Reset the device here. */

    /* Success. */
    return 0;
}

/* File close function. */
static int gpio_driver_release(struct inode *inode, struct file *filp) {
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
 *   The gpio_driver_read function transfers data from the driver buffer (gpio_driver_buffer)
 *   to user space with the function copy_to_user.
 */
static ssize_t gpio_driver_read(struct file *filp, char *buf, size_t len, loff_t *f_pos) {
    /* Size of valid data in gpio_driver - data to send in user space. */
    int data_size = 0;

    if (GetGpioPinValue(GPIO_12)) {
        gpio_driver_buffer[0] = '1';
    } else {
        gpio_driver_buffer[0] = '0';
    }
    if (GetGpioPinValue(GPIO_16)) {
        gpio_driver_buffer[1] = '1';
    } else {
        gpio_driver_buffer[1] = '0';
    }
    if (GetGpioPinValue(GPIO_20)) {
        gpio_driver_buffer[2] = '1';
    } else {
        gpio_driver_buffer[2] = '0';
    }
    if (GetGpioPinValue(GPIO_21)) {
        gpio_driver_buffer[3] = '1';
    } else {
        gpio_driver_buffer[3] = '0';
    }

    //gpio_driver_buffer[4] = '\0';

    if (*f_pos == 0) {
        /* Get size of valid data. */
        data_size = strlen(gpio_driver_buffer);

        /* Send data to user space. */
        if (copy_to_user(buf, gpio_driver_buffer, data_size) != 0) {
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
static ssize_t gpio_driver_write(struct file *filp, const char *buf, size_t len, loff_t *f_pos) {
    /* Reset memory. */
    memset(gpio_driver_buffer, 0, BUF_LEN);
    printk("%d", len);
    /* Get data from user space.*/
    if (copy_from_user(gpio_driver_buffer, buf, len) != 0) {
        return -EFAULT;
    } else {
        switch(gpio_driver_buffer[4]) {
            case '0':
                if (gpio_driver_buffer[6] == '1') {
                    SetGpioPin(GPIO_06);
                } else {
                    ClearGpioPin(GPIO_06);
                }
                break;
            case '1':
                if (gpio_driver_buffer[6] == '1') {
                    SetGpioPin(GPIO_13);
                } else {
                    ClearGpioPin(GPIO_13);
                }
                break;
            case '2':
                if (gpio_driver_buffer[6] == '1') {
                    SetGpioPin(GPIO_19);
                } else {
                    ClearGpioPin(GPIO_19);
                }
                break;
            case '3':
                if (gpio_driver_buffer[6] == '1') {
                    SetGpioPin(GPIO_26);
                } else {
                    ClearGpioPin(GPIO_26);
                }
                break;
        }
        return len;
    }
}

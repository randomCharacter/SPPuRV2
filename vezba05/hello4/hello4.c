/*
 * hello4.c − Demonstrates module documentation.
 */

#include <linux/module.h>   /* Needed by all modules. */
#include <linux/kernel.h>   /* Needed for KERN_INFO. */
#include <linux/init.h>     /* Needed for the macros. */

#define DRIVER_AUTHOR   "RT-RK"
#define DRIVER_DESC     "A simple driver"

static int __init init_hello_4(void) {
  printk(KERN_INFO "Hello world 4\n");

  return 0;
}

static void __exit cleanup_hello_4(void) {
  printk(KERN_INFO "Goodbye world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

/* Declaring kernel module settings: */

MODULE_LICENSE("GPL");              /* Declaring code as GPL. */

MODULE_AUTHOR(DRIVER_AUTHOR);       /* Who wrote this module? */

MODULE_DESCRIPTION(DRIVER_DESC);    /* What does this module do */

/*
 * The MODULE_SUPPORTED_DEVICE macro might be used in the future to help
 * automatic configuration of modules, but is currently unused other than
 * for documentation purposes.
 */
MODULE_SUPPORTED_DEVICE("Supported device: devA, devB, etc.");

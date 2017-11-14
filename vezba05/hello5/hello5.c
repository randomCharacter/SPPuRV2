/*
 * hello5.c − Demonstrates command line argument passing to a module.
 */

#include <linux/module.h>       /* Needed by all modules. */
#include <linux/kernel.h>       /* Needed for KERN_INFO. */
#include <linux/init.h>         /* Needed for the macros. */
#include <linux/moduleparam.h>  /* Needed for the arguments. */
#include <linux/stat.h>         /* Needed for the system call. */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RT-RK");
MODULE_DESCRIPTION("Demonstrates command line argument passing to a kernel module");

/* Global variables for kernel module - default initialization. */
static short int myShort     =  1;
static int myInt             =  100;
static long int myLong       =  1000;
static char *myString        =  "initString...";
static int myIntArray[2]     =  { -1, -1 };
static int argc              =  0;

/*
 * module_param(variable_name, data_type, 0000)
 * The first param is the parameters name.
 * The second param is it's data type.
 * The final argument is the permissions bits.
 * (For exposing parameters in sysfs (if non−zero) at a later stage)
 */
module_param(myShort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myShort, "A short integer.");

module_param(myInt, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myInt, "An integer.");

module_param(myLong, long, S_IRUSR);
MODULE_PARM_DESC(myLong, "A long integer.");

module_param(myString, charp, 0000);
MODULE_PARM_DESC(myString, "A character string.");

/*
 * module_param_array(variable_name, data_type, num, perm);
 * The first param is the parameter's (in this case the array's) name.
 * The second param is the data type of the elements of the array.
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time.
 * The fourth argument is the permission bits.
 */
module_param_array(myIntArray, int, &argc, 0000);
MODULE_PARM_DESC(myIntArray, "An array of integers.");

static int __init hello_5_init(void) {

  int i;
  printk(KERN_INFO "Hello world 5\n");
  printk(KERN_INFO "myShort is a short integer: %hd\n", myShort);
  printk(KERN_INFO "myInt is an integer: %d\n", myInt);
  printk(KERN_INFO "myLong is a long integer: %ld\n", myLong);
  printk(KERN_INFO "myString is a string: %s\n", myString);

  for (i = 0; i < (sizeof myIntArray / sizeof (int)); i++) {
    printk(KERN_INFO "myIntArray[%d] = %d\n", i, myIntArray[i]);
  }

  printk(KERN_INFO "Got %d arguments for myIntArray.\n", argc);

  return 0;
}

static void __exit hello_5_exit(void) {
  printk(KERN_INFO "Goodbye world 5\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);

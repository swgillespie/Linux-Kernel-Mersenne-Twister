#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/kern_levels.h>

/*
 * Mersenne Twister Linux Kernel Module
 * By Sean Gillespie - Georgia Institute of Technology
 */

/*
 * The device name for the Mersenne Twister device
 */
#define MERSENNE_DEVICE_NAME "mersenne"

/*
 * The size of the state matrix for the Mersenne twister. This
 * shouldn't ever be changed; I put it here to cut down on the magic
 * numbers.
 */
#define MERSENNE_STATE_MATRIX_SIZE 624

/*
 * The name of the module.
 */
#define MODULE_NAME "Mersenne Twister"

/*
 * Initialize the state of the mersenne twister.
 * @param seed - The integer seed used to initialize the twister
 */
static void mersenne_init(int seed);

/*
 * Generates a random integer and returns it.
 * @returns A random integer.
 */
static int mersenne_random_int(void);

/*
 * Regenerates the state of the mersenne twister after every
 * element of the matrix has been consumed. Called every 624
 * calls to mersenne_random_int.
 */
static void mersenne_generate(void);

/*
 * Fills a buffer with random data.
 * @param buf - A user-provided buffer to copy the data to.
 * @param size - The number of bytes to copy into the buffer.
 */
static int mersenne_fill_buf(char __user *buf, size_t size);

/*
 * Internal state for the mersenne twister.
 */
static int state[MERSENNE_STATE_MATRIX_SIZE];
static int index = 0;

/*
 * These three objects are used in the creation and maintence
 * of the char device. 
 */
static dev_t device_number;
static struct cdev char_device;
static struct class *device_class;

static void mersenne_init(int seed) {
  int i;
  index = 0;
  state[0] = seed;
  for (i = 1; i < MERSENNE_STATE_MATRIX_SIZE; i++) {
    state[i] = 0xFFFFFFFF & (0x6C078965 * (state[i - 1] ^ (state[i - 1] >> 30)) + i);
  }
}

static int mersenne_random_int(void) {
  int out_num;
  if (index == 0) {
    mersenne_generate();
  }
  out_num = state[index];
  out_num ^= (out_num >> 11);
  out_num ^= (out_num << 7) & 0x9D2C5680;
  out_num ^= (out_num << 15) & 0xEFC60000;
  out_num ^= (out_num >> 18);
  index = (index + 1) % MERSENNE_STATE_MATRIX_SIZE;
  return out_num;
}

static void mersenne_generate(void) {
  int i, y;
  for (i = 0; i < MERSENNE_STATE_MATRIX_SIZE - 1; i++) {
    y = (state[i] & 0x80000000) + (state[(i + 1) % MERSENNE_STATE_MATRIX_SIZE] & 0x7FFFFFFF);
    state[i] = state[(i + 397) % MERSENNE_STATE_MATRIX_SIZE] ^ (y >> 1);
    if ((y % 2) != 0) {
      state[i] ^= 0x9908B0DF;
    }
  }
}

static int mersenne_fill_buf(char __user *buf, size_t size) {
  int rand_num;
  int err;
  int buf_offset = 0;
  if (size <= 0) {
    return -EINVAL;
  }
  while(size / 4 > 0) {
    rand_num = mersenne_random_int();
    err = copy_to_user(buf + buf_offset, &rand_num, 4);
    if (err != 0) {
      return -EFAULT;
    }
    size -= 4;
    buf_offset += 4;
  }
  // size is in the range 0 to 3
  if (size != 0) {
    rand_num = mersenne_random_int();
    err = copy_to_user(buf + buf_offset, &rand_num, size); 
    if (err != 0) {
      return -EFAULT;
    }
  }
  return 0;
}

static int mersenne_open(struct inode *node, struct file *file) {
  // vaugely obscure seed. It really doesn't matter since I don't care if this
  // module is cryptographically secure.
  printk(KERN_INFO MODULE_NAME ": Device opened\n");
  return 0;
}

static int mersenne_close(struct inode *node, struct file *file) {
  // nothing to do here, just return
  return 0;
}

static ssize_t mersenne_read(struct file *file, char __user *buf, size_t len, loff_t *off) {
  // seek is not supported for this char device, so off is completely ignored
  int err;
  printk(KERN_INFO MODULE_NAME ": Device read from\n");
  err = mersenne_fill_buf(buf, len);
  if (err < 0) {
    return err;
  }
  return len;
}

static const struct file_operations mersenne_file_ops = {
  .owner    = THIS_MODULE,
  .open     = mersenne_open,
  .release  = mersenne_close,
  .read     = mersenne_read,
  .llseek   = noop_llseek, // not supported
};

static int __init mersenne_module_init(void) {
  printk(KERN_INFO MODULE_NAME ": Module init\n");
  // first, allocate a region of character device numbers, including a major number
  if (alloc_chrdev_region(&device_number, 0, 1, MODULE_NAME) < 0) {
    printk(KERN_ERR MODULE_NAME ": Chrdev allocation failed\n");
    return -ENOMEM;
  }
  // then, create a class of character devices owned by this module
  if ((device_class = class_create(THIS_MODULE, MODULE_NAME)) < 0) {
    printk(KERN_ERR MODULE_NAME ": Device class creation failed\n");
    unregister_chrdev_region(device_number, 1);
    return -ENOMEM; // not sure what kind of error this is
  }
  // then, create the char device using the allocated major number and device class
  if (device_create(device_class, NULL, device_number, NULL, MERSENNE_DEVICE_NAME) < 0) {
    printk(KERN_ERR MODULE_NAME ": Device create failed\n");
    unregister_chrdev_region(device_number, 1);
    class_destroy(device_class);
    return -1;
  }
  // once the device is created, initialize it and associate it with the device major number
  cdev_init(&char_device, &mersenne_file_ops);
  if (cdev_add(&char_device, device_number, 1) < 0) {
    printk(KERN_ERR MODULE_NAME ": cdev_add failed\n");
    device_destroy(device_class, device_number);
    class_destroy(device_class);
    unregister_chrdev_region(device_number, 1);
  }
  // If I really cared about good randomness and cryptographic security,
  // I wouldn't do this. This wasn't my main goal for this project, though;
  // I really just wanted to write a kernel module. Maybe I'll come back to this
  // and seed this correctly later.
  mersenne_init(0xDEADBEEF);
  printk(KERN_INFO MODULE_NAME ": Init success\n");
  return 0;
}

static void __exit mersenne_module_exit(void) {
  // clean up all of the things that were allocated in the module init function
  cdev_del(&char_device);
  device_destroy(device_class, device_number);
  class_destroy(device_class);
  unregister_chrdev_region(device_number, 1);
  printk(KERN_INFO MODULE_NAME ": Module unloading\n");
}

module_init(mersenne_module_init);
module_exit(mersenne_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sean Gillespie, github.com/swgillespie");
MODULE_DESCRIPTION("Mersenne Twister for the Linux Kernel");

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define CLASS_NAME "hello"
#define DEVICE_NAME "hello"

static int major;
static struct cdev hello_cdev;
static struct class *hello_class;
static struct device *hello_device;

static int hello_open (struct inode *, struct file *);
static int hello_release (struct inode *, struct file *);
static ssize_t hello_read (struct file *, char __user *, size_t, loff_t *);
static ssize_t hello_write (struct file *, const char __user *, size_t,
                            loff_t *);

static const struct file_operations hello_fops = {
  .owner = THIS_MODULE,
  .open = hello_open,
  .release = hello_release,
  .read = hello_read,
  .write = hello_write,
};

static int __init
hello_init (void)
{
  int err;
  dev_t dev;

  if ((err = alloc_chrdev_region (&dev, 0, 1, DEVICE_NAME)))
    {
      pr_err ("Failed to allocate device number\n");
      goto return_err;
    }

  major = MAJOR (dev);
  hello_class = class_create (CLASS_NAME);

  if (IS_ERR (hello_class))
    {
      err = PTR_ERR (hello_class);
      goto unregister_chrdev;
    }

  hello_device = device_create (hello_class, NULL, dev, NULL, DEVICE_NAME);

  if (IS_ERR (hello_device))
    {
      err = PTR_ERR (hello_class);
      goto destroy_class;
    }

  cdev_init (&hello_cdev, &hello_fops);
  hello_cdev.owner = THIS_MODULE;

  if ((err = cdev_add (&hello_cdev, dev, 1)))
    goto destroy_device;

  pr_info_once ("Hello module loaded\n");
  return 0;

destroy_device:
  device_destroy (hello_class, dev);

destroy_class:
  class_destroy (hello_class);

unregister_chrdev:
  unregister_chrdev_region (dev, 1);

return_err:
  return err;
}

static void __exit
hello_exit (void)
{
  dev_t dev = MKDEV (major, 0);

  cdev_del (&hello_cdev);
  device_destroy (hello_class, dev);
  class_destroy (hello_class);
  unregister_chrdev_region (dev, 1);

  pr_info ("Hello module unloaded\n");
}

static int
hello_open (struct inode *inode, struct file *filp)
{
  pr_info ("Hello device opened\n");
  return 0;
}

static int
hello_release (struct inode *inode, struct file *filp)
{
  pr_info ("Hello device closed\n");
  return 0;
}

static ssize_t
hello_read (struct file *filp, char __user *buff, size_t size, loff_t *offp)
{
  char str[] = "hello";
  size_t len = sizeof (str) - 1;

  if (*offp >= len)
    return 0;

  if (size > len - *offp)
    size = len - *offp;

  if (copy_to_user (buff, str + *offp, size))
    return -EFAULT;

  *offp += size;
  return size;
}

static ssize_t
hello_write (struct file *filp, const char __user *buff, size_t size,
             loff_t *offp)
{
  pr_alert ("This operation is not supported\n");
  return -EINVAL;
}

module_init (hello_init);
module_exit (hello_exit);

MODULE_AUTHOR ("Arthur");
MODULE_VERSION ("0.0.1");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("A simple module");

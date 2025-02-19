#include <linux/module.h>
#include <linux/proc_fs.h>

#define DATA_MAX 1024
#define PROC_NAME "hello"

static char data[DATA_MAX];
static size_t data_size = 0;
static struct proc_dir_entry *hello_proc;

static ssize_t hello_read (struct file *filp, char __user *buff, size_t size,
                           loff_t *offp);
static ssize_t hello_write (struct file *filp, const char __user *buff,
                            size_t size, loff_t *offp);

static const struct proc_ops hello_fops = {
  .proc_read = hello_read,
  .proc_write = hello_write,
};

static int __init
hello_init (void)

{
  hello_proc = proc_create (PROC_NAME, 0644, NULL, &hello_fops);

  if (hello_proc == NULL)
    {
      pr_alert ("could not initialize /proc/%s\n", PROC_NAME);
      return -ENOMEM;
    }

  pr_info ("/proc/%s created\n", PROC_NAME);
  return 0;
}

static void __exit
hello_exit (void)
{
  proc_remove (hello_proc);
  pr_info ("/proc/%s removed\n", PROC_NAME);
}

static ssize_t
hello_read (struct file *filp, char __user *buff, size_t size, loff_t *offp)

{
  ssize_t ret;
  char str[] = "hello";
  size = min (size - 1, sizeof (str));

  if (*offp >= size || copy_to_user (buff, str, size))
    {
      pr_info ("copy_to_user failed\n");
      ret = 0;
    }
  else
    {
      pr_info ("procfile read %s\n", filp->f_path.dentry->d_name.name);
      *offp += size;
      ret = size;
    }

  return ret;
}

static ssize_t
hello_write (struct file *filp, const char __user *buff, size_t size,
             loff_t *offp)
{
  size = min (size, DATA_MAX - 1);

  if (copy_from_user (data, buff, size))
    return -EFAULT;
  data[size] = 0;

  *offp += size;
  data_size = size;
  pr_info ("procfile write %s\n", data);

  return size;
}

module_init (hello_init);
module_exit (hello_exit);

MODULE_AUTHOR ("Arthur");
MODULE_VERSION ("0.0.1");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("A simple module");

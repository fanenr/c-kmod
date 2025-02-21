#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_NAME "hello"

static int hello_open (struct inode *inode, struct file *filp);
static void *hello_start (struct seq_file *seq, loff_t *offp);
static void *hello_next (struct seq_file *seq, void *v, loff_t *offp);
static int hello_show (struct seq_file *seq, void *ptr);
static void hello_stop (struct seq_file *seq, void *ptr);

static const struct proc_ops hello_proc_ops = {
  .proc_open = hello_open,
  .proc_read = seq_read,
  .proc_lseek = seq_lseek,
  .proc_release = seq_release,
};

static struct seq_operations hello_seq_ops = {
  .start = hello_start,
  .next = hello_next,
  .show = hello_show,
  .stop = hello_stop,
};

static int __init
hello_init (void)
{
  struct proc_dir_entry *entry;
  entry = proc_create (PROC_NAME, 0, NULL, &hello_proc_ops);

  if (entry == NULL)
    {
      pr_alert ("could not initialize /proc/%s\n", PROC_NAME);
      return -ENOMEM;
    }

  return 0;
}

static void __exit
hello_exit (void)
{
  remove_proc_entry (PROC_NAME, NULL);
  pr_info ("/proc/%s removed\n", PROC_NAME);
}

static int
hello_open (struct inode *inode, struct file *filp)
{
  return seq_open (filp, &hello_seq_ops);
};

static void *
hello_start (struct seq_file *seq, loff_t *offp)
{
  static size_t counter = 0;

  if (*offp == 0)
    return &counter;

  *offp = 0;
  return NULL;
}

static void *
hello_next (struct seq_file *seq, void *ptr, loff_t *offp)
{
  size_t *counter = ptr;

  (*counter)++;
  (*offp)++;

  return NULL;
}

static int
hello_show (struct seq_file *seq, void *ptr)
{
  size_t *counter = ptr;

  seq_printf (seq, "%zu\n", *counter);

  return 0;
}

static void
hello_stop (struct seq_file *seq, void *ptr)
{
}

module_init (hello_init);
module_exit (hello_exit);

MODULE_AUTHOR ("Arthur");
MODULE_VERSION ("0.0.1");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("A simple module");

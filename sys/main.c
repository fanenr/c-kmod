#include <linux/module.h>

static int myvariable = 0;
static struct kobject *mymodule;

static ssize_t
myvariable_show (struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return sprintf (buf, "%d\n", myvariable);
}

static ssize_t
myvariable_store (struct kobject *kobj, struct kobj_attribute *attr,
                  const char *buf, size_t count)
{
  sscanf (buf, "%d", &myvariable);
  return count;
}

static struct kobj_attribute myvariable_attribute
    = __ATTR (myvariable, 0660, myvariable_show, myvariable_store);

static int __init
mymodule_init (void)
{
  int error = 0;

  mymodule = kobject_create_and_add ("mymodule", kernel_kobj);

  if (!mymodule)
    return -ENOMEM;

  error = sysfs_create_file (mymodule, &myvariable_attribute.attr);

  if (error)
    {
      kobject_put (mymodule);
      pr_info ("failed to create the myvariable file "
               "in /sys/kernel/mymodule\n");
    }

  pr_info ("mymodule: initialized\n");

  return error;
}

static void __exit
mymodule_exit (void)
{
  pr_info ("mymodule: Exit success\n");
  kobject_put (mymodule);
}

module_init (mymodule_init);
module_exit (mymodule_exit);

MODULE_LICENSE ("GPL");

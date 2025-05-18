#include <linux/module.h>

#define KOBJ_NAME "hello"

static int var = 0;
static struct kobject *kobj;
static const char *KOBJ_PATH = KOBJ_NAME "/" __stringify (var);

static ssize_t var_show (struct kobject *kobj, struct kobj_attribute *attr,
			 char *buff);
static ssize_t var_store (struct kobject *kobj, struct kobj_attribute *attr,
			  const char *buff, size_t count);

static struct kobj_attribute var_attr
    = __ATTR (var, 0660, var_show, var_store);

static int __init
kobj_init (void)
{
  if (!(kobj = kobject_create_and_add (KOBJ_NAME, kernel_kobj)))
    return -ENOMEM;

  int error;
  if ((error = sysfs_create_file (kobj, &var_attr.attr)))
    {
      kobject_put (kobj);
      pr_info ("failed to create the /sys/kernel/%s", KOBJ_PATH);
      return error;
    }

  pr_info ("/sys/kernel/%s created\n", KOBJ_PATH);
  return 0;
}

static void __exit
kobj_exit (void)
{
  pr_info ("/sys/kernel/%s removed\n", KOBJ_PATH);
  kobject_put (kobj);
}

static ssize_t
var_show (struct kobject *kobj, struct kobj_attribute *attr, char *buff)
{
  return sprintf (buff, "%d\n", var);
}

static ssize_t
var_store (struct kobject *kobj, struct kobj_attribute *attr, const char *buff,
	   size_t count)
{
  sscanf (buff, "%d", &var);
  return count;
}

module_init (kobj_init);
module_exit (kobj_exit);

MODULE_AUTHOR ("Arthur");
MODULE_VERSION ("0.0.1");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("A simple module");

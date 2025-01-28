#include "util.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static char *langs[8] = { "C", "C++" };
static char *name = "Arthur";
static uint langn = 2;
static uint age = 20;

module_param (age, uint, 0644);
MODULE_PARM_DESC (age, "Age");

module_param (name, charp, 0644);
MODULE_PARM_DESC (name, "Name");

module_param_array (langs, charp, &langn, 0644);
MODULE_PARM_DESC (langs, "Languages");

static int __init
hello_init (void)
{
  pr_info ("Hello\n");

  pr_info ("age: %d\n", age);
  pr_info ("name: %s\n", name);

  char buff[256];
  strarr (buff, langs, sizeof (buff), langn);
  pr_info ("langs: [%s]\n", buff);

  return 0;
}

static void __exit
hello_exit (void)
{
  pr_info ("Goodbye\n");
}

module_init (hello_init);
module_exit (hello_exit);

MODULE_AUTHOR ("Arthur");
MODULE_VERSION ("0.0.1");
MODULE_LICENSE ("GPL v2");
MODULE_DESCRIPTION ("A simple module");

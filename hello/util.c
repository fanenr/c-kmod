#include "util.h"

#include <linux/module.h>

void
hello_strarr (char *dst, char **arr, size_t size, size_t n)
{
  for (size_t i = 0, len = 0; i < n; i++)
    {
      char *sep = i ? ", " : "";
      len += scnprintf (dst + len, size - len, "%s%s", sep, arr[i]);
    }
}

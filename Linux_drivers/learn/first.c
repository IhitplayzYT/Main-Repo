#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int initfxn() {

  printk("Hello world from the kernel");
  return 0;
}

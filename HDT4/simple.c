#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>

int simple_init(void){
	printk(KERN_INFO "Loading Module\n Algo");
	return 0;
}

void simple_exit(void){
	printk(KERN_INFO "Removing Module\n Algo mas");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ejercicioNo2");
MODULE_AUTHOR("Jose Block");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
dev_t devnum;

static struct file_operations myfops = {
	.owner = THIS_MODULE,
	.read = lectura,
	.write = escritura,
};
static int __init hello_init(void) {
	cdev_init(mycdev, myfops);
	alloc_chrdev_region(&devnum, minor, coun, myname);
	cdev_add(&mycdev, &devnum, count);
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
return 0;
}
static void __exit hello_exit(void) {
	printk(KERN_ALERT "Adios, mundo cruel...\n");
	//unregister_chrdev_region();
	//cdev_del
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de ’Hola, mundo’");

///INODO
static struct class *mi_class;
mi_class = class_create(THIS_MODULE, DEVICE_NAME);
device_create(mi_class, NULL, mi_devno, NULL, DEVICE_NAME);
device_destroy(mi_class, mi_devno);
class_destroy(mi_class);
///


ssize_t lectura(struct file *filp, char __user *data, size_t s, loff_t *off)
{
	return 0;
}

ssize_t escritura(struct file * filp, const char __user * data, size_t s, loff_t *off)	
{
	return (ssize_t) s;
}

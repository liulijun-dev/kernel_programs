#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/printk.h>
#include<linux/kdev_t.h>
#include<asm/string.h>
#include<linux/slab.h>
#include<linux/device.h>
#include "memdev.h"

MODULE_PARM_DESC(memdev_name, "memdev");

static  int memdev_major = MEMDEV_MAJOR;
module_param(memdev_major, int, S_IRUGO);

struct mem_dev *memdevp;
struct cdev mcdev;

int mem_open(struct inode *inode, struct file *filp){
    struct mem_dev *dev;
    
	//get minor device number
	int num = MINOR(inode->i_rdev);
	if(num > MEMDEV_NR_DEVS)
	  return -ENODEV;

	dev = &memdevp[num];

	filp->private_data = dev;

	return 0;
}

int mem_release(struct inode *inode, struct file *filp){
    return 0;
}

ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos){
    unsigned long p = *ppos; //record file pointer offset
	unsigned int count = size;	//record read byte count
	int ret = 0;	//return value
    struct mem_dev *dev = filp->private_data;

	//if offset great than device memory size
    if(p > MEMDEV_SIZE){
	    return 0;
	}

	//if count great than device memory size
	if(count > MEMDEV_SIZE-p){
	    count = MEMDEV_SIZE - p;
	}

	if(copy_to_user(buf, dev->data+p, count)){
		ret = -EFAULT;
	}else{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %ud byte(s) from %ld\n", count, p);
	}

	return ret;
}

ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos){
    unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct mem_dev *dev = filp->private_data;
        printk(KERN_INFO"%s %d", buf, size);
	if(p >= MEMDEV_SIZE){
		return 0;
	}
	if(count > MEMDEV_SIZE - p){
		count = MEMDEV_SIZE - p;
	}
	
	if(copy_from_user(dev->data+p, buf, count)){
		ret = -EFAULT;
	}else{
		*ppos += count;
		ret = count;
	}

	return ret;

}

static loff_t mem_llseek(struct file *filp, loff_t offset, int whence){
    loff_t newpos;
	switch(whence){
    case SEEK_SET:
		newpos = offset;
		break;
    case SEEK_CUR:
		newpos = filp->f_pos + offset;
		break;
	case SEEK_END:
		newpos = MEMDEV_SIZE - 1 - offset;
		break;
	default:
		return -EINVAL;
	}

	if(newpos<0 || newpos>MEMDEV_SIZE){
        return -EINVAL;
	}

	filp->f_pos = newpos;

	return newpos;
}

static const struct file_operations mem_fops = {
    .owner = THIS_MODULE,
	.llseek = mem_llseek,
	.read = mem_read,
	.write = mem_write,
	.open = mem_open,
	.release = mem_release
};

static int __init memdev_init(void){
	int result;
	int i;
	dev_t devno = MKDEV(memdev_major, 0);
        if(memdev_major){
            result = register_chrdev_region(devno, 2, "memdev");
        }else{
		result = alloc_chrdev_region(&devno, 0, 2, "memdev");
		memdev_major = MAJOR(devno);
	}
	
	if(result < 0)
	  return result;
    
	cdev_init(&mcdev, &mem_fops);
	mcdev.owner = THIS_MODULE;

	cdev_add(&mcdev, MKDEV(memdev_major, 0), MEMDEV_NR_DEVS);

	//alloc memory for device struct
	memdevp = kmalloc(MEMDEV_NR_DEVS*sizeof(struct mem_dev), GFP_KERNEL);

	if(!memdevp){
        result = -ENOMEM;
		goto fail_malloc;
	}

	memset(memdevp, 0, MEMDEV_NR_DEVS*sizeof(struct mem_dev));

    for(i=0; i<MEMDEV_NR_DEVS; i++){
		memdevp[i].size = MEMDEV_SIZE;
		memdevp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
		memset(memdevp[i].data, 0, MEMDEV_SIZE);
	}

	return 0;

fail_malloc:
	unregister_chrdev_region(MKDEV(memdev_major, 0), MEMDEV_NR_DEVS);

	return result;
}

static void __exit memdev_exit(void){
    cdev_del(&mcdev);
	kfree(memdevp);
	unregister_chrdev_region(MKDEV(memdev_major,0), MEMDEV_NR_DEVS);
}

static ssize_t memdev_show(struct device_driver *driver, char *buf){
    return 0;
}

static ssize_t memdev_store(struct device_driver *driver, const char *buf, size_t count){
    return 0;
}
MODULE_AUTHOR("liulijun");
MODULE_LICENSE("GPL");
module_init(memdev_init);
module_exit(memdev_exit);
DRIVER_ATTR(memdev_name, S_IRUGO|S_IWUSR, memdev_show, memdev_store);

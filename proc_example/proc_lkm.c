#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm-generic/uaccess.h>

#define MAX_COOKIE_LENGTH PAGE_SIZE
static struct proc_dir_entry *proc_entry;
static char *cookie_pot; //space for fortune string
static int cookie_index; //index to write next fortue
static int next_fortune;	//index to read next fortune

int fortune_write(struct file *filp, const char __user *buf, unsigned long count, void *data){
    int space_available = (MAX_COOKIE_LENGTH - cookie_index) - 1;
	if(count > space_available){
		printk(KERN_INFO"fortune: cookie pot is full!\n");
		return -ENOMEM;
	}
	if(copy_from_user(&cookie_pot[cookie_index], buf, count)){
		return -EFAULT;
	}
	cookie_index += count;
	cookie_pot[cookie_index] = 0;

	return count;
}

int fortune_read(char *page, char **start, off_t off, int count, int *eof, void *data){
    int len;
	if(off > 0){
		*eof = 1;
		return 0;
	}

	if(next_fortune >= cookie_index) next_fortune = 0;
	len = sprintf(page, "%s\n", &cookie_pot[next_fortune]);
	next_fortune += len;

	return len;
}

static int __init init_fortune_module(){
    int ret = 0;
	cookie_pot = (char *)vmalloc(MAX_COOKIE_LENGTH);
	if(!cookie_pot){
	  printk(KERN_INFO"fortune: couldn't create cookie_pot");
	  ret = -ENOMEM;
	}else{
        memset(cookie_pot, 0, MAX_COOKIE_LENGTH);
        proc_entry = create_proc_entry("fortune", 0644, NULL);
		if(proc_entry == NULL){
			ret = -ENOMEM;
			vfree(cookie_pot);
			printk(KERN_INFO"fortune: couldn't create proc entry\n");
		}else{
			cookie_index = 0;
			next_fortune = 0;
			proc_entry->read_proc = fortune_read;
			proc_entry->write_proc = fortune_write;
			printk(KERN_INFO"fortune: module loaded\n");
		}
	}
   
	return ret;
}

static void __exit clean_fortune_module(){
    remove_proc_entry("fortune", proc_entry->parent);
	vfree(cookie_pot);
	printk(KERN_INFO"fortune: module unloaded\n");
}

module_init(init_fortune_module);
module_exit(clean_fortune_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liulijun");
MODULE_DESCRIPTION("proc example");


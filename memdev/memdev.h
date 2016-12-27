#ifndef _MEDEV_H
#define _MEDEV_H

#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 300	//major device number
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 2	//device number
#endif

#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif

/**
 *mem device struct
 */
struct mem_dev{
    char *data;
	unsigned long size;
};

#endif /*_MEMDEV_H*/

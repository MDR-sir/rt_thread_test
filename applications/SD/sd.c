#include <rtthread.h>
#include <dfs_fs.h>
#include <dfs_elm.h>
#include "sd.h"


rt_mq_t  fs_mq;
static rt_uint8_t msg_pool[FS_BUFSIZE];



int sd_init()
{
		int fd,size;
		static char buffer[80];

		rt_thread_mdelay(100);
//		if(dfs_mkfs("elm", "sd")==0)
//				rt_kprintf("File System mkfs\n");
    if(dfs_mount("sd","/","elm",0,0)==0)
        rt_kprintf("File System initialized\n");
    else
        rt_kprintf("File System init failed\n");
		
		fd = open("/a.txt", O_RDONLY);
    if (fd>= 0)
    {
				rt_kprintf("open file successfully\n");
        size = read(fd, buffer, sizeof(buffer));
        close(fd);
        rt_kprintf("Read from file a.txt : %s \n", buffer);
    }
		else
				rt_kprintf("open file unsuccessfully\n");
		
		/* 初始化一个 messagequeue */
	
		fs_mq = rt_mq_create("fs_msg", FS_BUFSIZE,
           1, RT_IPC_FLAG_FIFO);
		
		rt_mq_send(fs_mq, buffer, size);
		
		
}
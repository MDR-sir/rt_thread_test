#include <rtthread.h>
#include <dfs_fs.h>
#include <dfs_elm.h>
#include "sd.h"


int sd_test()
{
		int fd,size;
		char buffer[80];

	
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
}
MSH_CMD_EXPORT(sd_test, a test of sd)
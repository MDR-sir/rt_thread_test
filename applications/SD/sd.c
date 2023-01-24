#include <rtthread.h>
#include <dfs_fs.h>
#include <dfs_elm.h>
#include "sd.h"


int sd_test()
{
		rt_thread_mdelay(1000);
    dfs_init();
    elm_init();
    if(dfs_mount("sd0","/","elm",0,0)==0)
        rt_kprintf("successfully\n");
    else
        rt_kprintf("Error!\n");
}
MSH_CMD_EXPORT(sd_test, a test of sd)
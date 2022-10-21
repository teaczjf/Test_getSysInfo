#include <iostream>
#include <string>
#include <string.h>
#include "./feature/easylogging/my_easylogging.h"
#include "./feature/getSysInfo/getSysInfo.h"

int main ( int argc, char *argv[] )
{


#if 1
    easylogginginit ( 1, 1 );
    string program_name = "SunloginClient.exe";
    float cpu_usage_ratio = 0;
    float memory_usage = 0;
    UINT32 pid = 0;
    while ( true ) {
        pid = GetProcessIDByName ( program_name );
        LOG ( INFO ) << " ============================ " ;
        UINT32 ram_value  = getWin_MemUsage();
        UINT32 CpuUsage_value = getWin_CpuUsage();//这个函数耗时1S
        LOG ( INFO ) << "SYS CpuUsage == " << CpuUsage_value << "% " << "SYS RAM == " << ram_value << "%";
        if ( pid > 0 ) {
            cpu_usage_ratio = GetCpuUsageRatio ( pid );
            memory_usage = GetMemoryUsage ( pid );
            LOG ( INFO ) << program_name << " current pid: " << pid ;
            LOG ( INFO ) << program_name << " cpu usage ratio: " << cpu_usage_ratio * 100 << "%" ;
            LOG ( INFO ) << program_name << " memory usage: " << memory_usage << "MB" ;
        } else {
            LOG ( INFO )  << program_name << "current pid: " << pid ;
            LOG ( INFO )  << program_name << "cpu usage ratio: " << cpu_usage_ratio * 100 << "%" ;
            LOG ( INFO )  << program_name << "memory usage: " << memory_usage << "MB" ;
        }
        get_AllHardDiskMemory();
        cpu_usage_ratio = 0;
        memory_usage = 0;
        pid = 0;
        int ret;
        string cmd_str = "echo 1111111111111111111";
        LOG ( INFO ) << "rte_str == " << getCmdResult ( cmd_str );
        Sleep ( 100 );

    }
#endif
    return 0;
}


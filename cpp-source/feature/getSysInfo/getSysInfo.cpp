#include "getSysInfo.h"
#include <iostream>
#include <windows.h>
#include <stdint.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "./../easylogging/my_easylogging.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
    #include <psapi.h>
    //#include <tlhelp32.h>
    #include <direct.h>
    #include <process.h>
#else
    #include <sys/stat.h>
    #include <sys/sysinfo.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif
using namespace std;
//TCHAR 转换 string
string TCHAR2STRING ( TCHAR * STR )
{
    int iLen = WideCharToMultiByte ( CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL ); //首先计算TCHAR 长度。
    char* chRtn = new char[iLen * sizeof ( char )]; //定义一个 TCHAR 长度大小的 CHAR 类型。
    WideCharToMultiByte ( CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL ); //将TCHAR 类型的数据转换为 CHAR 类型。
    std::string str ( chRtn ); //最后将CHAR 类型数据 转换为 STRING 类型数据。
    return str;
}
// std:wstring 转 LPWSTR
LPWSTR ws2LPWSTR ( const std::wstring& ws )
{
    return const_cast<LPWSTR> ( ws.c_str() );
}

std::wstring CharToWchar ( const char* c, size_t m_encode = CP_ACP )
{
    std::wstring str;
    int len = MultiByteToWideChar ( m_encode, 0, c, strlen ( c ), NULL, 0 );
    wchar_t*	m_wchar = new wchar_t[len + 1];
    MultiByteToWideChar ( m_encode, 0, c, strlen ( c ), m_wchar, len );
    m_wchar[len] = '\0';
    str = m_wchar;
    delete m_wchar;
    return str;
}
// std::string 转 wstring
wstring s2ws ( const std::string& s )
{
    int len;
    int slength = ( int ) s.length() + 1;
    len = MultiByteToWideChar ( CP_ACP, 0, s.c_str(), slength, 0, 0 );
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar ( CP_ACP, 0, s.c_str(), slength, buf, len );
    std::wstring r ( buf );
    delete[] buf;
    return r;
}



//windows 内存 使用率
UINT32 getWin_MemUsage()
{
    MEMORYSTATUS ms;
    ::GlobalMemoryStatus ( &ms );
    return ms.dwMemoryLoad;
}


__int64 CompareFileTime ( FILETIME time1, FILETIME time2 )
{
    __int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
    __int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

    return ( b - a );
}

//WIN CPU使用情况
INT32 getWin_CpuUsage() //比任务管理器中看到的要低很多
{
    HANDLE hEvent;
    BOOL res;
    FILETIME preidleTime;
    FILETIME prekernelTime;
    FILETIME preuserTime;
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    res = GetSystemTimes ( &idleTime, &kernelTime, &userTime );
    preidleTime = idleTime;
    prekernelTime = kernelTime;
    preuserTime = userTime;

    hEvent = CreateEventA ( NULL, FALSE, FALSE, NULL ); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled

    //    while ( true ) {
    WaitForSingleObject ( hEvent, 1000 );//延时等待1S
    res = GetSystemTimes ( &idleTime, &kernelTime, &userTime );

    __int64 idle = CompareFileTime ( preidleTime, idleTime );
    __int64 kernel = CompareFileTime ( prekernelTime, kernelTime );
    __int64 user = CompareFileTime ( preuserTime, userTime );

    INT32 cpu = ( kernel + user - idle ) * 100 / ( kernel + user );
    INT32 cpuidle = ( idle ) * 100 / ( kernel + user );
    //    cout << "CPU Usage:" << cpu << "%" << " CPU Free:" << cpuidle << "%" << endl;

    preidleTime = idleTime;
    prekernelTime = kernelTime;
    preuserTime = userTime;
    //    }

    return cpu;
}

std::string WcharToChar ( const wchar_t* wp, size_t m_encode = CP_ACP )
{
    std::string str;
    int len = WideCharToMultiByte ( m_encode, 0, wp, wcslen ( wp ), NULL, 0, NULL, NULL );
    char	*m_char = new char[len + 1];
    WideCharToMultiByte ( m_encode, 0, wp, wcslen ( wp ), m_char, len, NULL, NULL );
    m_char[len] = 0x00;
    str = m_char;
    delete m_char;
    return str;
}


typedef struct EnumHWndsArg {
    std::vector<HWND> *vecHWnds;
    DWORD dwProcessId;
} EnumHWndsArg, *LPEnumHWndsArg;

HANDLE GetProcessHandleByID ( int nID ) //通过进程ID获取进程句柄
{
    return OpenProcess ( PROCESS_ALL_ACCESS, FALSE, nID );
}

UINT32 GetProcessIDByName ( string pName )
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );
    if ( INVALID_HANDLE_VALUE == hSnapshot ) {
        return NULL;
    }
    PROCESSENTRY32 pe = { sizeof ( pe ) };
    for ( BOOL ret = Process32First ( hSnapshot, &pe ); ret; ret = Process32Next ( hSnapshot, &pe ) ) {
        string str = WcharToChar ( pe.szExeFile );
        if ( pName == str ) {
//            LOG ( INFO ) << "pName ID == " << pe.th32ProcessID  << "  pe.szExeFile == " <<  pe.szExeFile << " str == " << str;
            CloseHandle ( hSnapshot );
            return pe.th32ProcessID;
        }

    }
    CloseHandle ( hSnapshot );
    return 0;
}

BOOL CALLBACK lpEnumFunc ( HWND hwnd, LPARAM lParam )
{
    EnumHWndsArg *pArg = ( LPEnumHWndsArg ) lParam;
    DWORD  processId;
    GetWindowThreadProcessId ( hwnd, &processId );
    if ( processId == pArg->dwProcessId ) {
        pArg->vecHWnds->push_back ( hwnd );
        //printf("%p ", hwnd);
    }
    return TRUE;
}

void GetHWndsByProcessID ( DWORD processID, std::vector<HWND> &vecHWnds )
{
    EnumHWndsArg wi;
    wi.dwProcessId = processID;
    wi.vecHWnds = &vecHWnds;
    EnumWindows ( lpEnumFunc, ( LPARAM ) &wi );
}




// get current process pid
inline int GetCurrentPid()
{
    return getpid();
}

// get specific process cpu occupation ratio by pid
#ifdef WIN32
//
static uint64_t convert_time_format ( const FILETIME* ftime )
{
    LARGE_INTEGER li;

    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}
#else
// FIXME: can also get cpu and mem status from popen cmd
// the info line num in /proc/{pid}/status file
#define VMRSS_LINE 22
#define PROCESS_ITEM 14

static const char* get_items ( const char* buffer, unsigned int item )
{
    // read from buffer by offset
    const char* p = buffer;

    int len = strlen ( buffer );
    int count = 0;

    for ( int i = 0; i < len; i++ ) {
        if ( ' ' == *p ) {
            count++;
            if ( count == item - 1 ) {
                p++;
                break;
            }
        }
        p++;
    }

    return p;
}

static inline unsigned long get_cpu_total_occupy()
{
    // get total cpu use time

    // different mode cpu occupy time
    unsigned long user_time;
    unsigned long nice_time;
    unsigned long system_time;
    unsigned long idle_time;

    FILE* fd;
    char buff[1024] = { 0 };

    fd = fopen ( "/proc/stat", "r" );
    if ( nullptr == fd ) {
        return 0;
    }

    fgets ( buff, sizeof ( buff ), fd );
    char name[64] = { 0 };
    sscanf ( buff, "%s %ld %ld %ld %ld", name, &user_time, &nice_time, &system_time, &idle_time );
    fclose ( fd );

    return ( user_time + nice_time + system_time + idle_time );
}

static inline unsigned long get_cpu_proc_occupy ( int pid )
{
    // get specific pid cpu use time
    unsigned int tmp_pid;
    unsigned long utime;  // user time
    unsigned long stime;  // kernel time
    unsigned long cutime; // all user time
    unsigned long cstime; // all dead time

    char file_name[64] = { 0 };
    FILE* fd;
    char line_buff[1024] = { 0 };
    sprintf ( file_name, "/proc/%d/stat", pid );

    fd = fopen ( file_name, "r" );
    if ( nullptr == fd ) {
        return 0;
    }

    fgets ( line_buff, sizeof ( line_buff ), fd );

    sscanf ( line_buff, "%u", &tmp_pid );
    const char* q = get_items ( line_buff, PROCESS_ITEM );
    sscanf ( q, "%ld %ld %ld %ld", &utime, &stime, &cutime, &cstime );
    fclose ( fd );

    return ( utime + stime + cutime + cstime );
}
#endif

float GetCpuUsageRatio ( int pid )
{
#ifdef WIN32
    static int64_t last_time = 0;
    static int64_t last_system_time = 0;

    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t time;
    int64_t system_time_delta;
    int64_t time_delta;

    // get cpu num
    SYSTEM_INFO info;
    GetSystemInfo ( &info );
    int cpu_num = info.dwNumberOfProcessors;

    float cpu_ratio = 0.0;

    // get process hanlde by pid
    HANDLE process = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, pid );
    // use GetCurrentProcess() can get current process and no need to close handle

    // get now time
    GetSystemTimeAsFileTime ( &now );

    if ( !GetProcessTimes ( process, &creation_time, &exit_time, &kernel_time, &user_time ) ) {
        // We don't assert here because in some cases (such as in the Task Manager)
        // we may call this function on a process that has just exited but we have
        // not yet received the notification.
        printf ( "GetCpuUsageRatio GetProcessTimes failed\n" );
        return 0.0;
    }

    // should handle the multiple cpu num
    system_time = ( convert_time_format ( &kernel_time ) + convert_time_format ( &user_time ) ) / cpu_num;
    time = convert_time_format ( &now );

    if ( ( last_system_time == 0 ) || ( last_time == 0 ) ) {
        // First call, just set the last values.
        last_system_time = system_time;
        last_time = time;
        return 0.0;
    }

    system_time_delta = system_time - last_system_time;
    time_delta = time - last_time;

    CloseHandle ( process );

    if ( time_delta == 0 ) {
        printf ( "GetCpuUsageRatio time_delta is 0, error\n" );
        return 0.0;
    }

    // We add time_delta / 2 so the result is rounded.
    cpu_ratio = ( int ) ( ( system_time_delta * 100 + time_delta / 2 ) / time_delta ); // the % unit
    last_system_time = system_time;
    last_time = time;

    cpu_ratio /= 100.0; // convert to float number
    if ( cpu_ratio < 0 ) {
        cpu_ratio == 0;
    }

    return cpu_ratio;
#else
    unsigned long totalcputime1, totalcputime2;
    unsigned long procputime1, procputime2;

    totalcputime1 = get_cpu_total_occupy();
    procputime1 = get_cpu_proc_occupy ( pid );

    // FIXME: the 200ms is a magic number, works well
    usleep ( 200000 ); // sleep 200ms to fetch two time point cpu usage snapshots sample for later calculation

    totalcputime2 = get_cpu_total_occupy();
    procputime2 = get_cpu_proc_occupy ( pid );

    float pcpu = 0.0;
    if ( 0 != totalcputime2 - totalcputime1 ) {
        pcpu = ( procputime2 - procputime1 ) / float ( totalcputime2 - totalcputime1 );    // float number
    }

    int cpu_num = get_nprocs();
    pcpu *= cpu_num; // should multiply cpu num in multiple cpu machine

    return pcpu;
#endif
}

// get specific process physical memeory occupation size by pid (MB)
float GetMemoryUsage ( int pid )
{
#ifdef WIN32
    uint64_t mem = 0, vmem = 0;
    PROCESS_MEMORY_COUNTERS pmc;

    // get process hanlde by pid
    HANDLE process = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, pid );
    if ( GetProcessMemoryInfo ( process, &pmc, sizeof ( pmc ) ) ) {
        mem = pmc.WorkingSetSize; //就是程序在当前代码处的内存使用量。
        vmem = pmc.PagefileUsage;
    }
    CloseHandle ( process );

    // use GetCurrentProcess() can get current process and no need to close handle
//    LOG ( INFO ) << "mem == " << mem / 1024.0 / 1024.0  << "MB  "
//                 << "vmem ==  " <<  vmem / 1024.0 / 1024.0  << "MB  " ;

    // convert mem from B to MB
    return mem / 1024.0 / 1024.0;

#else
    char file_name[64] = { 0 };
    FILE* fd;
    char line_buff[512] = { 0 };
    sprintf ( file_name, "/proc/%d/status", pid );

    fd = fopen ( file_name, "r" );
    if ( nullptr == fd ) {
        return 0;
    }

    char name[64];
    int vmrss = 0;
    for ( int i = 0; i < VMRSS_LINE - 1; i++ ) {
        fgets ( line_buff, sizeof ( line_buff ), fd );
    }

    fgets ( line_buff, sizeof ( line_buff ), fd );
    sscanf ( line_buff, "%s %d", name, &vmrss );
    fclose ( fd );

    // cnvert VmRSS from KB to MB
    return vmrss / 1024.0;
#endif
}




//执行命令 但是不可以获取到返回值
int RunCmd ( const std::string& cmd, int& code, std::string& out )
{
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;
    sa.nLength = sizeof ( SECURITY_ATTRIBUTES );
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if ( !CreatePipe ( &hRead, &hWrite, &sa, 0 ) ) {
        DWORD ret = GetLastError();
        return ret ? ret : -1;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory ( &si, sizeof ( STARTUPINFO ) );

    si.cb = sizeof ( STARTUPINFO );
    GetStartupInfo ( &si );
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
//    wstring wstr =  CharToWchar ( cmd.data() );
//    char cmdline[200] = { 0 };
//    sprintf ( cmdline, "%s", cmd.c_str() );
    if ( !CreateProcess ( NULL, ws2LPWSTR ( s2ws ( cmd ) ), NULL, NULL, TRUE, NULL,
                          NULL, NULL, &si, &pi ) ) {
        DWORD ret = GetLastError();
        CloseHandle ( hRead );
        CloseHandle ( hWrite );
        return ret ? ret : -1;
    }

    CloseHandle ( hWrite );
    char buffer[4096] = { 0 };
    DWORD bytesRead;
    while ( true ) {
        if ( !ReadFile ( hRead, buffer, 4095, &bytesRead, NULL ) ) {
            break;
        }
        out.append ( buffer, bytesRead );
        Sleep ( 100 );
    }

    DWORD exitCode = 0;
    GetExitCodeProcess ( pi.hProcess, &exitCode );
    code = exitCode;
    CloseHandle ( hRead );
    CloseHandle ( pi.hThread );
    CloseHandle ( pi.hProcess );
    return 0;
}


//执行命令并且可以获取到返回值
string getCmdResult ( const string &strCmd )
{
    char buf[10240] = {0};
    FILE *pf = NULL;

    if ( ( pf = popen ( strCmd.c_str(), "r" ) ) == NULL ) {
        return "";
    }

    string strResult;
    while ( fgets ( buf, sizeof buf, pf ) ) {
        strResult += buf;
    }

    pclose ( pf );

    unsigned int iSize =  strResult.size();
    if ( iSize > 0 && strResult[iSize - 1] == '\n' ) { // linux
        strResult = strResult.substr ( 0, iSize - 1 );
    }

    return strResult;
}

//获取指定硬盘的内存
BOOL get_disk_space ( char driver, __int64& allSpace, __int64& freeSpace )
{
    BOOL ret = false;
    DWORD dwSector;
    DWORD dwBytesInSec;
    DWORD dwCluster;
    DWORD dwFreeCluster;
    string sDriver;
    sDriver += driver;
    sDriver += ":\\";

    ret = GetDiskFreeSpace ( ws2LPWSTR ( s2ws ( sDriver ) ), &dwSector, &dwBytesInSec, &dwFreeCluster, &dwCluster );
    allSpace = 0;
    freeSpace = 0;

    allSpace = dwSector;
    allSpace *= dwBytesInSec;
    allSpace *= dwCluster;

    freeSpace = dwSector;
    freeSpace *= dwBytesInSec;
    freeSpace *= dwFreeCluster;

    return ret;
}
//获取本地全部硬盘的内存大小
void get_AllHardDiskMemory()
{


    DWORD dwDriveStrLen;
    TCHAR wDrivesName[0x100] = {0};
    int DType;
    dwDriveStrLen = sizeof ( wDrivesName );
    GetLogicalDriveStrings ( dwDriveStrLen, wDrivesName );
    LOG ( INFO ) << "  wDrivesName =====" << wDrivesName;
    LOG ( INFO ) << "  dwDriveStrLen =====" << dwDriveStrLen;
    TCHAR *p = ( TCHAR * ) wDrivesName;

    __int64 freeSpace = 0;
    __int64 allSpace = 0;
    string ret ;

    while ( *p ) {
//        //这里还涉及到如果你想要通过Unicode来编码那么需要将tchar 转string。
//        //如果采用多字节字符集编码则可以直接转换
        string str = TCHAR2STRING ( p ) ;
        DType = GetDriveType ( LPTSTR ( p ) );
        if ( DType == DRIVE_FIXED ) {
            get_disk_space ( str.at ( 0 ), allSpace, freeSpace );
            //这里调用get_disk_space（s.at(0), allSpace, freeSpace）方法
            //用获取到的allSpace和freeSpace来计算使用率即可
            LOG ( INFO ) << str << " allSpace ==== " << allSpace / 1024 / 1024 / 1024 << "G"
                         << " freeSpace ==== " << freeSpace / 1024 / 1024 / 1024 << "G"
                         << " Utilization == " << ( 1 - float ( freeSpace ) / float ( allSpace ) ) * 100 << "%";

        }
        p += ( wcslen ( p ) + 1 );
    }
}

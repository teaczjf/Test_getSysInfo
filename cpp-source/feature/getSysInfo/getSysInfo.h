#ifndef _GETSYSINFO_H_
#define _GETSYSINFO_H_
#include "./include/normal_typedef.h"

using namespace std;
UINT32 getWin_MemUsage();
INT32 getWin_CpuUsage();
int GetCpuUsage() ;
int GetSystemCpuUsePrecent();
UINT32 GetProcessIDByName ( string pName );
void GetHWndsByProcessID ( DWORD processID, std::vector<HWND> &vecHWnds );
float GetCpuUsageRatio ( int pid );
float GetMemoryUsage ( int pid );
int RunCmd ( const std::string& cmd, int& code, std::string& out );
string getCmdResult ( const string &strCmd )  ;

wstring s2ws ( const std::string& s );// std::string 转 wstring
LPWSTR ws2LPWSTR ( const std::wstring& ws );// std:wstring 转 LPWSTR
string TCHAR2STRING ( TCHAR * STR );//TCHAR 转换 string
void get_AllHardDiskMemory();

#endif

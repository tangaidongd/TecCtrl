#ifndef _ZS_FDB_H_
#define _ZS_FDB_H_

/*
 *日志接口，可在同一进程之内使用，(线程安全未增加)	
 */
#include "ZsFdb.config" 

//#define FDB_DEBUG//此功能对于是否需要编译做一选择
#ifdef FDB_DEBUG
#define FDB_DEBUG_CALL(fun) fun
#else
#define FDB_DEBUG_CALL(fun) ((void)0)
#endif

#define MAX_LOG_LENGTH 1024

class __declspec(dllimport) ZsFdb
{
public:
	ZsFdb();
	ZsFdb(const char* name, int en=1);
	~ZsFdb();
	
public:
	bool SetName(const char* name, int en=1);
	bool Write(const char* log, ...);
	bool WriteTime(const char* log, ...);
	bool WriteStr(const char* log, int time_flag=0);
	char* GetErrDesc();

protected:
	void* pReserved;
};

//设置记录的文件路径和文件名,默认为当前目录,文件名为fdb_log.txt
__declspec(dllimport) bool zsFdbSetName(const char* name, int en=1);
//记录字符串,例如 zsFdbWrite("%d, %f", a, b);
__declspec(dllimport) bool zsFdbWrite(const char* log, ...);
//记录字符串,例如 zsFdbWriteTime("%d, %f", a, b);会记录时间yyyy-mm-dd hh-mm-ss 在日志前
__declspec(dllimport) bool zsFdbWriteTime(const char* log, ...);
//如果函数调用出错，可以获取错误描述，不提供编号方式错误
__declspec(dllimport) char* zsFdbGetErrDesc();



#endif//_ZS_FDB_H_

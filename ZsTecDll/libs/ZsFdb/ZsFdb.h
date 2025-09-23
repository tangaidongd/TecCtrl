#ifndef _ZS_FDB_H_
#define _ZS_FDB_H_

/*
 *��־�ӿڣ�����ͬһ����֮��ʹ�ã�(�̰߳�ȫδ����)	
 */
#include "ZsFdb.config" 

//#define FDB_DEBUG//�˹��ܶ����Ƿ���Ҫ������һѡ��
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

//���ü�¼���ļ�·�����ļ���,Ĭ��Ϊ��ǰĿ¼,�ļ���Ϊfdb_log.txt
__declspec(dllimport) bool zsFdbSetName(const char* name, int en=1);
//��¼�ַ���,���� zsFdbWrite("%d, %f", a, b);
__declspec(dllimport) bool zsFdbWrite(const char* log, ...);
//��¼�ַ���,���� zsFdbWriteTime("%d, %f", a, b);���¼ʱ��yyyy-mm-dd hh-mm-ss ����־ǰ
__declspec(dllimport) bool zsFdbWriteTime(const char* log, ...);
//����������ó������Ի�ȡ�������������ṩ��ŷ�ʽ����
__declspec(dllimport) char* zsFdbGetErrDesc();



#endif//_ZS_FDB_H_

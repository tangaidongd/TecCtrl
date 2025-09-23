#include "StdAfx.h"
#include "log.h"

#include <windows.h>
#include <stdio.h>
#include <string>
#include <io.h>


using namespace std;

typedef unsigned char BYTE;
#define MaxMallocMem 256



int CUtil::HexToAsc(unsigned char *pDst, char *pSrc, int nSrcLen)
{
	for(int i=0; i<nSrcLen; i+=2)
	{
		// 输出高4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else if(*pSrc>='A' && *pSrc<='F')
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}
		else
		{
			*pDst = (*pSrc - 'a' + 10) << 4;
		}

		pSrc++;

		// 输出低4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst |= *pSrc - '0';
		}
		else if(*pSrc>='A' && *pSrc<='F')
		{
			*pDst |= *pSrc - 'A' + 10;
		}
		else
		{
			*pDst |= *pSrc - 'a' + 10;
		}

		pSrc++;
		pDst++;
	}
	// 返回目标数据长度
	return nSrcLen / 2;
}

int CUtil::AscToHex(char *pDst,unsigned char *pSrc,int SrcLen)
{	
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
	for(int i=0; i<SrcLen; i++)
	{
		*pDst++ = tab[*((BYTE*)pSrc) >> 4];		// 输出低4位c
		*pDst++ = tab[*((BYTE*)pSrc) & 0x0f];	// 输出高4位
		pSrc++;
	}
	// 输出字符串加个结束符
	if(pDst)
	{
		*pDst = '\0';
	}
	// 返回目标字符串长度
	return SrcLen * 2;
}


HANDLE LogMutex= CreateMutex(NULL,FALSE, "ZsTecDll");
void CUtil::WriteLog(EM_LogLevel emLogLv, char * pMsg)
{
	SYSTEMTIME systime;
	FILE       *fp;
	if(pMsg==NULL)
	{
		return;
	}
	GetLocalTime(&systime);//GetSystemTime得到的是格林威治标准时间
	
	char zFilePath[MAX_PATH] = {0};
	sprintf_s(zFilePath, "%s\\%04d%02d%02d.log", g_pLogDir, systime.wYear,systime.wMonth,systime.wDay);

	WaitForSingleObject(LogMutex,INFINITE);
	if((fopen_s(&fp, zFilePath, "a")) == 0)
	{		
		fprintf(fp,"[%d %d-%d %d:%d:%d %d] [pid=0x%x,tid=0x%x] [%s] %s\n",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond,systime.wMilliseconds,GetCurrentProcessId(),GetCurrentThreadId(),g_pLogLevel[emLogLv], pMsg);
		fclose(fp);
	}
	ReleaseMutex(LogMutex);
}
bool CUtil::WriteFormatLog(EM_LogLevel emLogLv, const char* pFormat, ...)
{
	char zMemory[MaxMallocMem+128] = {0};
	va_list VaList;
	memset(zMemory, 0, MaxMallocMem+128);
	va_start(VaList, pFormat);
	vsnprintf_s(zMemory, MaxMallocMem+128-1,MaxMallocMem+128-1, pFormat, VaList); 
	WriteLog(emLogLv, zMemory);
	va_end(VaList);
	return true;
}
bool CUtil::WriteHexLog(EM_LogLevel emLogLv, char *pVarName,char *pBuffer,unsigned int Len)
{
	if(pVarName==NULL||pBuffer==NULL)
	{  
		WriteLog(emLogLv, "WriteHexLog() NULL parameter");
		return false;
	}
	char zMemory[MaxMallocMem+16] = {0};
	
	unsigned int i;
	unsigned int Groups=Len/MaxMallocMem/2;
	for(i=0;i<Groups;i++)
	{
		memset(zMemory,0,MaxMallocMem+16);
		AscToHex(zMemory,(unsigned char *)pBuffer+MaxMallocMem/2*i,MaxMallocMem/2);
		WriteFormatLog(emLogLv, "%s[%d](%d) = %s ",pVarName,MaxMallocMem/2,i,zMemory);
	}
	memset(zMemory,0,MaxMallocMem+16);
	AscToHex(zMemory,(unsigned char *)pBuffer+MaxMallocMem/2*i,Len%(MaxMallocMem/2));
	if(i==0)
	{
		WriteFormatLog(emLogLv, "%s[%d] = %s ",pVarName,(Len%(MaxMallocMem/2)),zMemory);
	}
	else
	{
		WriteFormatLog(emLogLv, "%s[%d](%d) = %s ",pVarName,(Len%(MaxMallocMem/2)),i,zMemory);
	}
	return true;
}


bool CheckLogDir()
{
	if (_access(g_pLogDir, 0) == -1)
		return false;

	return true;
}

void DEBUG_LOG(EM_LogLevel emLv, const char* pFormat, ...)
{
	if (!CheckLogDir())
		return;

	char zMemory[MaxMallocMem];
	va_list VaList; 
	memset(zMemory, 0, MaxMallocMem);
	va_start(VaList, pFormat);
	vsnprintf_s(zMemory, MaxMallocMem - 1, MaxMallocMem - 1, pFormat, VaList);

	CUtil::WriteLog(emLv, zMemory);
	va_end(VaList);
}

void DEBUG_HEX(EM_LogLevel emLv, char* pVarName, char* pBuffer, int Len)
{
	if (!CheckLogDir())
		return;

	CUtil::WriteHexLog(emLv, pVarName, pBuffer, Len);
}








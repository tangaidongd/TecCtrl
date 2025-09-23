#include "stdafx.h"
#include <io.h>
#include <afxmt.h>
#include "tecbase.h"

CTecBase::CTecBase()
{
	m_errDesc[0] = 0;
	m_isDebug = false;
	m_nTecValid = 0x3f;
}
CTecBase::~CTecBase()
{
	Exit();
}
bool CTecBase::Exit()
{
	return true;
}
const char* CTecBase::GetErrDesc()
{
	return m_errDesc;
}
void CTecBase::CheckDebugFlag()
{
	m_isDebug = false;

	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(exeFullPath, drive, dir, NULL, NULL);
	char detect_file[255];
	sprintf(detect_file, "%s%s\\tec.debug", drive, dir);
	if(_access(detect_file, 0)!=-1)
	{
		m_isDebug = true;
	}
	else
	{
		sprintf(detect_file, "%s%s\\hjl.616", drive, dir);
		if(_access(detect_file, 0)!=-1)
		{
			m_isDebug = true;
		}
	}
}
////////////////////////////////////
static CCriticalSection s_criticalSection;
CMyThreadLock::CMyThreadLock()
{
	s_criticalSection.Lock();
}
CMyThreadLock::~CMyThreadLock()
{
	s_criticalSection.Unlock();
}
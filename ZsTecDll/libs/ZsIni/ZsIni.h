#ifndef _ZS_INI_H_
#define _ZS_INI_H_

#include "ZsIni_config.h"

class __declspec(dllimport) ZsIni
{
public:
	ZsIni();
	~ZsIni();
public:
	bool SetFilePath(LPCTSTR pathName);
	bool InitSection(LPCTSTR sectionName);
public:
	bool WriteString(LPCTSTR sectionName, LPCTSTR lpKeyName, LPCTSTR lpString);
	bool WriteInt(LPCTSTR sectionName, LPCTSTR lpKeyName, int keyValue);
	bool WriteDouble(LPCTSTR sectionName, LPCTSTR lpKeyName, double keyValue);
	bool WriteDouble(LPCTSTR sectionName, LPCTSTR lpKeyName, double keyValue, int precision);//小数点后指定精度，不能超过15

	int ReadString(LPCTSTR sectionName, LPCTSTR lpKeyName, LPCTSTR lpDefaultString, LPTSTR lpReturnString, int nSize);
	int ReadInt(LPCTSTR sectionName, LPCTSTR lpKeyName, int nDefault);
	double ReadDouble(LPCTSTR sectionName, LPCTSTR lpKeyName, double fDefault);
public:
	bool SetSection(LPCTSTR sectionName);
	bool WriteString(LPCTSTR lpKeyName, LPCTSTR lpString);
	bool WriteInt(LPCTSTR lpKeyName, int keyValue);
	bool WriteDouble(LPCTSTR lpKeyName, double keyValue);
	bool WriteDouble(LPCTSTR lpKeyName, double keyValue, int precision);//小数点后指定精度，不能超过15

	int ReadString(LPCTSTR lpKeyName, LPCTSTR lpDefaultString, LPTSTR lpReturnString, int nSize);
	int ReadInt(LPCTSTR lpKeyName, int nDefault);
	double ReadDouble(LPCTSTR lpKeyName, double fDefault);
protected:
	void* preserved;
};


#endif//_ZS_INI_H_


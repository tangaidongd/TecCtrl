#ifndef _RSCOMM_DLL_H_
#define _RSCOMM_DLL_H_

#include "RsCommDll.config"

//同步阻塞型操作的串口类接口
class __declspec(dllimport) CSynRsComm
{
public:
	CSynRsComm();
	virtual ~CSynRsComm();
public:
	//port_no表示串口序号，默认无校验位，8位，1位停止位
	bool Open(int port_no, int baud_rate=9600);
//		int parity=0, int byte_size=8, int stop_bit=1);
	bool Close();
	bool IsOpen();
	int GetPort();
	bool WriteData(const unsigned char* pdata, int data_len);
	int ReadData(unsigned char* pdata, int data_len);
	bool WriteString(const char* pdata);
	int ReadString(char* pdata, int data_len);
protected:
	void* m_pReserved;
};


////////////////////////////////////////////////////////////////////
class __declspec(dllimport) CSynPComm
{
public:
	CSynPComm();
	virtual ~CSynPComm();
public:
	//port_no表示串口序号，默认无校验位，8位，1位停止位
	//parity: 0无校验; 1奇校验; 2偶校验
	bool Open(int port_no, int baud_rate=9600);	
	bool Close();
	bool IsOpen();
	int GetPort();
	bool WriteData(const unsigned char* pdata, int data_len);
	int ReadData(unsigned char* pdata, int data_len);
protected:
	void* m_pReserved;
};

#endif//_RSCOMM_DLL_H_

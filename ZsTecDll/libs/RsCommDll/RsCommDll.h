#ifndef _RSCOMM_DLL_H_
#define _RSCOMM_DLL_H_

#include "RsCommDll.config"

//ͬ�������Ͳ����Ĵ�����ӿ�
class __declspec(dllimport) CSynRsComm
{
public:
	CSynRsComm();
	virtual ~CSynRsComm();
public:
	//port_no��ʾ������ţ�Ĭ����У��λ��8λ��1λֹͣλ
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
	//port_no��ʾ������ţ�Ĭ����У��λ��8λ��1λֹͣλ
	//parity: 0��У��; 1��У��; 2żУ��
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

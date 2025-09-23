#ifndef _TEC_V2_H_
#define _TEC_V2_H_

#include "TecV1.h"

class CTecV2: public CTecV1
{
public:
	CTecV2();
	~CTecV2();
public:
	int Init(int com, int total);
	virtual bool Exit();
public:
	virtual int GetActTemp(double* temp, int total);
protected:
	virtual CSynPComm* GetComAndCh(int no, int& chNo, int& addr);
public:
	int m_bits;
};


UINT ThreadReqCurTempV2(LPVOID pParam);

extern CTecV2 g_tecV2Obj;

#endif//_TEC_V2_H_
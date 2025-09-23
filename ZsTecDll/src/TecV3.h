#ifndef _TEC_V3_H_
#define _TEC_V3_H_

#include "TecV1.h"

class CTecV3: public CTecV1
{
public:
	CTecV3();
	~CTecV3();
public:
	int Init(int com, int total);
	virtual bool Exit();
public:
	virtual int GetActTemp(double* temp, int total);
	virtual bool InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag);
	virtual bool InitTscNum(int no, double tscNum);
protected:
	virtual CSynPComm* GetComAndCh(int no, int& chNo, int& addr);
public:
	int m_bits;
};


UINT ThreadReqCurTempV3(LPVOID pParam);

extern CTecV3 g_tecV3Obj;

#endif//_TEC_V2_H_
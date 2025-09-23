#include "StdAfx.h"
#include "TecV2.h"


CTecV2 g_tecV2Obj;

CTecV2::CTecV2()
{
}
CTecV2::~CTecV2()
{
	Exit();
}
int CTecV2::Init(int com, int total)
{
	Exit();
	CheckDebugFlag();
	int rtBit = 0;
	const int RATE = 57600;
	int rt = m_comObj[0].Open(com, RATE);
	int rt1 = 0;
	//ASSERT(total==6 || total==12);
	m_total = total;
	m_Protocolmode = PROTOCOL_OLD;
	memset(m_curTemp, 0, sizeof(m_curTemp));
	//确认下该端口通信成功
	if(rt)
	{
		double temp = 0;
		for(int i=1; i<=total; i++)
		{
			if (!JudgeTecValid(i))
				continue;;

			if(GetCurTemp(i, temp))
			{
				rtBit |= (0x01<<(i-1));
			}
		}
/*		if(GetCurTemp(1, temp))
		{
			rtBit |= 0x01;
		}
		if(GetCurTemp(2, temp))
		{
			rtBit |= 0x02;
		}
		if(GetCurTemp(3, temp))
		{
			rtBit |= 0x04;
		}
		if(GetCurTemp(4, temp))
		{
			rtBit |= 0x08;
		}
		if(GetCurTemp(5, temp))
		{
			rtBit |= 0x10;
		}
		if(GetCurTemp(6, temp))
		{
			rtBit |= 0x20;
		}*/
	}
	else
	{
		strcpy(m_errDesc, "open com failed");
		return 0;
	}
	if(!rtBit)
	{
		m_comObj[0].Close();
		strcpy(m_errDesc, "open all tec failed");
		return 0;
	}
	//建立查询线程
	m_bits = rtBit;
	if(!AfxBeginThread(ThreadReqCurTempV2, this, THREAD_PRIORITY_LOWEST))
	{
		strcpy(m_errDesc, "create thread failed");
		Exit();
		return 0;
	}
	m_errDesc[0] = 0;
	for(int i=0; i<total; i++)
	{
		if(!(rtBit&(0x01<<i)) && JudgeTecValid(i+1))
		{
			char err[100];
			sprintf(err, "open tec%d failed, ", i+1);
			strcat(m_errDesc, err);
		}
	}
/*	if(!(rtBit&0x01))
	{
		strcat(m_errDesc, "open tec1 failed, ");
	}
	if(!(rtBit&0x02))
	{
		strcat(m_errDesc, "open tec2 failed, ");
	}
	if(!(rtBit&0x04))
	{
		strcat(m_errDesc, "open tec3 failed, ");
	}
	if(!(rtBit&0x08))
	{
		strcat(m_errDesc, "open tec4 failed, ");
	}
	if(!(rtBit&0x10))
	{
		strcat(m_errDesc, "open tec5 failed, ");
	}
	if(!(rtBit&0x20))
	{
		strcat(m_errDesc, "open tec6 failed, ");
	}*/
	return rtBit;
}
bool CTecV2::Exit()
{
	//关闭查询线程
	// 这段代码在异常退出时候，会卡死
	m_isThreadRun = false;
	while (m_threadRunState)
	{
		Sleep(200);
	}

	m_comObj[0].Close();
	m_bits = 0;
	return false;
}

int CTecV2::GetActTemp(double* temp, int total)
{
	//ASSERT(total>=6);
	if (total > m_total)
		total = m_total;

	int rt = 0;
	if(!m_comObj[0].IsOpen())
	{
		for(int i=0; i<total; i++)
		{
			temp[i] = 0;
		}
		return 0;
	}
	for(int i=0; i<total; i++)
	{
		if(m_bits&(0x01<<i) && JudgeTecValid(i+1))
		{
			temp[i] = m_curTemp[i];
			rt += 1;
		}
		else
		{
			temp[i] = 0;
		}
	}
/*	if(m_bits&0x01)
	{
		temp[0] = m_curTemp[0];
		rt  += 1;
	}
	else
	{
		temp[0] = 0;
	}
	if(m_bits&0x02)
	{
		temp[1] = m_curTemp[1];
		rt  += 1;
	}
	else
	{
		temp[1] = 0;
	}
	if(m_bits&0x04)
	{
		temp[2] = m_curTemp[2];
		rt  += 1;
	}
	else
	{
		temp[2] = 0;
	}
	if(m_bits&0x08)
	{
		temp[3] = m_curTemp[3];
		rt  += 1;
	}
	else
	{
		temp[3] = 0;
	}
	if(m_bits&0x10)
	{
		temp[4] = m_curTemp[4];
		rt  += 1;
	}
	else
	{
		temp[4] = 0;
	}
	if(m_bits&0x20)
	{
		temp[5] = m_curTemp[5];
		rt  += 1;
	}	
	else
	{
		temp[5] = 0;
	}*/
	return rt;
}

CSynPComm* CTecV2::GetComAndCh(int no, int& chNo, int& addr)
{
	if(no<1 || no>12)
	{
		//ASSERT(no>0 && no<12);
		return NULL;
	}
	chNo = 1;
	addr = no-1;
	return &m_comObj[0];
}

UINT ThreadReqCurTempV2(LPVOID pParam)
{
	CTecV2* pTec = (CTecV2*)pParam;
	pTec->m_isThreadRun = true;
	pTec->m_threadRunState = true;
	int total = pTec->m_total;
	int cnt = 0;
	DWORD st = GetTickCount();
	while(pTec->m_isThreadRun)
	{
		if(GetTickCount()-st < 100)
		{
			Sleep(1);
			continue;
		}
		if(cnt>total)
		{
			cnt = 1;
		}
		else
		{
			cnt++;
		}
		double temp = 0;
		for(int i=1; i<=total; i++)
		{
			if((pTec->m_bits&(0x01<<(i-1)) && cnt==i))
			{
				if (!pTec->GetCurTemp(i, temp))
					pTec->m_curTemp[cnt - 1] = 100;
			}
		}
	/*	if((pTec->m_bits&0x01) && cnt==1)
		{
			if(!pTec->GetCurTemp(1, temp))
				temp = 100;
		}
		if((pTec->m_bits&0x02) && cnt==2)
		{
			if(!pTec->GetCurTemp(2, temp))
				temp = 100;
		}
		if((pTec->m_bits&0x04) && cnt==3)
		{
			if(!pTec->GetCurTemp(3, temp))
				temp = 100;
		}
		if((pTec->m_bits&0x08) && cnt==4)
		{
			if(!pTec->GetCurTemp(4, temp))
				temp = 100;
		}
		if((pTec->m_bits&0x10) && cnt==5)
		{
			if(!pTec->GetCurTemp(5, temp))
				temp = 100;
		}
		if((pTec->m_bits&0x20) && cnt==6)
		{
			if(!pTec->GetCurTemp(6, temp))
				temp = 100;
		}*/
		st = GetTickCount();
	}
	pTec->m_threadRunState = false;
	return 0;
}
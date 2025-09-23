#include "StdAfx.h"
#include "TecV3.h"


CTecV3 g_tecV3Obj;

CTecV3::CTecV3()
{
}
CTecV3::~CTecV3()
{
	Exit();
}
int CTecV3::Init(int com, int total)
{
	Exit();
	CheckDebugFlag();
	int rtBit = 0;
	const int RATE = 57600;
	int rt = m_comObj[0].Open(com, RATE);
	int rt1 = 0;
	m_total = total;
	m_Protocolmode = PROTOCOL_NEW;
	memset(m_curTemp, 0, sizeof(m_curTemp));

	if (m_total != 6 && m_total != 12 && m_total != 14)
	{
		strcpy(m_errDesc, "tec must be 6 or 12 or 14");
		return 0;
	}

	//确认下该端口通信成功
	if(rt)
	{
		double temp = 0;
		for(int i=1; i<=total; i++)
		{
			if (!JudgeTecValid(i))
				continue;
			if(GetCurTemp(i, temp))
			{
				rtBit |= (0x01<<(i-1));
			}
		}
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
	if(!AfxBeginThread(ThreadReqCurTempV3, this, THREAD_PRIORITY_LOWEST))
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
	return rtBit;
}
bool CTecV3::Exit()
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

int CTecV3::GetActTemp(double* temp, int total)
{
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
		if(m_bits&(0x01<<i))
		{
			temp[i] = m_curTemp[i];
			rt += 1;
		}
		else
		{
			temp[i] = 0;
		}
	}
	return rt;
}

bool CTecV3::InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag)
{
	int nRetFlag = retFlag;
	for (int i = 0; i < m_total; ++i)
	{
		double dMaxLimtTemp = 0.0;
		double dMinLimtTemp = 0.0;
		if (GetLimitTemp(i + 1, dMaxLimtTemp, dMinLimtTemp))
		{
			m_stLimitTemp[i].dLimitMaxTemp = dMaxLimtTemp;
			m_stLimitTemp[i].dLimitMinTemp = dMinLimtTemp;
		}
		else
		{
			m_stLimitTemp[i].dLimitMaxTemp = maxLimtTemp;
			m_stLimitTemp[i].dLimitMinTemp = minLimtTemp;

			// 获取失败的设备，标志错误
			nRetFlag &= ~(1 << i);
		}
	}

	if (nRetFlag != retFlag)
	{
		retFlag = nRetFlag;
		char err[100];
		sprintf(err, "GetLimitTemp failed the result = %d ", nRetFlag);
		strcat(m_errDesc, err);
		return false;
	}
	return true;
}

bool CTecV3::InitTscNum(int no, double tscNum)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if (pComObj == NULL)
		return false;
	char cmd[100];
	// 设置tsc数目
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(INIT_TSC_NUM), tscNum);
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(INIT_TSC_NUM));
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}

	// 查询tsc数目
	double dTscNUm = 0;
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(INIT_TSC_NUM));
	if (!ExecGetCmd(pComObj, addr, cmd, dTscNUm))
	{
		return false;
	}

	if (dTscNUm != tscNum)
	{
		return false;
	}

	return true;
}

CSynPComm* CTecV3::GetComAndCh(int no, int& chNo, int& addr)
{
	if (no < 1 || no > 14)
	{
		//ASSERT(no > 0 && no < 12);
		return NULL;
	}

	int nNo = no - 1;
	int nDiv = m_total / 2;
	if (m_total == 6)
	{
		nDiv = m_total;
		if (no > m_total)
		{
			return NULL;
		}
	}

	chNo = nNo / nDiv + 1;
	addr = nNo % nDiv;
	return &m_comObj[0];
}

UINT ThreadReqCurTempV3(LPVOID pParam)
{
	CTecV3* pTec = (CTecV3*)pParam;
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
				if(!pTec->GetCurTemp(i, temp))
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
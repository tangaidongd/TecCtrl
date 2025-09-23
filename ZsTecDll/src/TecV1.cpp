#include "StdAfx.h"
#include <direct.h>
#include <math.h>
#include <vector>
#include "TecV1.h"

CTecV1 g_tecV1Obj;

CTecV1::CTecV1()
{
	m_isThreadRun = false;
	m_threadRunState = false;

	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(exeFullPath, drive, dir, NULL,NULL);
	char logDir[255];
	sprintf(logDir, "%s%s\\log", drive, dir);
	_mkdir(logDir);
	char logFile[255];
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	sprintf(logFile, "%s\\tec-%04d-%02d.log", logDir, tm.wYear, tm.wMonth);
	m_fdbObj.SetName(logFile);

	memset(m_curTemp, 0, sizeof(double));
}
CTecV1::~CTecV1()
{
	Exit();
}
int CTecV1::Init(int com1, int com2, int com3)
{
	Exit();
	CheckDebugFlag();
	int rtBit = 0;
	const int RATE = 57600;
	int rt1 = m_comObj[0].Open(com1, RATE);
	m_total = 6;
	m_Protocolmode = PROTOCOL_OLD;
	memset(m_curTemp, 0, sizeof(m_curTemp));
	//确认下该端口通信成功
	int nNo = 1;
	if(rt1)
	{
		double temp = 0;
		int TempBit = 0;
		for (int i = 0; i < 2; ++i)
		{
			if (JudgeTecValid(i+1))
			{
				if (GetCurTemp(i+1, temp))
				{
					TempBit |= 0x01 << i;
				}
			}
		}

		if (TempBit == 0)
		{
			m_comObj[0].Close();
			rt1 = 0;
		}

		rtBit = rtBit | TempBit;
	}
	int rt2 = m_comObj[1].Open(com2, RATE);
	if(rt2)
	{
		double temp = 0;
		int TempBit = 0;
		for (int i = 0; i < 2; ++i)
		{
			if (JudgeTecValid(i + 3))
			{
				if (GetCurTemp(i + 3, temp))
				{
					TempBit |= 0x01<<i;
				}
			}
		}

		if (TempBit == 0)
		{
			m_comObj[1].Close();
			rt2 = 0;
		}
		rtBit = rtBit | (TempBit<<2);
	}
	int rt3 = m_comObj[2].Open(com3, RATE);
	if(rt3)
	{
		double temp = 0;
		int TempBit = 0;
		for (int i = 0; i < 2; ++i)
		{
			if (JudgeTecValid(i + 5))
			{
				if (GetCurTemp(i + 5, temp))
				{
					TempBit |= 0x01 << i;
				}
			}
		}

		if (TempBit == 0)
		{
			m_comObj[2].Close();
			rt3 = 0;
		}
		rtBit = rtBit | (TempBit << 4);
	}

	if(rt1||rt2||rt3)
	{
		//建立查询线程
		if(!AfxBeginThread(ThreadReqCurTemp, this, THREAD_PRIORITY_LOWEST))
		{
			strcpy(m_errDesc, "create thread failed");
			Exit();
			return 0;
		}	
	}
	if(!(rt1||rt2||rt3))
	{
		strcpy(m_errDesc, "open all tec failed");
		return 0;
	}
	m_errDesc[0] = 0;
	if(!rt1)
	{
		strcat(m_errDesc, "open com1 failed, ");
	}
	if(!rt2)
	{
		strcat(m_errDesc, "open com2 failed, ");
	}
	if(!rt3)
	{
		strcat(m_errDesc, "open com3 failed, ");
	}
	return rtBit;
}
bool CTecV1::Exit()
{
	//关闭查询线程
	m_isThreadRun = false;
	while (m_threadRunState)
	{
		Sleep(200);
	}

	m_comObj[0].Close();
	m_comObj[1].Close();
	m_comObj[2].Close();
	return false;
}
//base 1
bool CTecV1::SetCtrlPara(int no, double destTemp, double maxTemp, 
	double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	//设置调节温度
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(DEST_TEMP), destTemp);
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(DEST_TEMP));
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	//设置温度高阈值
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(MAX_TEMP), maxTemp);
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(MAX_TEMP));
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	//设置最大输出
	double rMaxOutput = 0;
	if(!GetMaxOutput(no, rMaxOutput))
		return false;
	if(fabs(maxOutput-rMaxOutput)>0.01)
	{
		int isOpen = 0;
		if(!IsTecOpen(no, isOpen))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 0))
				return false;
		}		
		if(!SetMaxOutput(no, maxOutput))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 1))
				return false;
		}	
	}
	//设置最大current
	double rMaxCurrent = 0;
	if(!GetMaxOutput(no, rMaxCurrent))
		return false;
	if(fabs(maxCurrent-rMaxCurrent)>0.01)
	{
		int isOpen = 0;
		if(!IsTecOpen(no, isOpen))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 0))
				return false;
		}		
		if(!SetMaxCurrent(no, maxCurrent))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 1))
				return false;
		}
		
	}

	if (m_Protocolmode == PROTOCOL_NEW)
	{
		// 设置超温温度上限和下限
		if (!SetLimitTemp(no, maxLimtTemp, minLimtTemp))
			return false;
	}
	m_stLimitTemp[no - 1].dLimitMaxTemp = maxLimtTemp;
	m_stLimitTemp[no - 1].dLimitMinTemp = minLimtTemp;

	//设置控制模式
	int rMode = 0;
	if(!GetMode(no, rMode))
		return false;
	if(rMode!=mode)
	{
		int isOpen = 0;
		if(!IsTecOpen(no, isOpen))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 0))
				return false;
		}		
		if(!SetMode(no, mode))
			return false;
		if(isOpen)
		{
			if(!OpenTec(no, 1))
				return false;
		}
	}
	return true;
}
bool CTecV1::GetCtrlPara(int no, double& destTemp, double& maxTemp, 
	double& maxOutput, double& maxCurrent, double& maxLimtTemp, double& minLimtTemp, int& mode)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	//获取调节温度
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(DEST_TEMP));
	if(!ExecGetCmd(pComObj, addr, cmd, destTemp))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(MAX_TEMP));
	if(!ExecGetCmd(pComObj, addr, cmd, maxTemp))
	{
		return false;
	}

	// 获取上下限温度
	if (m_Protocolmode == PROTOCOL_NEW)
	{
		// 设置超温温度上限和下限
		if (!GetLimitTemp(no, m_stLimitTemp[no - 1].dLimitMaxTemp, m_stLimitTemp[no - 1].dLimitMinTemp))
			return false;
	}
	maxLimtTemp = m_stLimitTemp[no - 1].dLimitMaxTemp;
	minLimtTemp = m_stLimitTemp[no - 1].dLimitMinTemp;

	if(!GetMaxOutput(no, maxOutput))
		return false;
	if(!GetMaxCurrent(no, maxCurrent))
		return false;
	if(!GetMode(no, mode))
		return false;
	return true;
}
bool CTecV1::GetCurTemp(int no, double& temp)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	//获取调节温度
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(CUR_TEMP));
	if(!ExecGetCmd(pComObj, addr, cmd, temp))
	{
		return false;
	}
	m_curTemp[no-1] = temp;
	return true;
}
bool CTecV1::IsTecOpen(int no, int& isOpen)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(TEC_OPEN));
	double fOpen = 0;
	if(!ExecGetCmd(pComObj, addr, cmd, fOpen))
		return false;
	isOpen = int(fOpen);
	return true;
}

bool CTecV1::IsAllTecOpen(int& isOpen)
{
	int nRetStatus = 0;
	if (m_Protocolmode == PROTOCOL_NEW)
	{
		// 先计算有几个tc通道
		int chNo = 0;
		int addr = 0;
		std::vector<int> vRetStatus;
		vRetStatus.clear();
		for (int i = 1; i <= m_total; ++i)
		{
			int chNoTemp = 0;
			CSynPComm* pComObj = GetComAndCh(i, chNoTemp, addr);
			if (pComObj == NULL)
				return false;
			if (chNoTemp > 2)
			{
				strcat(m_errDesc, "TCNO count > 2");
				return false;
			}
				
			if (chNoTemp != chNo)
			{
				chNo = chNoTemp;
				char cmd[100];
				sprintf(cmd, "TC%d:%s", chNoTemp, GetParaName(TEC_ALL_OPEN));
				double fOpen = 0;
				if (!ExecGetCmd(pComObj, 0, cmd, fOpen))
					return false;
				vRetStatus.push_back(fOpen);
			}
		}
		//vRetStatus.push_back(0x3e);
		//vRetStatus.push_back(0x3f);
		// 正常tsc数目是2的整数倍
		int nSigleTcNum = vRetStatus.size() > 1 ? m_total / 2 : m_total;
		for (int i = 0; i< vRetStatus.size(); ++i)
		{
			nRetStatus |= (vRetStatus.at(i) << (i* nSigleTcNum));
		}
		isOpen = nRetStatus;
	}
	else
	{
		// 老协议
		for (int i = 1; i <= m_total; ++i)
		{
			if (!JudgeTecValid(i))
				continue;

			int chNo = 0;
			int addr = 0;
			CSynPComm* pComObj = GetComAndCh(i, chNo, addr);
			if (pComObj == NULL)
				return false;
			char cmd[100];
			sprintf(cmd, "TC%d:%s", chNo, GetParaName(TEC_OPEN));
			double fOpen = 0;
			if (!ExecGetCmd(pComObj, addr, cmd, fOpen))
				return false;
			int nValue = int(fOpen);
			nRetStatus |= (nValue << (i - 1));
		}
		isOpen = nRetStatus;
	}
	return true;
}

bool CTecV1::OpenAllTec(int& isOpen)
{
	if (m_Protocolmode == PROTOCOL_NEW)
	{
		// 先计算有几个tc通道
		int chNo = 0;
		int addr = 0;
		int nTcCount = 0;
		for (int i = 1; i <= m_total; ++i)
		{
			int chNoTemp = 0;
			CSynPComm* pComObj = GetComAndCh(i, chNoTemp, addr);
			if (pComObj == NULL)
				return false;
			if (chNoTemp > 2)
			{
				strcat(m_errDesc, "TCNO count > 2 ");
				return false;
			}
			if (chNoTemp != chNo)
			{
				chNo = chNoTemp;
				nTcCount++;
			}
		}

		// 设置开关状态
		int nSigleTcNum = nTcCount > 1 ? m_total / 2 : m_total;
		int nOriValue = (1 << nSigleTcNum) - 1;
		for (int i = 1; i <= nTcCount; ++i)
		{
			CSynPComm* pComObj = &m_comObj[0];
			if (pComObj == NULL)
				return false;
			char cmd[100];
			int nOffset = (i - 1) * nSigleTcNum;
			int nStatusVal = nOriValue & (isOpen >> nOffset);
			sprintf(cmd, "TC%d:%s=%d", i, GetParaName(TEC_ALL_OPEN), nStatusVal);
			if (!ExecSetCmd(pComObj, 0, cmd))
				return false;
		}
	}
	else
	{
		// 老协议
		for (int i = 1; i <= m_total; ++i)
		{
			if (!JudgeTecValid(i))
				continue;

			int chNo = 0;
			int addr = 0;
			CSynPComm* pComObj = GetComAndCh(i, chNo, addr);
			if (pComObj == NULL)
				return false;
			char cmd[100];
			int nOffset = i - 1;
			int nSetValue = (isOpen>>nOffset) & 0x0001;
			sprintf(cmd, "TC%d:%s=%d", chNo, GetParaName(TEC_OPEN), nSetValue);
			if (!ExecSetCmd(pComObj, addr, cmd))
				return false;
		}
	}
	return true;
}

bool CTecV1::OpenTec(int no, int op)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s=%d", chNo, GetParaName(TEC_OPEN), op?1:0);
	if(!ExecSetCmd(pComObj, addr, cmd))
		return false;
	return true;
}

bool CTecV1::InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag)
{
	for (int i = 0; i < m_total; ++i)
	{
		m_stLimitTemp[i].dLimitMaxTemp = maxLimtTemp;
		m_stLimitTemp[i].dLimitMinTemp = minLimtTemp;
	}

	return true;
}

int CTecV1::GetActTemp(double* temp, int total)
{
	//ASSERT(total>=6);
	if (total > m_total)
		total = m_total;

	int rt = 0;
	if(m_comObj[0].IsOpen())
	{
		temp[0] = m_curTemp[0];
		temp[1] = m_curTemp[1];
		rt += 2;
	}
	else
	{
		temp[0] = 0;
		temp[1] = 0;
	}
	if(m_comObj[1].IsOpen())
	{
		temp[2] = m_curTemp[2];
		temp[3] = m_curTemp[3];
		rt += 2;
	}
	else
	{
		temp[2] = 0;
		temp[3] = 0;
	}
	if(m_comObj[2].IsOpen())
	{
		temp[4] = m_curTemp[4];
		temp[5] = m_curTemp[5];
		rt += 2;
	}
	else
	{
		temp[4] = 0;
		temp[5] = 0;
	}
	return rt;
}


bool CTecV1::JudgeTecValid(int nNo)
{
	if (nNo < 1 || nNo > 24)			// 限制序列号返回1-24
	{
		return false;
	}

	if ((m_nTecValid >> (nNo - 1) & 0x00000001) == 1)
		return true;
	else
		return false;
}

void CTecV1::WriteLogMsg(std::string strMsg)
{
	if (strMsg.empty() || !m_isDebug)
		return;

	m_fdbObj.Write("custom msg: %s", strMsg.c_str());
}

//////////////////////////////////////////////////////
bool CTecV1::SetMaxOutput(int no, double maxOutput)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(MAX_OUTPUT), maxOutput);
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(MAX_OUTPUT));
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	return true;
}
bool CTecV1::SetMaxCurrent(int no, double maxCurrent)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(MAX_CURRENT), maxCurrent);
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(MAX_CURRENT));
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	return true;
}
bool CTecV1::SetMode(int no, int mode)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s=%d", chNo, GetParaName(CTRL_MODE), mode);
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(CTRL_MODE));
	if(!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	return true;
}
bool CTecV1::GetMaxOutput(int no, double& maxOutput)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(MAX_OUTPUT));
	if(!ExecGetCmd(pComObj, addr, cmd, maxOutput))
	{
		return false;
	}
	return true;
}
bool CTecV1::GetMaxCurrent(int no, double& maxCurrent)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(MAX_CURRENT));
	if(!ExecGetCmd(pComObj, addr, cmd, maxCurrent))
	{
		return false;
	}
	return true;
}
bool CTecV1::GetMode(int no, int& mode)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if(pComObj==NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(CTRL_MODE));
	double fmode = 0;
	if(!ExecGetCmd(pComObj, addr, cmd, fmode))
	{
		return false;
	}
	mode = int(fmode);
	return true;
}



bool CTecV1::GetLimitTemp(int no, double& maxLimit, double& minLimit)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if (pComObj == NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s", chNo, GetParaName(LIMIT_MAX_TEMP));
	if (!ExecGetCmd(pComObj, addr, cmd, maxLimit))
	{
		return false;
	}

	sprintf(cmd, "TC%d:%s", chNo, GetParaName(LIMIT_MIN_TEMP));
	if (!ExecGetCmd(pComObj, addr, cmd, minLimit))
	{
		return false;
	}
	return true;
}

bool CTecV1::SetLimitTemp(int no, double maxLimit, double minLimit)
{
	int chNo = 0;
	int addr = 0;
	CSynPComm* pComObj = GetComAndCh(no, chNo, addr);
	if (pComObj == NULL)
		return false;
	char cmd[100];
	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(LIMIT_MAX_TEMP), maxLimit);
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(LIMIT_MAX_TEMP));
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}

	sprintf(cmd, "TC%d:%s=%3.3f", chNo, GetParaName(LIMIT_MIN_TEMP), minLimit);
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}
	sprintf(cmd, "TC%d:%s!", chNo, GetParaName(LIMIT_MIN_TEMP));
	if (!ExecSetCmd(pComObj, addr, cmd))
	{
		return false;
	}

	return true;
}

CSynPComm* CTecV1::GetComAndCh(int no, int& chNo, int& addr)
{
	if(no<1 || no>6)
	{
		//ASSERT(no>0 && no<6);
		return NULL;
	}
	int comNo = (no-1)/2;
	chNo = no - comNo*2;
	addr = 0;
	return &m_comObj[comNo];
}

const char* CTecV1::GetParaName(int paraType)
{
	switch(paraType)
	{
	case DEST_TEMP:
		return "TCADJUSTTEMP";
	case CUR_TEMP:
		return "TCACTUALTEMP";
	case MAX_TEMP:
		return "TCOTPHT";
	case MAX_OUTPUT:
		return "TCMAXV";
	case MAX_CURRENT:
		return "TCADJUSTOCPC";
	case CTRL_MODE:
		return "TCMODE";
	case TEC_OPEN:
		return "TCSW";
	case TEC_ALL_OPEN:
		return "TCALLSW";
	case TEC_IS_OUTPUT:
		return "TCOE";
	case LIMIT_MAX_TEMP:
		return "TCMAXSETTEMP";
	case LIMIT_MIN_TEMP:
		return "TCMINSETTEMP";
	case INIT_TSC_NUM:
		return "TCNUM";
	default:
		return "UNKNOWPARA";
	}
}
bool CTecV1::ExecGetCmd(CSynPComm* pComObj, int addr, const char* cmd, double& val)
{
	CMyThreadLock lockObj;

	bool rt = false;
	char sendBuf[100];
	char recvBuf[100];
	sendBuf[0] = 0;
	recvBuf[0] = 0;
	sprintf(sendBuf, "%s?@%d\r", cmd, addr);
	rt = pComObj->WriteData((unsigned char*)(sendBuf), strlen(sendBuf));
	if(rt)
	{
		Sleep(50);
		DWORD tick = GetTickCount();
		int recvLen = 0;
		while(1)
		{
			int len = pComObj->ReadData((unsigned char*)(recvBuf+recvLen), 99-recvLen);
			recvLen += len;
			bool isFindEndFlag = false;
			for(int i=0; i<len; i++)
			{
				if(recvBuf[recvLen-1-i]=='\r')
				{
					isFindEndFlag = true;
					recvBuf[recvLen-i] = 0;
					break;
				}
			}
			if(isFindEndFlag)
				break;
			else if(GetTickCount()-tick > 500)
			{
				rt = false;
				recvBuf[recvLen] = 0;
				break;
			}
			else
			{
				Sleep(1);
			}
		}
		CString str = recvBuf;
		int st = str.Find(cmd);
		if(st != -1)
		{
			int ed = str.Find('\r', 0);
			if(ed != -1)
			{
				st += strlen(cmd)+1;//加上=号
				CString str2 = str.Mid(st, ed-st);
				val = atof(str2.GetBuffer(0));
			}
			else
			{
				rt = false;
			}
		}
		else
		{
			rt = false;
		}
	}
	if(!rt)
	{
		strcpy(m_errDesc, "exec get cmd failed");
		m_fdbObj.WriteTime(m_errDesc);
		m_fdbObj.Write("com%02d send: %s", pComObj->GetPort(), sendBuf);
		m_fdbObj.Write("com%02d recv: %s", pComObj->GetPort(), recvBuf);
	}
	else if(m_isDebug)
	{
		m_fdbObj.Write("com%02d send: %s", pComObj->GetPort(), sendBuf);
		m_fdbObj.Write("com%02d recv: %s", pComObj->GetPort(), recvBuf);
	}
	return rt;	
}
bool CTecV1::ExecSetCmd(CSynPComm* pComObj, int addr, const char* cmd)
{
	CMyThreadLock lockObj;

	bool rt = false;
	char sendBuf[100];
	char recvBuf[100];
	sendBuf[0] = 0;
	recvBuf[0] = 0;
	sprintf(sendBuf, "%s@%d\r", cmd, addr);
	rt = pComObj->WriteData((unsigned char*)(sendBuf), strlen(sendBuf));
	if(rt)
	{
		Sleep(50);
		DWORD tick = GetTickCount();
		int recvLen = 0;
		while(1)
		{
			int len = pComObj->ReadData((unsigned char*)(recvBuf+recvLen), 100-recvLen);
			recvLen += len;
			bool isFindEndFlag = false;
			for(int i=0; i<len; i++)
			{
				if(recvBuf[recvLen-1-i]=='\r')
				{
					isFindEndFlag = true;
					recvBuf[recvLen-i] = 0;
					break;
				}
			}
			if(isFindEndFlag)
				break;
			else if(GetTickCount()-tick > 500)
			{
				rt = false;
				recvBuf[recvLen] = 0;
				break;
			}
			else
			{
				Sleep(1);
			}
		}
		if(rt)
		{
			CString str = recvBuf;
			if(str.Find("CMD:REPLY=1")==-1)
			{
				if(str.Find("CMD:REPLY=8")==-1)
				{
					rt = false;
				}
			}
		}		
	}
	if(!rt)
	{
		strcpy(m_errDesc, "exec set cmd failed");
		m_fdbObj.WriteTime(m_errDesc);
		m_fdbObj.Write("com%02d send: %s", pComObj->GetPort(), sendBuf);
		m_fdbObj.Write("com%02d recv: %s", pComObj->GetPort(), recvBuf);
	}
	else if(m_isDebug)
	{
		m_fdbObj.Write("com%02d send: %s", pComObj->GetPort(), sendBuf);
		m_fdbObj.Write("com%02d recv: %s", pComObj->GetPort(), recvBuf);
	}
	return rt;
}
/////////////////////////////////////////
UINT ThreadReqCurTemp(LPVOID pParam)
{
	CTecV1* pTec = (CTecV1*)pParam;
	pTec->m_isThreadRun = true;
	pTec->m_threadRunState = true;
	int cnt = 0;
	DWORD st = GetTickCount();
	while(pTec->m_isThreadRun)
	{
		if(GetTickCount()-st < 100)
		{
			Sleep(1);
			continue;
		}
		if(cnt>6)
		{
			cnt = 1;
		}
		else
		{
			cnt++;
		}


		double temp = 0;
		if(pTec->m_comObj[0].IsOpen() && cnt==1 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(1, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		if(pTec->m_comObj[0].IsOpen() && cnt==2 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(2, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		if(pTec->m_comObj[1].IsOpen() && cnt==3 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(3, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		if(pTec->m_comObj[1].IsOpen() && cnt==4 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(4, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		if(pTec->m_comObj[2].IsOpen() && cnt==5 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(5, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		if(pTec->m_comObj[2].IsOpen() && cnt==6 && pTec->JudgeTecValid(cnt))
		{
			if(!pTec->GetCurTemp(6, temp))
				pTec->m_curTemp[cnt - 1] = 100;
		}
		st = GetTickCount();
	}
	pTec->m_threadRunState = false;
	return 0;
}
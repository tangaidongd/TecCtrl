

#include "stdafx.h"
#include <direct.h>
#include "zstec.h"
#include "..\\libs\\zsini\\zsini.h"



ZsTec& GetTecObj()
{
	static ZsTec s_tecObj;
	return s_tecObj;
}

ZsTec::ZsTec()
{
	m_pData = new TecDataBlk();
	ASSERT(m_pData);
	
	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(exeFullPath, drive, dir, NULL,NULL);
	char paramDir[255];
	sprintf(paramDir, "%s%s\\param", drive, dir);
	_mkdir(paramDir);
	sprintf(m_pData->paraFile, "%s\\tecpara.dat", paramDir);
	Load();
}
ZsTec::~ZsTec()
{
	Exit();
	if(m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}
}
const char* ZsTec::GetErrDesc()
{
	return m_pData->errDesc;
}

void ZsTec::SetPara(int com1, int com2, int com3, int mode)
{
	m_pData->com = com1;
	m_pData->com2 = com2;
	m_pData->com3 = com3;
	m_pData->mode = mode;
	m_pData->total = 6;
}
void ZsTec::SetPara(int com, int mode)
{
	m_pData->com = com;
	m_pData->mode = mode;
	m_pData->total = 6;
	if (mode == TEC_6CARD_2CH_NEW)
	{
		m_pData->total = 12;
		m_pData->TecExist = 0xFFF;
	}
	if (mode == TEC_12CARD_1CH)
	{
		m_pData->total = 12;
		m_pData->TecExist = 0xFFF;
	}
	if (mode == TEC_7CARD_2CH_NEW)
	{
		m_pData->total = 14;
		m_pData->TecExist = 0x3FFF;
	}
	if (mode == TEC_6CARD_1CH_NEW)
	{
		m_pData->total = 6;
		m_pData->TecExist = 0x3F;
	}
	if (mode == TEC_CUSTOM_CARD_1CH_NEW)
	{
		m_pData->total = 0;
		m_pData->TecExist = 0;
	}
		
}
void ZsTec::SetLmtTemp(double maxTemp, double minTemp)
{
	ASSERT(maxTemp>10);
	ASSERT(minTemp>0);
	m_pData->maxTemp = maxTemp;
	m_pData->minTemp = minTemp;
}

void ZsTec::SetLogFlag(int nLogFlag)
{
	m_pData->nLogFlag = nLogFlag;
}

void ZsTec::GetLogFlag(int& nLogFlag)
{
	nLogFlag = m_pData->nLogFlag;
}

void ZsTec::GetLogRefreshCycle(int& nLogRefreshCycle)
{
	nLogRefreshCycle = m_pData->nLogRefreshCycle;
}

void ZsTec::SetLogRefreshCycle(int nLogRefreshCycle)
{
	m_pData->nLogRefreshCycle = nLogRefreshCycle;
}

void ZsTec::GetPara(int& mode, int& com1, int& com2, int& com3, 
	double& maxTemp, double& minTemp, int& chTotal)
{
	mode = m_pData->mode;
	com1 = m_pData->com;
	com2 = m_pData->com2;
	com3 = m_pData->com3;
	maxTemp = m_pData->maxTemp;
	minTemp = m_pData->minTemp;
	chTotal = m_pData->total;
}

void ZsTec::SetTecExistStatus(int& nTecExistStatus)
{
	m_pData->TecExist = nTecExistStatus;
	if (m_pData->pTecObj)
	{
		m_pData->pTecObj->m_nTecValid = nTecExistStatus;
	}
}

int ZsTec::GetTecExistStatus()
{
	return m_pData->TecExist;
}

int ZsTec::Init()
{
	Exit();
	
	Load();
	int rt = 0;
	// 更新所有设备的缺失情况
	g_tecV1Obj.m_nTecValid = m_pData->TecExist;
	g_tecV2Obj.m_nTecValid = m_pData->TecExist;
	g_tecV3Obj.m_nTecValid = m_pData->TecExist;
	g_tecNewProObj.m_nTecValid = m_pData->TecExist;
	switch(m_pData->mode)
	{
	case TEC_3CARD_2CH:
		rt = g_tecV1Obj.Init(m_pData->com, m_pData->com2, m_pData->com3);
		if(rt)
			m_pData->pTecObj = &g_tecV1Obj;
		if(rt < m_pData->TecExist)
			strcpy(m_pData->errDesc, g_tecV1Obj.GetErrDesc());
		break;
	case TEC_6CARD_1CH:
	case TEC_12CARD_1CH:
		rt = g_tecV2Obj.Init(m_pData->com, m_pData->total);
		if(rt)
			m_pData->pTecObj = &g_tecV2Obj;
		if(rt < m_pData->TecExist)
			strcpy(m_pData->errDesc, g_tecV2Obj.GetErrDesc());
		break;
	case TEC_6CARD_2CH_NEW:
		rt = g_tecV3Obj.Init(m_pData->com, m_pData->total);
		if (rt)
			m_pData->pTecObj = &g_tecV3Obj;
		if (rt < m_pData->TecExist)
			strcpy(m_pData->errDesc, g_tecV3Obj.GetErrDesc());
		break;
	case TEC_7CARD_2CH_NEW:
		rt = g_tecV3Obj.Init(m_pData->com, m_pData->total);
		if (rt)
			m_pData->pTecObj = &g_tecV3Obj;
		if (rt < m_pData->TecExist)
			strcpy(m_pData->errDesc, g_tecV3Obj.GetErrDesc());
		break;
	case TEC_6CARD_1CH_NEW:
		rt = g_tecV3Obj.Init(m_pData->com, m_pData->total);
		if (rt)
			m_pData->pTecObj = &g_tecV3Obj;
		if (rt < m_pData->TecExist)
			strcpy(m_pData->errDesc, g_tecV3Obj.GetErrDesc());
		break;
	case TEC_CUSTOM_CARD_1CH_NEW:
		rt = g_tecNewProObj.Init(m_pData->com, m_pData->total, m_pData->nLogFlag, m_pData->nLogRefreshCycle, m_pData->bIsPowerDouble);
		if (rt)
		{
			m_pData->pTecObj = &g_tecNewProObj;
			if (m_pData->total == 0)
			{
				m_pData->total = g_tecNewProObj.GetTecNum();
				m_pData->TecExist = rt;
			}
		}
		if (!rt)
			strcpy(m_pData->errDesc, g_tecNewProObj.GetErrDesc());
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	// 避免出现新接口，不做函数封装
	if (m_pData->pTecObj)
	{
		// 初始化温度上下限
		m_pData->pTecObj->InitLimitTemp(m_pData->maxTemp, m_pData->minTemp, rt);

		if (m_pData->pTecObj->m_Protocolmode == PROTOCOL_NEW)
		{
			// 初始化tscnum 值初始化每个模块的第一个设备
			int tscIndex = 0;
			if (TEC_6CARD_1CH_NEW == m_pData->mode)
			{
				if (!m_pData->pTecObj->InitTscNum(tscIndex + 1, m_pData->total))
				{
					rt &= ~(1 << tscIndex);
					strcat(m_pData->errDesc, "Set TC1 SetTscNum failed  ");
				}
			}
			else
			{
				tscIndex = 0;
				if (!m_pData->pTecObj->InitTscNum(tscIndex + 1, m_pData->total / 2))
				{
					rt &= ~(1 << tscIndex);
					strcat(m_pData->errDesc, "Set TC1 SetTscNum failed  ");
				}
				// 一定会大于6
				if (m_pData->total > 6)
				{
					tscIndex = m_pData->total / 2;
					if (!m_pData->pTecObj->InitTscNum(tscIndex + 1, m_pData->total / 2))
					{
						rt &= ~(1 << tscIndex);
						strcat(m_pData->errDesc, "Set TC2 SetTscNum failed ");
					}
				}
			}
		}
	}

	// 检查一次返回值
	if (!rt)
	{
		Exit();
		strcpy(m_pData->errDesc, "all tec failed");
		m_pData->bInitRun = true;
		return 0;
	}
	m_pData->bInitRun = true;
	return rt;
}

bool ZsTec::IsInitSucceed()
{
	if (m_pData->pTecObj)
	{
		return true;
	}

	return false;
}

bool ZsTec::IsInitRun()
{
	if (m_pData->bInitRun)
	{
		return true;
	}
	return false;
}

bool ZsTec::Exit()
{
	m_pData->bInitRun = false;
	bool rt = true;
	if(m_pData->pTecObj)
	{
		rt = m_pData->pTecObj->Exit();
		if(rt)
			m_pData->pTecObj = NULL;
	}
	return rt;
}
bool ZsTec::Load()
{
	ZsIni iniObj;
	iniObj.SetFilePath(m_pData->paraFile);
	m_pData->mode = iniObj.ReadInt("PARA", "Mode", TEC_3CARD_2CH);
	m_pData->com = iniObj.ReadInt("PARA", "Com", 7);
	m_pData->com2 = iniObj.ReadInt("PARA", "Com2", 8);
	m_pData->com3 = iniObj.ReadInt("PARA", "Com3", 9);
	m_pData->total = iniObj.ReadInt("PARA", "Total", 6);
	m_pData->TecExist = iniObj.ReadInt("PARA", "TecExist", -1);
	m_pData->nLogRefreshCycle = iniObj.ReadInt("PARA", "LogRefreshCycle", 200);
	m_pData->maxTemp = iniObj.ReadDouble("PARA", "MaxTemp", 25);
	m_pData->minTemp = iniObj.ReadDouble("PARA", "MinTemp", 10);
	m_pData->nLogFlag = iniObj.ReadDouble("PARA", "LOGFLAG", 0);
	m_pData->bIsPowerDouble = iniObj.ReadDouble("PARA", "PowerDouble", 0);

	// 给默认值
	if (-1 == m_pData->TecExist)
	{
		m_pData->TecExist = 0x3F;
		if (m_pData->mode == TEC_12CARD_1CH)
		{
			m_pData->TecExist = 0xFFF;
		}
		if (m_pData->mode == TEC_6CARD_2CH_NEW)
		{
			m_pData->TecExist = 0xFFF;
		}
		if (m_pData->mode == TEC_7CARD_2CH_NEW)
		{
			m_pData->TecExist = 0x3FFF;
		}
		if (m_pData->mode == TEC_6CARD_1CH_NEW)
		{
			m_pData->TecExist = 0x3F;
		}
		if (m_pData->mode == TEC_CUSTOM_CARD_1CH_NEW)
		{
			int nRet = 0;
			for (int i = 0; i < m_pData->total; ++i)
			{
				nRet |= (0x01 << i);
			}
			m_pData->TecExist = nRet;
		}
	}
	return true;
}
bool ZsTec::Save()
{
	ZsIni iniObj;
	iniObj.SetFilePath(m_pData->paraFile);
	iniObj.WriteInt("PARA", "Mode", m_pData->mode);
	iniObj.WriteInt("PARA", "Com", m_pData->com);
	iniObj.WriteInt("PARA", "Com2", m_pData->com2);
	iniObj.WriteInt("PARA", "Com3", m_pData->com3);
	iniObj.WriteInt("PARA", "Total", m_pData->total);
	iniObj.WriteInt("PARA", "TecExist", m_pData->TecExist);
	iniObj.WriteInt("PARA", "LogRefreshCycle", m_pData->nLogRefreshCycle);
	iniObj.WriteDouble("PARA", "MaxTemp", m_pData->maxTemp);
	iniObj.WriteDouble("PARA", "MinTemp", m_pData->minTemp);
	iniObj.WriteDouble("PARA", "LOGFLAG", m_pData->nLogFlag);
	iniObj.WriteDouble("PARA", "PowerDouble", m_pData->bIsPowerDouble);
	return true;
}
bool ZsTec::SetCtrlPara(int no, double destTemp, double maxTemp, 
		double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode)
{
	if(m_pData->pTecObj==NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}
	if (!m_pData->pTecObj->JudgeTecValid(no))
	{
		strcpy(m_pData->errDesc, "the no is invalid");
		return false;
	}
	

	bool rt = m_pData->pTecObj->SetCtrlPara(no, destTemp, maxTemp, maxOutput, maxCurrent, maxLimtTemp, minLimtTemp, mode);
	if(!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}

	return rt;
}
bool ZsTec::GetCtrlPara(int no, double& destTemp, double& maxTemp, 
		double& maxOutput, double& maxCurrent, double& maxLimtTemp, double& minLimtTemp, int& mode)
{
	if(m_pData->pTecObj==NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}

	if (!m_pData->pTecObj->JudgeTecValid(no))
	{
		strcpy(m_pData->errDesc, "the no is invalid");
		return false;
	}

	bool rt = m_pData->pTecObj->GetCtrlPara(no, destTemp, maxTemp, maxOutput, maxCurrent, maxLimtTemp, minLimtTemp, mode);
	if(!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}
int ZsTec::GetActTemp(double* temp, int total)
{
	if(m_pData->pTecObj==NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return 0;
	}
	int rt = m_pData->pTecObj->GetActTemp(temp, total);
	if(!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}


int ZsTec::GetActTempTec(double* temp, int total)
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return 0;
	}
	if (m_pData->mode != TEC_CUSTOM_CARD_1CH_NEW)
	{
		strcpy(m_pData->errDesc, "mode error");
		return 0;
	}

	int rt = m_pData->pTecObj->GetActTempTec(temp, total);
	if (!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}

int ZsTec::GetActTempWater(double& temp, double& temp2)
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return 0;
	}
	if (m_pData->mode != TEC_CUSTOM_CARD_1CH_NEW)
	{
		strcpy(m_pData->errDesc, "mode error");
		return 0;
	}

	int rt = m_pData->pTecObj->GetActTempWater(temp, temp2);
	if (!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}

bool ZsTec::CleanAlarmMsg()
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}
	if (m_pData->mode != TEC_CUSTOM_CARD_1CH_NEW)
	{
		strcpy(m_pData->errDesc, "mode error");
		return false;
	}

	return m_pData->pTecObj->CleanAlarmMsg();
}

bool ZsTec::IsNewProtocol()
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}

	if (m_pData->mode != TEC_CUSTOM_CARD_1CH_NEW)
	{
		return false;
	}

	return true;
}

bool ZsTec::NotifyLaserSwitchStatus(int nSwitchValue, int nTotalNum)
{
	if (TEC_CUSTOM_CARD_1CH_NEW == m_pData->mode &&  m_pData->pTecObj != NULL)
	{
		if (GetProObj().TecWriteTecLaserSwitch(nSwitchValue, nTotalNum))
		{
			return true;
		}
	}
	return false;
}

int ZsTec::IsTempExceed()
{
	//ASSERT(m_pData->pTecObj);
	int flagInit = 0;
	for (int i = 0; i< m_pData->total;++i)
	{
		flagInit |= (0x01 <<  i);
	}
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return flagInit;
	}

	if(!GetActTemp(m_pData->actTemp, m_pData->total))
		return flagInit;

	std::string strOverTemp = "";
	char szOverMsg[255] = { 0 };
	int flag = 0;
	for(int i=0; i<m_pData->total; i++)
	{
		if (!m_pData->pTecObj->JudgeTecValid(i+1))
			continue;

		if(m_pData->actTemp[i]> m_pData->pTecObj->m_stLimitTemp[i].dLimitMaxTemp
			|| m_pData->actTemp[i]< m_pData->pTecObj->m_stLimitTemp[i].dLimitMinTemp)
		{
			flag |= (0x01<<i);

			sprintf_s(szOverMsg, "tec%d:%.2f ", i + 1, m_pData->actTemp[i]);
			strOverTemp.append(szOverMsg);
		}
	}

	// 超温时，写入真实温度
	if (flag != 0)
	{
		sprintf_s(szOverMsg, " 温度范围%.2f-%.2f", m_pData->pTecObj->m_stLimitTemp[0].dLimitMinTemp, 
			m_pData->pTecObj->m_stLimitTemp[0].dLimitMaxTemp);
		strOverTemp.append(szOverMsg);
		m_pData->pTecObj->WriteLogMsg(strOverTemp);
	}

	// 新协议就获取一次异常信息
	if (TEC_CUSTOM_CARD_1CH_NEW == m_pData->mode && flag != 0)
	{
		std::string strError =  m_pData->pTecObj->ExceptionStrMsg();
		if (!strError.empty())
		{
			strcpy(m_pData->errDesc, strError.c_str());
		}
	}
	return flag;
}
bool ZsTec::IsTecOpen(int no, int& isOpen)//判断tec是否在运行
{
	if(m_pData->pTecObj==NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}
	// 先检测是否存在tec
	if (!m_pData->pTecObj->JudgeTecValid(no))
	{
		strcpy(m_pData->errDesc, "the no is invalid");
		return false;
	}

	bool rt = m_pData->pTecObj->IsTecOpen(no, isOpen);
	if(!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}

bool ZsTec::IsAllTecOpen(int& nOpenStatus)
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}
	int flagInit = 0;
	bool rt = m_pData->pTecObj->IsAllTecOpen(flagInit);
	if (!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	nOpenStatus = flagInit;

	return rt;
}


bool ZsTec::OpenAllTec(int nOpenStatus)
{
	if (m_pData->pTecObj == NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}
	bool rt = m_pData->pTecObj->OpenAllTec(nOpenStatus);
	if (!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}

bool ZsTec::OpenTec(int no, int en)//启动或停止tec
{  
	if(m_pData->pTecObj==NULL)
	{
		strcpy(m_pData->errDesc, "tec not init");
		return false;
	}

	// 先检测是否存在tec
	if (!m_pData->pTecObj->JudgeTecValid(no))
	{
		strcpy(m_pData->errDesc, "the no is invalid");
		return false;
	}

	bool rt = m_pData->pTecObj->OpenTec(no, en);
	if(!rt)
	{
		strcpy(m_pData->errDesc, m_pData->pTecObj->GetErrDesc());
	}
	return rt;
}

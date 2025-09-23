#ifndef _ZS_TEC_DLL_H_
#define _ZS_TEC_DLL_H_

enum TecType
{
	TEC_3CARD_2CH,		//3个tec，每个2通道
	TEC_6CARD_1CH,		//6个tec，每个1通道
	TEC_6CARD_2CH_NEW,	//6个tec，每个2通道
	TEC_7CARD_2CH_NEW,	//7个tec，每个2通道
	TEC_6CARD_1CH_NEW,	//6个tec，每个1通道
	TEC_12CARD_1CH,		//12个tec，1个通道
	TEC_CUSTOM_CARD_1CH_NEW,
};


class __declspec(dllimport) ZsTec
{
protected:
	ZsTec();
public:
	~ZsTec();
	friend ZsTec& GetTecObj();
	const char* GetErrDesc();
public:
	void SetPara(int com1, int com2, int com3, int mode);
	void SetPara(int com, int mode);
//	void SetPara(int com, int total);
	void SetLmtTemp(double maxTemp, double minTemp);
	//mode代表tec类型，存在多种方式
	void GetPara(int& mode, int& com1, int& com2, int& com3, 
		double& maxTemp, double& minTemp, int& chTotal);

	void SetTecExistStatus(int& nTecExistStatus);
	int  GetTecExistStatus();

	void SetLogFlag(int nLogFlag);		// 应对新协议的日志开关
	void GetLogFlag(int& nLogFlag);		// 应对新协议的日志开关
	//void GetLogRefreshCycle(int& nLogRefreshCycle);		// 应对新协议的日志刷新周期
	//void SetLogRefreshCycle(int nLogRefreshCycle);		// 应对新协议的日志刷新周期
	
	int Init();//0表示失败，按bit表示某路是否成功
	bool IsInitSucceed();	// 初始化是否成功
	bool IsInitRun();		// 是否执行过初始化
	bool Exit();
	bool Load();
	bool Save();
public:
	//base 1
	bool SetCtrlPara(int no, double destTemp, double maxTemp, 
			double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode);
	bool GetCtrlPara(int no, double& destTemp, double& maxTemp, 
			double& maxOutput, double& maxCurrent, double &maxLimtTemp, double &minLimtTemp, int& mode);

public:
	int GetActTemp(double* temp, int total);//返回有效个数,如果温度查询失败，温度值置为100度
	int GetActTempTec(double* temp, int total);	//Tec温度 
	int GetActTempWater(double& temp, double& temp2);			//水温 
	bool CleanAlarmMsg();						// 清除tec报警信息
	bool IsNewProtocol();							// 是否是新协议
	bool NotifyLaserSwitchStatus(int nSwitchValue, int nTotalNum);	// 通知激光器开关状态
	int IsTempExceed();//以bit来设置是否有超温标志，如果温度查询失败，以超温表示
	bool IsTecOpen(int no, int& isOpen);//判断tec是否在运行
	bool OpenTec(int no, int en);//启动或停止tec
	bool IsAllTecOpen(int& nOpenStatus); //判断所有tec是否在运行，按位计算
	bool OpenAllTec(int nOpenStatus); //启动或停止指定的tec，可指定所有，按位计算
protected:
	void* m_pReserved;

};

#endif//_ZS_TEC_DLL_H_

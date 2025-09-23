#ifndef _ZS_TEC_DLL_H_
#define _ZS_TEC_DLL_H_

#include "TecV1.h"
#include "TecV2.h"
#include "TecV3.h"
#include "TecNewPro.h"

struct TecDataBlk
{
	int mode;
	int com, com2, com3, com4;
	int total;
	int TecExist;
	double maxTemp, minTemp;
	bool bInitRun;
	bool bIsPowerDouble;
	int nLogFlag;
	int nLogRefreshCycle;

	double actTemp[14];//not need save
	CTecBase* pTecObj;
	char errDesc[255*5];

	char paraFile[255];

	TecDataBlk()
	{
		mode = 0;
		com = 0;
		com2 = 0;
		com3 = 0;
		com4 = 0;
		total = 6;
		TecExist = 0x3f;
		maxTemp = 25;
		minTemp = 10;
		nLogFlag = 0;
		nLogRefreshCycle = 0;
		bInitRun = false;
		bIsPowerDouble = false;

		pTecObj = NULL;
		errDesc[0] = 0;
	}
};
enum TecType
{
	TEC_3CARD_2CH,
	TEC_6CARD_1CH,
	TEC_6CARD_2CH_NEW,
	TEC_7CARD_2CH_NEW,
	TEC_6CARD_1CH_NEW,
	TEC_12CARD_1CH,
	TEC_CUSTOM_CARD_1CH_NEW,
};

class __declspec(dllexport) ZsTec
{
protected:
	ZsTec();
public:
	~ZsTec();
	__declspec(dllexport) friend ZsTec& GetTecObj();
	const char* GetErrDesc();
public:
	void SetPara(int com1, int com2, int com3, int mode);
	void SetPara(int com, int mode);
//	void SetPara(int com, int chTotal);
	void SetLmtTemp(double maxTemp, double minTemp);
	
	void GetPara(int& mode, int& com1, int& com2, int& com3, 
		double& maxTemp, double& minTemp, int& chTotal);
	void SetTecExistStatus(int& nTecExistStatus);
	int  GetTecExistStatus();

	void SetLogFlag(int nLogFlag);		// Ӧ����Э�����־����
	void GetLogFlag(int& nLogFlag);		// Ӧ����Э�����־����
	void GetLogRefreshCycle(int& nLogRefreshCycle);		// Ӧ����Э�����־ˢ������
	void SetLogRefreshCycle(int nLogRefreshCycle);		// Ӧ����Э�����־ˢ������
	

	int Init();//0��ʾʧ�ܣ���bit��ʾĳ·�Ƿ�ɹ�
	bool IsInitSucceed();	// ��ʼ���Ƿ�ɹ�
	bool IsInitRun();		// �Ƿ�ִ�й���ʼ��
	bool Exit();
	bool Load();
	bool Save();
public:
	//base 1
	bool SetCtrlPara(int no, double destTemp, double maxTemp, 
			double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode);
	bool GetCtrlPara(int no, double& destTemp, double& maxTemp, 
			double& maxOutput, double& maxCurrent, double& maxLimtTemp, double& minLimtTemp, int& mode);
public:
	int GetActTemp(double* temp, int total);	//dmd�¶� ������Ч����,����¶Ȳ�ѯʧ�ܣ��¶�ֵ��Ϊ100��
	int GetActTempTec(double* temp, int total);	//Tec�¶� 
	int GetActTempWater(double& temp, double& temp2);			//ˮ�� 
	bool CleanAlarmMsg();						// ���tec������Ϣ
	bool IsNewProtocol();							// �Ƿ�����Э��
	bool NotifyLaserSwitchStatus(int nSwitchValue, int nTotalNum);		// ֪ͨ����������״̬
	int IsTempExceed();//��bit�������Ƿ��г��±�־������¶Ȳ�ѯʧ�ܣ��Գ��±�ʾ
	bool IsTecOpen(int no, int& isOpen);//�ж�tec�Ƿ�������
	bool OpenTec(int no, int en);//������ֹͣtec
	bool IsAllTecOpen(int& nOpenStatus); //�ж�����tec�Ƿ������У���λ����
	bool OpenAllTec(int nOpenStatus); //������ָֹͣ����tec����ָ�����У���λ����
protected:
	TecDataBlk* m_pData;

};

#endif//_ZS_TEC_DLL_H_

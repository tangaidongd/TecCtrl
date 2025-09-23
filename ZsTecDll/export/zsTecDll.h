#ifndef _ZS_TEC_DLL_H_
#define _ZS_TEC_DLL_H_

enum TecType
{
	TEC_3CARD_2CH,		//3��tec��ÿ��2ͨ��
	TEC_6CARD_1CH,		//6��tec��ÿ��1ͨ��
	TEC_6CARD_2CH_NEW,	//6��tec��ÿ��2ͨ��
	TEC_7CARD_2CH_NEW,	//7��tec��ÿ��2ͨ��
	TEC_6CARD_1CH_NEW,	//6��tec��ÿ��1ͨ��
	TEC_12CARD_1CH,		//12��tec��1��ͨ��
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
	//mode����tec���ͣ����ڶ��ַ�ʽ
	void GetPara(int& mode, int& com1, int& com2, int& com3, 
		double& maxTemp, double& minTemp, int& chTotal);

	void SetTecExistStatus(int& nTecExistStatus);
	int  GetTecExistStatus();

	void SetLogFlag(int nLogFlag);		// Ӧ����Э�����־����
	void GetLogFlag(int& nLogFlag);		// Ӧ����Э�����־����
	//void GetLogRefreshCycle(int& nLogRefreshCycle);		// Ӧ����Э�����־ˢ������
	//void SetLogRefreshCycle(int nLogRefreshCycle);		// Ӧ����Э�����־ˢ������
	
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
			double& maxOutput, double& maxCurrent, double &maxLimtTemp, double &minLimtTemp, int& mode);

public:
	int GetActTemp(double* temp, int total);//������Ч����,����¶Ȳ�ѯʧ�ܣ��¶�ֵ��Ϊ100��
	int GetActTempTec(double* temp, int total);	//Tec�¶� 
	int GetActTempWater(double& temp, double& temp2);			//ˮ�� 
	bool CleanAlarmMsg();						// ���tec������Ϣ
	bool IsNewProtocol();							// �Ƿ�����Э��
	bool NotifyLaserSwitchStatus(int nSwitchValue, int nTotalNum);	// ֪ͨ����������״̬
	int IsTempExceed();//��bit�������Ƿ��г��±�־������¶Ȳ�ѯʧ�ܣ��Գ��±�ʾ
	bool IsTecOpen(int no, int& isOpen);//�ж�tec�Ƿ�������
	bool OpenTec(int no, int en);//������ֹͣtec
	bool IsAllTecOpen(int& nOpenStatus); //�ж�����tec�Ƿ������У���λ����
	bool OpenAllTec(int nOpenStatus); //������ָֹͣ����tec����ָ�����У���λ����
protected:
	void* m_pReserved;

};

#endif//_ZS_TEC_DLL_H_

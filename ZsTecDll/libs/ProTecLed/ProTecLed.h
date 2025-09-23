#ifndef _PRO_TEC_LED_H_
#define _PRO_TEC_LED_H_



class __declspec(dllexport) CProTecLed
{
protected:
	CProTecLed();
public:
	~CProTecLed();
	__declspec(dllexport) friend CProTecLed& GetProObj();
public:
	// ����ָ��
	const char* GetErrDesc();
	bool OpenCom(const char* pPortname);
	bool CloseCom();
	bool ComIsOpen();
	/**********************************************************************
	* ��������:	���õ�Դ��ַ
	* �������:	nAddress			��Դ��ַ
	***********************************************************************/
	bool SetPowerAddress(int nAddress);
	/**********************************************************************
	* ��������:	�����Ƿ��Ƕ����Դ��ַ
	* �������:	nIsPowerDouble	 �Ƿ�Ϊ˫��Դ
	***********************************************************************/
	bool SetPowerDouble(bool bIsPowerDouble);
	/**********************************************************************
	* ��������:	��ȡ��ذ汾
	* �������	nLedNum				LED����
	* �������	nTecNum				TEC����
	* �������	nBaudrate			������
	* �������	nProVersion			Э��汾
	* �������	nDriverVersion		Ӳ�������汾
	***********************************************************************/
	bool CommonReadVersion(int& nLedNum, int& nTecNum, int& nBaudrate, int& nProVersion, int& nDriverVersion);

	/**********************************************************************
	* ��������:	��ȡ�豸����״̬
	* �������	nWorkStatus			ÿ���豸�Ĺ���״̬ 0 ֹͣ 1����
	* �������	nTotalNum			�����ĸ�����������Ϊ�ж��ٸ��豸
	***********************************************************************/
	bool CommonReadWorkStatus(unsigned int& nWorkStatus, int& nTotalNum);

	/**********************************************************************
	* ��������:	��ȡͨ������
	* �������	zLedValue			LEDͨ������ֵ����
	* �������	nLedNum				LED��Ŀ������ƥ��
	* �������	zTecValue			TECͨ������ֵ
	* �������	nTecNum				TEC��Ŀ������ƥ��
	***********************************************************************/
	bool CommonReadChannalElectric(int* zLedValue, int& nLedNum, int* zTecValue, int& nTecNum);	   		
	
	/**********************************************************************
	* ��������:	��ȡ�����쳣��־
	* �������	zAllException		�����豸���쳣��־
	* �������	nTotalNum			�豸����
	* �������	nAlarm				�ܸ澯
	***********************************************************************/
	bool CommonReadAllExceptionFlag(int* zAllException, int& nTotalNum, int& nAlarm);	   		
	
	/**********************************************************************
	* ��������:	����쳣��־
	* �������	zAllException		�����豸���쳣��־
	* �������	nTotalNum			�豸����
	* �������	nAlarm				�ܸ澯
	***********************************************************************/
	bool CommonControlClearExceptionFlag(int nAddress);		// ����쳣

	/**********************************************************************
	* ��������:	��ȡDMD�¶�
	* �������	zAllException		�����豸���쳣��־
	* �������	nTotalNum			�豸����
	* �������	nAlarm				�ܸ澯
	***********************************************************************/
	bool CommonReadDmdTemperature(int* zTecValue, int& nTotalNum);		   	

	/**********************************************************************
	* ��������:	��ȡLED�¶�
	* �������	zLedValue			LED�¶�
	* �������	nTotalNum			LED�¶���Ŀ
	***********************************************************************/
	bool CommonReadLedTemperature(int* zLedValue, int& nTotalNum);

	/**********************************************************************
	* ��������:	��ȡ���ư��¶Ⱥͷ���ת��
	* �������	nLedTemp			LED�¶�
	***********************************************************************/
	bool CommonReadBoardTemp_fan_speed(int& nBoardTemp, int& nFanSpeed);   	

	/**********************************************************************
	* ��������:	�����ͺ�
	* �������	nLedNum				LED��Ŀ
	* �������	nTecNum				TEC��Ŀ
	***********************************************************************/
	bool CommonWriteModel(int nLedNum, int nTecNum);	 

	/**********************************************************************
	* ��������:	��ȡ�ͺ�
	* �������	nLedNum				LED��Ŀ
	* �������	nTecNum				TEC��Ŀ
	* �������	nBaudrate			������
	***********************************************************************/
	bool CommonReadModel(int& nLedNum, int& nTecNum, int& nBaudrate);				

	/**********************************************************************
	* ��������:	���õ����������
	* �������	zElectricLimit		����������ֵ
	* �������	nTotalNum			�豸������ͬ����������Чֵ��С
	***********************************************************************/
	bool CommonWriteOutputElectricLimit(int* zElectricLimit, int nTotalNum); 		

	/**********************************************************************
	* ��������:	��ȡ�����������
	* �������	zElectricLimit		����������ֵ
	* �������	nTotalNum			�豸������ͬ����������Чֵ��С
	***********************************************************************/
	bool CommonReadOutputElectricLimit(int* zElectricLimit, int& nTotalNum);  

	/**********************************************************************
	* ��������:	���õ���У׼ֵ
	* �������	zElectricCali		����У׼ֵ����
	* �������	nTotalNum			�豸������ͬ����������Чֵ��С
	***********************************************************************/
	bool CommonWriteElectricCalibration(int* zElectricCali, int nTotalNum);  	

	/**********************************************************************
	* ��������:	��ȡ����У׼ֵ
	* �������	zElectricCali		����У׼ֵ����
	* �������	nTotalNum			�豸������ͬ����������Чֵ��С
	***********************************************************************/
	bool CommonReadElectricCalibration(int* zElectricCali, int& nTotalNum);   	// 


	/**********************************************************************
	* ��������:	��ȡ���������
	* �������	zCustomValue		������ֵ ��һλ�������׶Σ������ʧ��ԭ�� �ڶ�λ������Pֵ������������Iֵ������������Dֵ
	* �������	nTotalNum			������������
	***********************************************************************/
	bool CommonReadCustomValue(int* zCustomValue, int& nValueNum);   				// ��ȡ���������


	bool CommonReadTecTemperature(int* zTecValue, int& nTotalNum);			
	bool CommonReadTecColdWaterTemperature(int* zTecColdValue, int& nTotalNum);	// ��ȡTEC��ˮ���¶�
	bool CommonReadDmdEnvironmentTemp(int* zTecValue, int& nTotalNum);			// ��ȡDMD�����¶�
	bool CommonReadDmdEnvironmentHumidity(int* zTecValue, int& nTotalNum);		// ��ȡDMD�����¶�

	// TECָ�
	/**********************************************************************
	* ��������:	д��TEC�¿ؿ���
	* �������	zTempSwitch			Ϊ�տ��ذ���λ���
	* �������	nTotalNum			�¿ؿ�����Ŀ
	***********************************************************************/
	bool TecWriteTempControlSwitch(int zTempSwitch, int nTotalNum);	   		// 

	/**********************************************************************
	* ��������:	��ȡTEC�¿ؿ���
	* �������	zTempSwitch			Ϊ�տ��ذ���λ���
	* �������	nTotalNum			�¿ؿ�����Ŀ
	***********************************************************************/
	bool TecReadTempControlSwitch(int& zTempSwitch, int& nTotalNum);	   		// 

	/**********************************************************************
	* ��������:	д��DMD�¿�ֵ
	* �������	zTempValue			�¿�ֵ����
	* �������	nTotalNum			�¿������С
	***********************************************************************/
	bool TecWriteDmdTempControlValue(int* zTempValue, int nTotalNum);	   		// д��TEC�¿�ֵ

	/**********************************************************************
	* ��������:	д��DMD�¿�ֵ
	* �������	zTempValue			�¿�ֵ����
	* �������	nTotalNum			�¿������С
	***********************************************************************/
	bool TecReadDmdTempControlValue(int* zTempValue, int& nTotalNum);		   		// ��ȡTECͨ��ֵ


	/**********************************************************************
	* ��������:	д��DMD�¶�����
	* �������	zTempMaxLimit		����¶�����ֵ����
	* �������	nTotalNum			���������С
	***********************************************************************/
	bool TecWriteDmdTempControlMaxLimit(int* zTempMaxLimit, int nTotalNum);   		// 

	/**********************************************************************
	* ��������:	��ȡDMD�¶�����
	* �������	zTempMaxLimit		����¿�����ֵ����
	* �������	nTotalNum			���������С
	***********************************************************************/
	bool TecReadDmdTempControlMaxLimit(int* zTempMaxLimit, int& nTotalNum);	   		// 

	/**********************************************************************
	* ��������:	д��DMD�¿�����
	* �������	zTempMinLimit		��С�¿�����ֵ����
	* �������	nTotalNum			���������С
	***********************************************************************/
	bool TecWriteDmdTempControlMinLimit(int* zTempMinLimit, int nTotalNum);   		// 

	/**********************************************************************
	* ��������:	��ȡDMD�¶�����
	* �������	zTempMinLimit		��С�¿�����ֵ����
	* �������	nTotalNum			���������С
	***********************************************************************/
	bool TecReadDmdTempControlMinLimit(int* zTempMinLimit, int& nTotalNum);	   		// ��ȡTEC�¶�����


	/**********************************************************************
	* ��������:	д��TEC��Pֵ
	* �������	zValueP				Pֵ����
	* �������	nTotalNum			Pֵ�����С
	***********************************************************************/
	bool TecWriteValueP(int* zValueP, int nTotalNum);				   	// 

	/**********************************************************************
	* ��������:	��ȡTEC��Pֵ
	* �������	zValueP				Pֵ����
	* �������	nTotalNum			Pֵ�����С
	***********************************************************************/
	bool TecReadValueP(int* zValueP, int& nTotalNum);				   	// 

	/**********************************************************************
	* ��������:	д��TEC��Iֵ
	* �������	zValueI				Iֵ����
	* �������	nTotalNum			Iֵ�����С
	***********************************************************************/
	bool TecWriteValueI(int* zValueI, int nTotalNum);				   	// 

	/**********************************************************************
	* ��������:	��ȡTEC��Iֵ
	* �������	zValueI				Iֵ����
	* �������	nTotalNum			Iֵ�����С
	***********************************************************************/
	bool TecReadValueI(int* zValueI, int& nTotalNum);				   	// 


	/**********************************************************************
	* ��������:	д��TEC��Dֵ
	* �������	zValueD				Dֵ����
	* �������	nTotalNum			Dֵ�����С
	***********************************************************************/
	bool TecWriteValueD(int* zValueD, int nTotalNum);				   	// 

	/**********************************************************************
	* ��������:	��ȡTEC��Dֵ
	* �������	zValueD				Dֵ����
	* �������	nTotalNum			Dֵ�����С
	***********************************************************************/
	bool TecReadValueD(int* zValueD, int& nTotalNum);				   	// 

	/**********************************************************************
	* ��������:	д�����������غ�ͨ��
	* �������	zValue				д��������ֵ ��һλ���������أ��ڶ�λ������TECͨ�����
	* �������	nValueNum			���������С
	***********************************************************************/
	bool TecWriteCustomSwitchAndChannal(int* zValue, int nValueNum);	// 

	/**********************************************************************
	* ��������:	��ȡ���������غ�ͨ��
	* �������	zValue				д��������ֵ ��һλ���������أ��ڶ�λ������TECͨ�����
	* �������	nValueNum			���������С
	***********************************************************************/
	bool TecReadCustomSwitchAndChannal(int* zValue, int& nValueNum);	// 

	bool TecWriteTecTempMaxLimit(int* zTempValue, int nTotalNum);			// д��TEC�¶�����
	bool TecReadTecTempMaxLimit(int* zTempValue, int& nTotalNum);			// ��ȡTEC�¶�����
	bool TecWriteTecTempMinLimit(int* zTempValue, int nTotalNum);				// д��TEC�¶�����
	bool TecReadTecTempMinLimit(int* zTempValue, int& nTotalNum);				// ��ȡTEC�¶�����
	bool TecWriteColdBlockTempMaxLimit(int* zTempValue, int nTotalNum);		// д����ˮ���¶����� �����nTotalNum����tec�豸��ĿӦ��Ϊ1
	bool TecReadColdBlockTempMaxLimit(int* zTempValue, int& nTotalNum);		// ��ȡ��ˮ���¶����� 
	bool TecWriteColdBlockTempMinLimit(int* zTempValue, int nTotalNum);		// д����ˮ���¶����� �����nTotalNum����tec�豸��ĿӦ��Ϊ1	
	bool TecReadColdBlockTempMinLimit(int* zTempValue, int& nTotalNum);		// ��ȡ��ˮ���¶�����
	bool TecWriteHumidityAlarmSwitch(int nSwitch);							// д��ʪ�ȱ������� 0��ʾ�����ʪ�ȣ�1��ʾ���ʪ��,  Ĭ�ϼ��
	bool TecReadHumidityAlarmSwitch(int& nSwitch);							// ��ȡʪ�ȱ�������
	bool TecWriteSelfCheckMode(int nSelfCheck);								// �����Լ�ģʽ
	bool TecReadSelfCheckMode(int& nSelfCheck);								// ��ѯ�Լ�ģʽ
	bool TecWriteHumidityAlarm(int* zTempValue, int nTotalNum);			    // д��ʪ�ȳ��߱���ֵ
	bool TecReadHumidityAlarm(int* zTempValue, int& nTotalNum);				// ��ȡʪ�ȳ��߱���ֵ
	bool TecWriteDmdColdBlockDiffPositive(int* zDiffValue, int nTotalNum);			// д��DMD����ˮ���²������ֵ
	bool TecReadDmdColdBlockDiffPositive(int* zDiffValue, int& nTotalNum);			// ��ȡDMD����ˮ���²������ֵ
	bool TecWriteDmdColdBlockDiffNegative(int* zNegativeValue, int nTotalNum);		// д��DMD����ˮ���²����ֵ
	bool TecReadDmdColdBlockDiffNegative(int* zNegativeValue, int& nTotalNum);		// ��ȡDMD����ˮ���²����ֵ
	bool TecWriteTecAlarmThresholdElectric(int* zElectricValue, int nTotalNum);		// д��TEC������ֵ����
	bool TecReadTecAlarmThresholdElectric(int* zElectricValue, int& nTotalNum);		// ��ȡTEC������ֵ����
	bool TecWriteTecAlarmThresholdTime(int* zTimeValue, int nTotalNum);				// д��TEC��ֵ����ʱ��
	bool TecReadTecAlarmThresholdTime(int* zTimeValue, int& nTotalNum);			// ��ȡTEC��ֵ����ʱ��

	bool TecWriteTecAdvenceEnableTemp(int nEnableValue, int nTotalNum);			// ����TEC��ǰ����ʹ��
	bool TecReadTecAdvenceEnableTemp(int& nEnableValue, int& nTotalNum);		// ��ȡTEC��ǰ����ʹ��
	bool TecWriteTecLaserSwitch(int nSwitchValue, int nTotalNum);				// д�뼤��������
	bool TecReadTecLaserSwitch(int& nSwitchValue, int& nTotalNum);				// ��ȡ����������

	// LEDָ�
	/**********************************************************************
	* ��������:	д��LEDͨ������
	* �������	zCh					ͨ�����ذ�λ����
	* �������	nTotalNum			ͨ����Ŀ
	***********************************************************************/
	bool LedWriteChannalSwitch(int zCh, int nTotalNum);		   	// 

	/**********************************************************************
	* ��������:	��ȡLEDͨ������
	* �������	zLedValue			ͨ�����ذ�λ����
	* �������	nTotalNum			ͨ����Ŀ
	***********************************************************************/
	bool LedReadChannalSwitch(int& zLedValue, int& nTotalNum);			// 

	/**********************************************************************
	* ��������:	д��LEDͨ�������趨ֵ
	* �������	zChannalValue		ͨ������ֵ
	* �������	nTotalNum			ͨ����Ŀ
	***********************************************************************/
	bool LedWriteChannalElectric(int* zChannalValue, int nTotalNum);		   	// 


	/**********************************************************************
	* ��������:	��ȡLEDͨ�������趨ֵ
	* �������	zChannalValue		ͨ������ֵ
	* �������	nTotalNum			ͨ����Ŀ
	***********************************************************************/
	bool LedReadChannalElectric(int* zChannalValue, int& nTotalNum);		   	

	/**********************************************************************
	* ��������:	����LED�¿�����
	* �������	nControlNum		�¿�����
	***********************************************************************/
	bool LedWriteTempControlNum(int nControlNum);		   	// 

	/**********************************************************************
	* ��������:	��ȡLED�¿�����
	* �������	nControlNum		�¿�����
	***********************************************************************/
	bool LedReadTempControlNum(int& nControlNum);		   	// 

	/**********************************************************************
	* ��������:	д��LED�¶�����
	
	* �������	zTempMaxLimit		�¿�����ֵ
	* �������	nTotalNum		�¿�����ֵ��Ŀ����������
	***********************************************************************/
	bool LedWriteTempControlMaxLimit(int* zTempMaxLimit, int nTotalNum);   	// 

	/**********************************************************************
	* ��������:	��ȡLED�¿�����
	* �������	zTempMaxLimit		�¿�����ֵ
	* �������	nTotalNum			�¿�����ֵ��Ŀ
	***********************************************************************/
	bool LedReadTempControlMaxLimit(int* zTempMaxLimit, int& nTotalNum);	   	// 

	/**********************************************************************
	* ��������:	��ȡLED�¿�����
	* �������	zTempMinLimit		�¿�����ֵ
	* �������	nTotalNum			�¿�����ֵ��Ŀ
	***********************************************************************/
	bool LedReadTempControlMinLimit(int* zTempMinLimit, int& nTotalNum);	   	// 

	/**********************************************************************
	* ��������:	д��LED�¶�����
	* �������	zTempMinLimit		�¿�����ֵ
	* �������	nTotalNum			�¿�ֵ��Ŀ����������
	***********************************************************************/
	bool LedWriteTempControlMinLimit(int* zTempMinLimit, int nTotalNum);   	// 

	/**********************************************************************
	* ��������:	�������е�ԴLEDͨ��
	* �������	nTempMinLimit		�¿�����ֵ
	***********************************************************************/
	bool LedWriteOpenCloseAllPowerControl(int nPowerCount);	   	// ��������LEDͨ��


protected:
	void* m_FunProTecLed;

};

























#endif // !_PRO_TEC_LED_H_

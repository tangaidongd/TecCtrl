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
	// 公共指令
	const char* GetErrDesc();
	bool OpenCom(const char* pPortname);
	bool CloseCom();
	bool ComIsOpen();
	/**********************************************************************
	* 功能描述:	设置电源地址
	* 输入参数:	nAddress			电源地址
	***********************************************************************/
	bool SetPowerAddress(int nAddress);
	/**********************************************************************
	* 功能描述:	设置是否是多个电源地址
	* 输入参数:	nIsPowerDouble	 是否为双电源
	***********************************************************************/
	bool SetPowerDouble(bool bIsPowerDouble);
	/**********************************************************************
	* 功能描述:	获取相关版本
	* 输出参数	nLedNum				LED数量
	* 输出参数	nTecNum				TEC数量
	* 输出参数	nBaudrate			波特率
	* 输出参数	nProVersion			协议版本
	* 输出参数	nDriverVersion		硬件驱动版本
	***********************************************************************/
	bool CommonReadVersion(int& nLedNum, int& nTecNum, int& nBaudrate, int& nProVersion, int& nDriverVersion);

	/**********************************************************************
	* 功能描述:	获取设备工作状态
	* 输出参数	nWorkStatus			每个设备的工作状态 0 停止 1工作
	* 输出参数	nTotalNum			参数的个数，可以认为有多少个设备
	***********************************************************************/
	bool CommonReadWorkStatus(unsigned int& nWorkStatus, int& nTotalNum);

	/**********************************************************************
	* 功能描述:	获取通道电流
	* 输出参数	zLedValue			LED通道电流值数组
	* 输出参数	nLedNum				LED数目与数组匹配
	* 输出参数	zTecValue			TEC通道电流值
	* 输出参数	nTecNum				TEC数目与数组匹配
	***********************************************************************/
	bool CommonReadChannalElectric(int* zLedValue, int& nLedNum, int* zTecValue, int& nTecNum);	   		
	
	/**********************************************************************
	* 功能描述:	读取所有异常标志
	* 输出参数	zAllException		所有设备的异常标志
	* 输出参数	nTotalNum			设备总数
	* 输出参数	nAlarm				总告警
	***********************************************************************/
	bool CommonReadAllExceptionFlag(int* zAllException, int& nTotalNum, int& nAlarm);	   		
	
	/**********************************************************************
	* 功能描述:	清除异常标志
	* 输出参数	zAllException		所有设备的异常标志
	* 输出参数	nTotalNum			设备总数
	* 输出参数	nAlarm				总告警
	***********************************************************************/
	bool CommonControlClearExceptionFlag(int nAddress);		// 清除异常

	/**********************************************************************
	* 功能描述:	读取DMD温度
	* 输出参数	zAllException		所有设备的异常标志
	* 输出参数	nTotalNum			设备总数
	* 输出参数	nAlarm				总告警
	***********************************************************************/
	bool CommonReadDmdTemperature(int* zTecValue, int& nTotalNum);		   	

	/**********************************************************************
	* 功能描述:	获取LED温度
	* 输出参数	zLedValue			LED温度
	* 输出参数	nTotalNum			LED温度数目
	***********************************************************************/
	bool CommonReadLedTemperature(int* zLedValue, int& nTotalNum);

	/**********************************************************************
	* 功能描述:	读取控制板温度和风扇转速
	* 输出参数	nLedTemp			LED温度
	***********************************************************************/
	bool CommonReadBoardTemp_fan_speed(int& nBoardTemp, int& nFanSpeed);   	

	/**********************************************************************
	* 功能描述:	设置型号
	* 输出参数	nLedNum				LED数目
	* 输出参数	nTecNum				TEC数目
	***********************************************************************/
	bool CommonWriteModel(int nLedNum, int nTecNum);	 

	/**********************************************************************
	* 功能描述:	读取型号
	* 输出参数	nLedNum				LED数目
	* 输出参数	nTecNum				TEC数目
	* 输出参数	nBaudrate			波特率
	***********************************************************************/
	bool CommonReadModel(int& nLedNum, int& nTecNum, int& nBaudrate);				

	/**********************************************************************
	* 功能描述:	设置电流输出上限
	* 输入参数	zElectricLimit		电流限制数值
	* 输入参数	nTotalNum			设备总数等同电流数组有效值大小
	***********************************************************************/
	bool CommonWriteOutputElectricLimit(int* zElectricLimit, int nTotalNum); 		

	/**********************************************************************
	* 功能描述:	读取电流输出上限
	* 输出参数	zElectricLimit		电流限制数值
	* 输出参数	nTotalNum			设备总数等同电流数组有效值大小
	***********************************************************************/
	bool CommonReadOutputElectricLimit(int* zElectricLimit, int& nTotalNum);  

	/**********************************************************************
	* 功能描述:	设置电流校准值
	* 输入参数	zElectricCali		电流校准值数组
	* 输入参数	nTotalNum			设备总数等同电流数组有效值大小
	***********************************************************************/
	bool CommonWriteElectricCalibration(int* zElectricCali, int nTotalNum);  	

	/**********************************************************************
	* 功能描述:	读取电流校准值
	* 输出参数	zElectricCali		电流校准值数组
	* 输出参数	nTotalNum			设备总数等同电流数组有效值大小
	***********************************************************************/
	bool CommonReadElectricCalibration(int* zElectricCali, int& nTotalNum);   	// 


	/**********************************************************************
	* 功能描述:	读取自整定结果
	* 输出参数	zCustomValue		自整定值 第一位自整定阶段，结果及失败原因 第二位自整定P值，第三自整定I值，第四自整定D值
	* 输出参数	nTotalNum			自整定数据数
	***********************************************************************/
	bool CommonReadCustomValue(int* zCustomValue, int& nValueNum);   				// 读取自整定结果


	bool CommonReadTecTemperature(int* zTecValue, int& nTotalNum);			
	bool CommonReadTecColdWaterTemperature(int* zTecColdValue, int& nTotalNum);	// 读取TEC冷水块温度
	bool CommonReadDmdEnvironmentTemp(int* zTecValue, int& nTotalNum);			// 读取DMD环境温度
	bool CommonReadDmdEnvironmentHumidity(int* zTecValue, int& nTotalNum);		// 读取DMD环境温度

	// TEC指令集
	/**********************************************************************
	* 功能描述:	写入TEC温控开关
	* 输入参数	zTempSwitch			为空开关按照位存放
	* 输入参数	nTotalNum			温控开关数目
	***********************************************************************/
	bool TecWriteTempControlSwitch(int zTempSwitch, int nTotalNum);	   		// 

	/**********************************************************************
	* 功能描述:	读取TEC温控开关
	* 输出参数	zTempSwitch			为空开关按照位存放
	* 输出参数	nTotalNum			温控开关数目
	***********************************************************************/
	bool TecReadTempControlSwitch(int& zTempSwitch, int& nTotalNum);	   		// 

	/**********************************************************************
	* 功能描述:	写入DMD温控值
	* 输入参数	zTempValue			温控值数组
	* 输入参数	nTotalNum			温控数组大小
	***********************************************************************/
	bool TecWriteDmdTempControlValue(int* zTempValue, int nTotalNum);	   		// 写入TEC温控值

	/**********************************************************************
	* 功能描述:	写入DMD温控值
	* 输出参数	zTempValue			温控值数组
	* 输出参数	nTotalNum			温控数组大小
	***********************************************************************/
	bool TecReadDmdTempControlValue(int* zTempValue, int& nTotalNum);		   		// 读取TEC通控值


	/**********************************************************************
	* 功能描述:	写入DMD温度上限
	* 输入参数	zTempMaxLimit		最大温度限制值数组
	* 输入参数	nTotalNum			限制数组大小
	***********************************************************************/
	bool TecWriteDmdTempControlMaxLimit(int* zTempMaxLimit, int nTotalNum);   		// 

	/**********************************************************************
	* 功能描述:	读取DMD温度上限
	* 输出参数	zTempMaxLimit		最大温控限制值数组
	* 输出参数	nTotalNum			限制数组大小
	***********************************************************************/
	bool TecReadDmdTempControlMaxLimit(int* zTempMaxLimit, int& nTotalNum);	   		// 

	/**********************************************************************
	* 功能描述:	写入DMD温控下限
	* 输入参数	zTempMinLimit		最小温控限制值数组
	* 输入参数	nTotalNum			限制数组大小
	***********************************************************************/
	bool TecWriteDmdTempControlMinLimit(int* zTempMinLimit, int nTotalNum);   		// 

	/**********************************************************************
	* 功能描述:	读取DMD温度下限
	* 输出参数	zTempMinLimit		最小温控限制值数组
	* 输出参数	nTotalNum			限制数组大小
	***********************************************************************/
	bool TecReadDmdTempControlMinLimit(int* zTempMinLimit, int& nTotalNum);	   		// 读取TEC温度下限


	/**********************************************************************
	* 功能描述:	写入TEC的P值
	* 输入参数	zValueP				P值数组
	* 输入参数	nTotalNum			P值数组大小
	***********************************************************************/
	bool TecWriteValueP(int* zValueP, int nTotalNum);				   	// 

	/**********************************************************************
	* 功能描述:	读取TEC的P值
	* 输出参数	zValueP				P值数组
	* 输出参数	nTotalNum			P值数组大小
	***********************************************************************/
	bool TecReadValueP(int* zValueP, int& nTotalNum);				   	// 

	/**********************************************************************
	* 功能描述:	写入TEC的I值
	* 输入参数	zValueI				I值数组
	* 输入参数	nTotalNum			I值数组大小
	***********************************************************************/
	bool TecWriteValueI(int* zValueI, int nTotalNum);				   	// 

	/**********************************************************************
	* 功能描述:	读取TEC的I值
	* 输出参数	zValueI				I值数组
	* 输出参数	nTotalNum			I值数组大小
	***********************************************************************/
	bool TecReadValueI(int* zValueI, int& nTotalNum);				   	// 


	/**********************************************************************
	* 功能描述:	写入TEC的D值
	* 输入参数	zValueD				D值数组
	* 输入参数	nTotalNum			D值数组大小
	***********************************************************************/
	bool TecWriteValueD(int* zValueD, int nTotalNum);				   	// 

	/**********************************************************************
	* 功能描述:	读取TEC的D值
	* 输出参数	zValueD				D值数组
	* 输出参数	nTotalNum			D值数组大小
	***********************************************************************/
	bool TecReadValueD(int* zValueD, int& nTotalNum);				   	// 

	/**********************************************************************
	* 功能描述:	写入自整定开关和通道
	* 输入参数	zValue				写入自整定值 第一位自整定开关，第二位自整定TEC通道编号
	* 输入参数	nValueNum			数据数组大小
	***********************************************************************/
	bool TecWriteCustomSwitchAndChannal(int* zValue, int nValueNum);	// 

	/**********************************************************************
	* 功能描述:	读取自整定开关和通道
	* 输出参数	zValue				写入自整定值 第一位自整定开关，第二位自整定TEC通道编号
	* 输出参数	nValueNum			数据数组大小
	***********************************************************************/
	bool TecReadCustomSwitchAndChannal(int* zValue, int& nValueNum);	// 

	bool TecWriteTecTempMaxLimit(int* zTempValue, int nTotalNum);			// 写入TEC温度上限
	bool TecReadTecTempMaxLimit(int* zTempValue, int& nTotalNum);			// 读取TEC温度上限
	bool TecWriteTecTempMinLimit(int* zTempValue, int nTotalNum);				// 写入TEC温度下限
	bool TecReadTecTempMinLimit(int* zTempValue, int& nTotalNum);				// 读取TEC温度下限
	bool TecWriteColdBlockTempMaxLimit(int* zTempValue, int nTotalNum);		// 写入冷水块温度上限 这里的nTotalNum不是tec设备数目应该为1
	bool TecReadColdBlockTempMaxLimit(int* zTempValue, int& nTotalNum);		// 读取冷水块温度上限 
	bool TecWriteColdBlockTempMinLimit(int* zTempValue, int nTotalNum);		// 写入冷水块温度下限 这里的nTotalNum不是tec设备数目应该为1	
	bool TecReadColdBlockTempMinLimit(int* zTempValue, int& nTotalNum);		// 读取冷水块温度下限
	bool TecWriteHumidityAlarmSwitch(int nSwitch);							// 写入湿度报警屏蔽 0表示不检测湿度，1表示检测湿度,  默认检测
	bool TecReadHumidityAlarmSwitch(int& nSwitch);							// 读取湿度报警屏蔽
	bool TecWriteSelfCheckMode(int nSelfCheck);								// 进入自检模式
	bool TecReadSelfCheckMode(int& nSelfCheck);								// 查询自检模式
	bool TecWriteHumidityAlarm(int* zTempValue, int nTotalNum);			    // 写入湿度超高报警值
	bool TecReadHumidityAlarm(int* zTempValue, int& nTotalNum);				// 读取湿度超高报警值
	bool TecWriteDmdColdBlockDiffPositive(int* zDiffValue, int nTotalNum);			// 写入DMD和冷水块温差最大正值
	bool TecReadDmdColdBlockDiffPositive(int* zDiffValue, int& nTotalNum);			// 读取DMD和冷水块温差最大正值
	bool TecWriteDmdColdBlockDiffNegative(int* zNegativeValue, int nTotalNum);		// 写入DMD和冷水块温差最大负值
	bool TecReadDmdColdBlockDiffNegative(int* zNegativeValue, int& nTotalNum);		// 读取DMD和冷水块温差最大负值
	bool TecWriteTecAlarmThresholdElectric(int* zElectricValue, int nTotalNum);		// 写入TEC报警阈值电流
	bool TecReadTecAlarmThresholdElectric(int* zElectricValue, int& nTotalNum);		// 读取TEC报警阈值电流
	bool TecWriteTecAlarmThresholdTime(int* zTimeValue, int nTotalNum);				// 写入TEC阈值报警时间
	bool TecReadTecAlarmThresholdTime(int* zTimeValue, int& nTotalNum);			// 读取TEC阈值报警时间

	bool TecWriteTecAdvenceEnableTemp(int nEnableValue, int nTotalNum);			// 设置TEC提前控温使能
	bool TecReadTecAdvenceEnableTemp(int& nEnableValue, int& nTotalNum);		// 读取TEC提前控温使能
	bool TecWriteTecLaserSwitch(int nSwitchValue, int nTotalNum);				// 写入激光器开关
	bool TecReadTecLaserSwitch(int& nSwitchValue, int& nTotalNum);				// 读取激光器开关

	// LED指令集
	/**********************************************************************
	* 功能描述:	写入LED通道开关
	* 输入参数	zCh					通道开关按位排列
	* 输入参数	nTotalNum			通道数目
	***********************************************************************/
	bool LedWriteChannalSwitch(int zCh, int nTotalNum);		   	// 

	/**********************************************************************
	* 功能描述:	读取LED通道开关
	* 输出参数	zLedValue			通道开关按位排列
	* 输出参数	nTotalNum			通道数目
	***********************************************************************/
	bool LedReadChannalSwitch(int& zLedValue, int& nTotalNum);			// 

	/**********************************************************************
	* 功能描述:	写入LED通道电流设定值
	* 输入参数	zChannalValue		通道电流值
	* 输入参数	nTotalNum			通道数目
	***********************************************************************/
	bool LedWriteChannalElectric(int* zChannalValue, int nTotalNum);		   	// 


	/**********************************************************************
	* 功能描述:	读取LED通道电流设定值
	* 输出参数	zChannalValue		通道电流值
	* 输出参数	nTotalNum			通道数目
	***********************************************************************/
	bool LedReadChannalElectric(int* zChannalValue, int& nTotalNum);		   	

	/**********************************************************************
	* 功能描述:	设置LED温控数量
	* 输入参数	nControlNum		温控数量
	***********************************************************************/
	bool LedWriteTempControlNum(int nControlNum);		   	// 

	/**********************************************************************
	* 功能描述:	读取LED温控数量
	* 输出参数	nControlNum		温控数量
	***********************************************************************/
	bool LedReadTempControlNum(int& nControlNum);		   	// 

	/**********************************************************************
	* 功能描述:	写入LED温度上限
	
	* 输入参数	zTempMaxLimit		温控上限值
	* 输入参数	nTotalNum		温控上限值数目，必须三个
	***********************************************************************/
	bool LedWriteTempControlMaxLimit(int* zTempMaxLimit, int nTotalNum);   	// 

	/**********************************************************************
	* 功能描述:	读取LED温控上限
	* 输入参数	zTempMaxLimit		温控上限值
	* 输入参数	nTotalNum			温控上限值数目
	***********************************************************************/
	bool LedReadTempControlMaxLimit(int* zTempMaxLimit, int& nTotalNum);	   	// 

	/**********************************************************************
	* 功能描述:	读取LED温控下限
	* 输出参数	zTempMinLimit		温控下限值
	* 输出参数	nTotalNum			温控下限值数目
	***********************************************************************/
	bool LedReadTempControlMinLimit(int* zTempMinLimit, int& nTotalNum);	   	// 

	/**********************************************************************
	* 功能描述:	写入LED温度下限
	* 输入参数	zTempMinLimit		温控下限值
	* 输入参数	nTotalNum			温控值数目，必须三个
	***********************************************************************/
	bool LedWriteTempControlMinLimit(int* zTempMinLimit, int nTotalNum);   	// 

	/**********************************************************************
	* 功能描述:	开关所有电源LED通道
	* 输入参数	nTempMinLimit		温控下限值
	***********************************************************************/
	bool LedWriteOpenCloseAllPowerControl(int nPowerCount);	   	// 开关所有LED通道


protected:
	void* m_FunProTecLed;

};

























#endif // !_PRO_TEC_LED_H_

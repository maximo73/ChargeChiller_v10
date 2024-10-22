/*
	// -------------------------------------------------------------------------------------------
	// EEPROM Initilize 
	// -------------------------------------------------------------------------------------------

	Addr		Type			Value				Commnet
	----------------------------------------------------------------------------------------------
	33		char			Flag_EEPROM Write already

	50		char 		Pump Control Mode

	100~101	int			SystempRunDelay		시스템 운전 지연 
	102~103	int			SystemStopDelay		시스템 정지 지연

	104~105	int			StartAlarmDelay		시작 알람 지연
	106~107	int			RunAlarmDelay			운전 알람 지연 

	108~109	int			OilWarningTemp			윤활유 경고 온도
	110~111	int			OilAlarmTemp			윤활유 알람 온도

	112~113	int			HeatRunTemp			히터 운전 온도
	114~115	int			HeatStopTemp			히터 정지 온도

	116~117	int			FanRunTemp			팬 운전 온도
	118~119	int			FanStopTemp			팬 정지 온도

	120~121	int			OffsetTP				온도 오프셋

	122~123	int			LowPR				이상저압
	124~125	int			HighPR				이상고압
	126~127	int			SetPR				설정압력
	128~129	int			OffsetPR				압력 오프셋
	
	130		int			SensorMode			압력센서 선택

	132		int			ComAddr				통신주소
	134		int			IndBaud				통신속도 인덱스

*/

__flash unsigned char A_Font[128] = {
  // 0x00-------------------------------0x0F
  0xBF, /* 0. <-- 0x00 : NUL */
  0x86, /* 1. <-- 0x01 : SOH */
  0xDB, /* 2. <-- 0x02 : STX */
  0xCF, /* 3. <-- 0x03 : ETX */
  0xE6, /* 4. <-- 0x04 : EOT */
  0xED, /* 5. <-- 0x05 : ENQ */
  0xFD, /* 6. <-- 0x06 : ACK */
  0xA7, /* 7. <-- 0x07 : BEL */
  0xFF, /* 8. <-- 0x08 : BS  */
  0xE7, /* 9. <-- 0x09 : HT  */
  0x00, /* 0x0A : LF  */
  0x00, /* 0x0B : VT  */
  0x00, /* 0x0C : FF */
  0x00, /* 0x0D : CR */
  0x00, /* 0x0E : SO */
  0x00, /* 0x0F : SI */
  // 0x10-------------------------------0x1F
  0x00, /* 0x10 : DLE */
  0x00, /* 0x11 : DC1 */
  0x00, /* 0x12 : DC2 */
  0x00, /* 0x13 : DC3 */
  0x00, /* 0x14 : DC4 */
  0x00, /* 0x15 : NAK */
  0x00, /* 0x16 : SYN */
  0x00, /* 0x17 : ETB */
  0x00, /* 0x18 : CAN */
  0x00, /* 0x19 : EM  */
  0x00, /* 0x1A : SUB */
  0x00, /* 0x1B : ESC */
  0x00, /* 0x1C : FS  */
  0x00, /* 0x1D : GS  */
  0x00, /* 0x1E : RS  */
  0x00, /* 0x1F : US */
  // 0x20-------------------------------0x2F
  0x00, /* space */
  0x00, /* ! */
  0x00, /* " */
  0x00, /* # */
  0x00, /* $ */
  0x00, /* % */
  0x00, /* & */
  0x00, /* ' */
  0x00, /* ( */
  0x00, /* ) */
  0x00, /* * */
  0x00, /* + */
  0x00, /* , */
  0x40, /* - */
  0x00, /* . */
  0x00, /* / */
  // 0x30-------------------------------0x3F
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x27, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x00, /* : */
  0x00, /* ; */
  0x00, /* < */
  0x00, /* = */
  0x00, /* > */
  0x00, /* ? */
  // 0x40-------------------------------0x4F
  0x00, /* @ */
  0x5F, /* A */
  0x7C, /* B */
  0x58, /* C */
  0x5E, /* D */
  0x79, /* E */
  0x71, /* F */
  0x6B, /* G */
  0x74, /* H */
  0x30, /* I */
  0x0E, /* J */
  0x72, /* K */
  0x38, /* L */
  0x15, /* M */
  0x54, /* N */
  0x5C, /* O */
  // 0x50-------------------------------0x5F
  0x73, /* P */
  0x67, /* Q */
  0x50, /* R */
  0x2D, /* S */
  0x78, /* T */
  0x1C, /* U */
  0x62, /* V */
  0x2A, /* W */
  0x76, /* X */
  0x6E, /* Y */
  0x49, /* Z */
  0x00, /* [ */
  0x00, /* \ */
  0x00, /* ] */
  0x00, /* ^ */
  0x00, /* _ */
  // 0x60-------------------------------0x6F
  0x00, /* ` */
  0x5F, /* a */
  0x7C, /* b */
  0x58, /* c */
  0x5E, /* d */
  0x79, /* e */
  0x71, /* f */
  0x6B, /* g */
  0x74, /* h */
  0x30, /* i */
  0x0E, /* j */
  0x72, /* k */
  0x38, /* l */
  0x15, /* m */
  0x54, /* n */
  0x5C, /* o */
  // 0x70-------------------------------0x7F
  0x73, /* p */
  0x67, /* q */
  0x50, /* r */
  0x2D, /* s */
  0x78, /* t */
  0x1C, /* u */
  0x62, /* v */
  0x2A, /* w */
  0x76, /* x */
  0x6E, /* y */
  0x49, /* z */
  0x00, /* { */
  0x00, /* | */
  0x00, /* } */
  0x00, /* ~ */
  0x00};/* 0x7F : DEL */

//########### Setup Variable #############
unsigned char	PumpMode;				// Pump Control Mode

		int  SystemRunDelay;		// System Run Delay
		int	SystemStopDelay;		// System Stop Delay
		
		int	StartAlarmDelay;		// Delay for Warning & Alarm after Power ON
		int	RunAlarmDelay;			// Delay for Warning & Alarm in Running
		
		int	OilWarningTemp;		// Oil temperature for warning
		int	OilAlarmTemp;			// Oil temperature for alarm
		
//		int	HeatRunTemp;			// Heater Run Temperature
//		int	HeatStopTemp;			// Heater Stop Temperature
		
		int	FanRunTemp;			// Fan Run Temperature
		int	FanStopTemp;			// Fan Stop Temperature
          
		int	OffsetTP;				// Temperature Offset
		
		int	LowPR;				// Lowest Pressure
		int	HighPR;				// Highest pressure
		int	SetPR;				// Setpoint Pressure
		int	OffsetPR;				// Pressure Offset
		
		int	SensorMode;			// Pressure Sensor
		
		int	ComAddr;				// Communication Address
		int	IndBaud;				// Communication Baudrate
		
//########### Global Variable #############
unsigned  char Ctrl; 			// Control Command
// 			Ctrl.0: Stop(0)/Run(1)
		
unsigned  char NowERROR, PreERROR;	// NowERROR: Current Error, PreERROR: Error before Display
//			NowERROR.0: Oil Temp. Warning
//			NowERROR.1: Oil Temp. Alarm
//			NowERROR.2: Low Pressure 
//			NowERROR.3: High Pressure
//			NowERROR.4: Level Warning(Middle)
//			NowERROR.5: Level Alarm(Low)

		int  CNT_RunDelay, CNT_StopDelay;		// System Run/Stop Signal Delay
		char CNT_CheckAlarm, FLG_CheckAlarm;	// Check Delay for Alarm after Power ON
		char CNT_WarningTP, CNT_AlarmTP;		// Temperature Warning & Alarm Count
		char CNT_LowPR, CNT_HighPR;			// Pressure Lowest & Highest Count
		char CNT_WarningLV, CNT_AlarmLV;		// Level Warning & Alarm Count
		char CNT_PumpFault;					// Pump Fault Check Count
		
		// PID Control
		float KP, KI, KD;
		float u0, u1, e0, e1, e2; 		// PID Controller를 위한 변수
         
		int PWM_OUT;   				// 인버터 출력을 위한 D/A 
		
		int MaxSpeed, MinSpeed;
//######### Variables related to ADC ###########
		int	 AD_index0, AD_index1;		// ADC 10bit 압력값을 저장하기 위한 배열 인텍스		
		int   AD_temp0[10], AD_temp1[10];	// ADC 10bit 압력값을 10개까지 저장하기 위한 배열		
		int   AD_result0, AD_result1;		// 계산된 값을 저장하기 위한 배열
		
		int	NowTP;					// Measured Temperature
		int	NowPR;					// Measured Pressure

//######### Display Variable ###########
unsigned  char CHAR_FND[4];		// Display FND Character
		char CNT_FND;			// Dispaly FND Count
		char CNT_PrErr;		// Pressure Error check delay
		
//#########  Variable for Key Input ###########
unsigned char bPASS;  
unsigned char bQuit;
unsigned char KEY;
unsigned char Two_Key;			// 현재 루프에서 상위루프로 탈출할때 키값을 가지고 가기위해 사용하는 플래그변수 

	    char Flag_Disp;			// 고장이 있을 경우, 고장정보와 상태정보를 전환하여 보고자 할 경우.
	    
unsigned char KeyScan;
unsigned char PreKeyScan;
	    int  SameKeyRepeat;
	    int  SameModeKey;
	    int  NotUpDownKey;
	    char Start_KeyScan;
	    char Finish_KeyScan;

//########### Variable for Control Function #############	 
		int	Cnt_Task0, Cnt_Task1;		// 시간처리를 위한 타스크 카운터 		
unsigned 	char TimeSchFlag;				// 기본 타스크주기를 위한 플래그 레지스터   

		char FLG_BUZZ;					// Turn-on Flag for Buzzer
		char CNT_BUZZER;				// Count for Turn-off Buzzer
		
		char Flag_Pump;				// Pump Status
//########### Variable for Display Function #############	
		int  Error_index;				// index for error display

////#########  Variable for Display Communication ###########
unsigned int InReg[5];		// Address 0x0000(0) ~ 0x0005(5)
unsigned int HoReg[17];		// Address 0x000A(10)~ 0x001A(26)
unsigned char TX_BUFF[20], RX_BUFF[20];
	    char Flag_Query;
	    char Count_Query;
	    char Count_NumByte;
	    char Flag_EndQuery;
	    char NumByte;
	    char Flag_ModBusTX;
	    int  Index_ModBusTX;
	    
//########### ADC Function #############	    
void TASK_temperature(void)  					// Temperature Processing
{
	int i, sum = 0, max = 0, min = 1023;
	float fdata = 0.;  //temp = 0.,
	
	AD_temp0[AD_index0++] = ad_conversion(0) & 0x03FF;
     if(AD_index0 > 9) AD_index0 = 0;    
     
     for(i=0;i<10;i++) {
        if(AD_temp0[i] > max) max = AD_temp0[i];  	// 최대값을 구한다 
        if(AD_temp0[i] < min) min = AD_temp0[i];    	// 최소값을 구한다 
        sum += AD_temp0[i];
     }
	
     sum -= max;
     sum -= min;
     AD_result0 = sum/8;  
//	temp = (float)AD_result0;

	// Temperature -50(3389 kohm, 15) ~ 99(3.391kohm, 958)
	if(AD_result0 > 939)      fdata = 0.4813 * AD_result0 - 362.0443;
	else if(AD_result0 > 911) fdata = 0.3582 * AD_result0 - 246.4384;
	else if(AD_result0 > 873) fdata = 0.1966 * AD_result0 - 101.6736;
	else if(AD_result0 > 823) fdata = 0.1966 * AD_result0 - 101.6736;
	else if(AD_result0 > 756) fdata = 0.1498 * AD_result0 - 63.2482;
	else if(AD_result0 > 671) fdata = 0.1185 * AD_result0 - 39.5524;	
	else if(AD_result0 > 570) fdata = 0.0988 * AD_result0 - 26.3603;
	else if(AD_result0 > 515) fdata = 0.0904 * AD_result0 - 21.5281;
	else if(AD_result0 > 458) fdata = 0.0876 * AD_result0 - 20.1052;
	else if(AD_result0 > 344) fdata = 0.0881 * AD_result0 - 20.3154;	
	else if(AD_result0 > 241) fdata = 0.0970 * AD_result0 - 23.3806;
	else if(AD_result0 > 157) fdata = 0.1190 * AD_result0 - 28.7014;
	else if(AD_result0 > 96)  fdata = 0.1626 * AD_result0 - 35.5455;
	else if(AD_result0 > 55)  fdata = 0.2435 * AD_result0 - 43.2811;
	else if(AD_result0 > 29)  fdata = 0.3962 * AD_result0 - 51.6117;
	else if(AD_result0 > 15)  fdata = 0.6915 * AD_result0 - 60.2646;
	else                      fdata = -50;  
	
//	
//	if( temp <= 15.){					// below -50 15,
//		fdata = -50.;
//	}	
//	if(15. < temp <= 29.){			// -49'C ~ -40'C
//		fdata = 0.6915 * temp - 60.2646;
//	}
//	if(29. < temp <= 55.){			// -39'C ~ -30'C
//		fdata = 0.3962 * AD_result0 - 51.6117;
//	}
//	if(55. < temp <= 96.){			// -29'C ~ -20'C
//		fdata = 0.2435 * temp - 43.2811;
//	}
//	if(96. < temp <= 157.){			// -19'C ~ -10'C
//		fdata = 0.1626 * temp - 35.5455;
//	}
//	if(157. < temp <= 241.){		// -9'C ~ 0'C
//		fdata = 0.1190 * temp - 28.7014;
//	}
//	if(241. < temp <= 344.){		// 1'C ~ 10'C
//		fdata = 0.0970 * temp - 23.3806;
//	}
//	if(344. < temp <= 458.){		// 11'C ~ 20'C
//		fdata = 0.0881 * temp - 20.3154;
//	}
//	if(458. < temp <= 515.){		// 21'C ~ 25'C
//		fdata = 0.0876 * AD_result0 - 20.1052;
//	}
//	if(515. < temp <= 570.){		// 25'C ~ 30'C
//		fdata = 0.0904 * temp - 21.5281;
//	}
//	if(570. < temp <= 671.){		// 31'C ~ 40'C
//		fdata = 0.0988 * temp - 26.3603;
//	}
//	if(671. < temp <= 756.){		// 41'C ~ 50'C
//		fdata = 0.1185 * temp - 39.5524;
//	}
//	if(756. < temp <= 823.){		// 51'C ~ 60'C
//		fdata = 0.1498 * temp - 63.2482;
//	}
//	if(823. < temp <= 873.){		// 61'C ~ 70'C
//		fdata = 0.1966 * temp - 101.6736;	
//	}
//	if(873. < temp <= 911.){		// 71'C ~ 80'C
//		fdata = 0.2641 * temp - 160.7125;		
//	}
//	if(911. < temp <= 939.){		// 81'C ~ 90'C
//		fdata = 0.3582 * temp - 246.4384;		
//	}
//	if(939. < temp <= 958.){		// 91'C ~ 99'C
//		fdata = 0.4813 * temp - 362.0443;			
//	}
//	if(temp > 958.){
//		fdata = 99.;
//	}
	
	fdata += ((float)OffsetTP - 5.);

	// fdata  = (1.1667 * (float)AD_result[ch]) - (335.3372 - (float)OffsetTP);
	// fdata  = 0.0000003*temp*temp*temp - 0.0004*temp*temp + 0.2766*temp - (47.404 - ((float)OffsetTP-5.));
	if(fdata < -50.) fdata = -50.;
	if(fdata >  99.) fdata =  99.;

	NowTP = (int)fdata;
	InReg[4] = (unsigned int)NowTP;
} 
	    
void TASK_pressure(void)  					// Pressure Processing
{
	int i, sum = 0, max = 0, min = 1023;
	float temp = 0., fdata = 0.;
	
	AD_temp1[AD_index1++] = ad_conversion(1) & 0x03FF;
     if(AD_index1 > 9) AD_index1 = 0;    
     
     for(i=0;i<10;i++) {
        if(AD_temp1[i] > max) max = AD_temp1[i];  	// 최대값을 구한다 
        if(AD_temp1[i] < min) min = AD_temp1[i];    	// 최소값을 구한다 
        sum += AD_temp1[i];
     }
	
     sum -= max;
     sum -= min;
     AD_result1 = sum/8;  
	temp = (float)AD_result1;
	
	// Pressure 0.0 ~ 10.0 bar
	if(SensorMode == 16){
		fdata  = (0.0196 * temp) - (4.0098 - ((float)OffsetPR - 5.)/10.);
		if(fdata < 0.) fdata = 0.;
		if(fdata > 16.) fdata = 16.;
	}else {		
		fdata  = (0.0122 * temp) - (2.5061 - ((float)OffsetPR - 5.)/10.);
		if(fdata < 0.) fdata = 0.;
		if(fdata > 10.) fdata = 10.;
	}
	
	NowPR = (int)(fdata * 10.);
	InReg[3] = (unsigned int)NowPR;
} 
//########### EEPROM Initialize #############
void EEPROM_Init(void)
{	
     // int type variable initializ	
	PumpMode = 0x01;				// Pump Control Mode = 0 (On/Off Control)
	EE_PUT(50, PumpMode);
	
     SystemRunDelay = 1;			// System Run Delay = 1 sec
     EE_PUT(100,(unsigned char)SystemRunDelay);				  
     EE_PUT(101,(unsigned char)(SystemRunDelay>>8));
	
	SystemStopDelay = 60;		// System Stop Delay = 60 sec
     EE_PUT(102,(unsigned char)SystemStopDelay);				  
     EE_PUT(103,(unsigned char)(SystemStopDelay>>8));	
		
	StartAlarmDelay = 10;		// Delay for Warning & Alarm after Power ON = 10 sec
     EE_PUT(104,(unsigned char)StartAlarmDelay);				  
     EE_PUT(105,(unsigned char)(StartAlarmDelay>>8));
	
	RunAlarmDelay = 2;			// Delay for Warning & Alarm in Running = 2 sec
     EE_PUT(106,(unsigned char)RunAlarmDelay);				  
     EE_PUT(107,(unsigned char)(RunAlarmDelay>>8));	
		
	OilWarningTemp = 85;		// Oil temperature for warning = 85 'C
     EE_PUT(108,(unsigned char)OilWarningTemp);				  
     EE_PUT(109,(unsigned char)(OilWarningTemp>>8));
	
	OilAlarmTemp = 95;			// Oil temperature for alarm = 95 'C
     EE_PUT(110,(unsigned char)OilAlarmTemp);				  
     EE_PUT(111,(unsigned char)(OilAlarmTemp>>8));	
		
//	HeatRunTemp = 0;			// Heater Run Temperature = 0 'C
//     EE_PUT(112,(unsigned char)HeatRunTemp);				  
//     EE_PUT(113,(unsigned char)(HeatRunTemp>>8));	
//	
//	HeatStopTemp = 10;			// Heater Stop Temperature = 10 'C
//     EE_PUT(114,(unsigned char)HeatStopTemp);				  
//     EE_PUT(115,(unsigned char)(HeatStopTemp>>8));	
//		
	FanRunTemp = 20;			// Fan Run Temperature = 20 'C
     EE_PUT(116,(unsigned char)FanRunTemp);				  
     EE_PUT(117,(unsigned char)(FanRunTemp>>8));
	
	FanStopTemp = 15;			// Fan Stop Temperature = 15 'C
     EE_PUT(118,(unsigned char)FanStopTemp);				  
     EE_PUT(119,(unsigned char)(FanStopTemp>>8));	
	
	OffsetTP = 5;				// Temperature Offset = 0'C
     EE_PUT(120,(unsigned char)OffsetTP);	
		
	LowPR = 10;				// Lowest Pressure = 1.0 bar
     EE_PUT(122,(unsigned char)LowPR);				  
     EE_PUT(123,(unsigned char)(LowPR>>8));
	
	HighPR = 80;				// Highest pressure = 8.0 bar
     EE_PUT(124,(unsigned char)HighPR);				  
     EE_PUT(125,(unsigned char)(HighPR>>8));
	
	SetPR = 50;				// Setpoint Pressure = 5.0 bar
     EE_PUT(126,(unsigned char)SetPR);				  
     EE_PUT(127,(unsigned char)(SetPR>>8));
	
	OffsetPR = 5;				// Pressure Offset = 0.0 bar
     EE_PUT(128,(unsigned char)OffsetPR);	
	
	SensorMode = 16;			// SensorMode = 16bar
	EE_PUT(130,(unsigned char)SensorMode);
	
	ComAddr = 1;				// Communication address = 1
	EE_PUT(132,(unsigned char)ComAddr); 
	
	IndBaud = 3;				// Communication baudrate = 3[19.2kbps]
	EE_PUT(134,(unsigned char)IndBaud); 
	
}

//########### Variable Initialize #############
void VARIABLE_Init(void)
{
	int i;
	
	for(i=0;i<5;i++) InReg[i] = 0;
	for(i=0;i<17;i++) HoReg[i] = 0;
	
	SystemRunDelay 	= ((unsigned int)EE_GET(101)<<8) + (unsigned int)EE_GET(100);		// System Run Delay = 1 sec	
	HoReg[0]			= (unsigned int)SystemRunDelay;
	SystemStopDelay 	= ((unsigned int)EE_GET(103)<<8) + (unsigned int)EE_GET(102);		// System Stop Delay = 1 sec
	HoReg[1]			= (unsigned int)SystemStopDelay;
	StartAlarmDelay 	= ((unsigned int)EE_GET(105)<<8) + (unsigned int)EE_GET(104);		// Delay for Warning & Alarm after Power ON = 10 sec
	HoReg[2]			= (unsigned int)StartAlarmDelay;
	RunAlarmDelay 		= ((unsigned int)EE_GET(107)<<8) + (unsigned int)EE_GET(106);		// Delay for Warning & Alarm in Running = 2 sec
	HoReg[3]			= (unsigned int)RunAlarmDelay;	
	OilWarningTemp 	= ((unsigned int)EE_GET(109)<<8) + (unsigned int)EE_GET(108);		// Oil temperature for warning = 85 'C
	HoReg[4]			= (unsigned int)OilWarningTemp;
	
	OilAlarmTemp 		= ((unsigned int)EE_GET(111)<<8) + (unsigned int)EE_GET(110);		// Oil temperature for alarm = 95 'C
	HoReg[5]			= (unsigned int)OilAlarmTemp;
//	HeatRunTemp 		= ((unsigned int)EE_GET(113)<<8) + (unsigned int)EE_GET(112);		// Heater Run Temperature = 0 'C
//	HeatStopTemp 		= ((unsigned int)EE_GET(115)<<8) + (unsigned int)EE_GET(114);		// Heater Stop Temperature = 10 'C
	FanRunTemp 		= ((unsigned int)EE_GET(117)<<8) + (unsigned int)EE_GET(116);		// Fan Run Temperature = 20 'C
	HoReg[6]			= (unsigned int)FanRunTemp;
	FanStopTemp 		= ((unsigned int)EE_GET(119)<<8) + (unsigned int)EE_GET(118);		// Fan Stop Temperature = 15 'C
	HoReg[7]			= (unsigned int)FanStopTemp;
	OffsetTP	 		= (unsigned int)EE_GET(120);									// Temperature Offset = 0 'C 
	HoReg[8]			= (unsigned int)OffsetTP;
	LowPR 			= ((unsigned int)EE_GET(123)<<8) + (unsigned int)EE_GET(122);		// Lowest Pressure = 3.0 bar
	HoReg[9]			= (unsigned int)LowPR;
	HighPR 			= ((unsigned int)EE_GET(125)<<8) + (unsigned int)EE_GET(124);		// Highest pressure = 7.0 bar
	HoReg[10]			= (unsigned int)HighPR;
	SetPR 			= ((unsigned int)EE_GET(127)<<8) + (unsigned int)EE_GET(126);		// Setpoint Pressure = 5.0 bar
	HoReg[11]			= (unsigned int)SetPR;
	
	OffsetPR 			= (unsigned int)EE_GET(128);									// Pressure Offset = 0.0 bar	
	HoReg[12]			= (unsigned int)OffsetPR;
	PumpMode			= EE_GET(50);												// Pump Control Mode	
	HoReg[13]			= (unsigned int)PumpMode;
	SensorMode 		= (unsigned int)EE_GET(130);									// SensorMode = 16bar
	HoReg[14]			= (unsigned int)SensorMode;
	ComAddr			= (unsigned int)EE_GET(132);									// Communication address = 1	
	HoReg[15]			= (unsigned int)ComAddr;
	IndBaud			= (unsigned int)EE_GET(134);									// Communication baudrate = 3[19.2kbps]
	HoReg[16]			= (unsigned int)IndBaud;
	UBRR1L 			= Baudrate[IndBaud];	// 0:4800bps, 1:9600bps, 2:14,4kbps, 3:19.2kbps, 4:28.8kbps
										// 5:38.4kbps, 6:57.6kbps, 7:76.8kbps, 8:115.2kbps	
		
	//######### Variables related to Remote Control Signal ###########	
	Ctrl = 0x00;
	
	//######### Variables related to ADC ###########	
	for(i=0;i<10;i++) {		// ADC 10bit 압력값을 10개까지 저장하기 위한 배열
		AD_temp0[i] = 0;
		AD_temp1[i] = 0;
	}
	AD_index0 = 0;			// ADC 10bit 압력값을 저장하기 위한 배열 인텍스	
	AD_index1 = 0;
	AD_result0 = 0;		// 계산된 값을 저장하기 위한 배열
	AD_result1 = 0;
			
	NowTP = 0;			// Measured Temperature			(-20~99 )
	NowPR = 0;			// Measured Pressure			(0~100 	--> 0.0~10.0)
	
	NowERROR = 0x00;		// Detected Error
	PreERROR = 0x00;		// for Error Display
	
	CNT_RunDelay = 0;		// System Run Signal Input Delay
	CNT_StopDelay = 0;		// System Stop Signal Input Delay
	CNT_CheckAlarm = 0;		// Alarm Check Delay
	FLG_CheckAlarm = CLR;	// Alarm Check Flag
	CNT_WarningTP = 0;		// Temperature Warning Count
	CNT_AlarmTP = 0;		// Temperature Alarm Count
	CNT_LowPR = 0;			// Pressure Lowest Count
	CNT_HighPR = 0;		// Pressure Highest Count
	CNT_WarningLV = 0;		// Level Warning Count
	CNT_AlarmLV = 0;		// Level Warning Count
	CNT_PumpFault = 0;		// Pump Fault Check Count 
	
	CNT_PrErr = 0;			// Pressure Error check delay
		
	for(i=0;i<4;i++) CHAR_FND[i] = 0;		// Display FND Character
	CNT_FND = 0;							// Dispaly FND Count
			
	KP = 1.5, KI = 10., KD = 1.;				// PID float 형 계수 
     u0 = 0., u1 = 0., e0 = 0., e1 = 0., e2 = 0.; // PID Controller를 위한 변수
         
	PWM_OUT = 0;   				// 인버터 출력을 위한 D/A 
	//#########  Variable for Key Input ###########
	bPASS = CLR;  
     bQuit = CLR;
     KEY   = 0x00;    
     Two_Key = 0x00;    // 현재 루프에서 상위루프로 탈출할때 키값을 가지고 가기위해 사용하는 플래그변수 

	Flag_Disp = 0;
	
	KeyScan = 0x00;
	PreKeyScan = 0x00;
	SameKeyRepeat = 0;
	SameModeKey = 0;
	NotUpDownKey = 0;
	Start_KeyScan = CLR;
	Finish_KeyScan = CLR;	
	
	//########### Variable for Control Function #############	 
	Cnt_Task0 = 0;
	Cnt_Task1 = 0;					// 시간처리를 위한 타스크 카운터 		
	TimeSchFlag = 0x00;				// 기본 타스크주기를 위한 플래그 레지스터   
	
	FLG_BUZZ = CLR;
	CNT_BUZZER = 0;
	
	Flag_Pump = CLR;
	//########### Variable for Control Function #############	 
	Error_index = 0;
	
	Flag_Query = CLR;
	Count_Query = 0;
	Count_NumByte = 0;
	Flag_EndQuery = CLR;
	NumByte = 0;
	Flag_ModBusTX = CLR;
	Index_ModBusTX = 0;
	for(i=0;i<20;i++){
		TX_BUFF[i] = 0x00;
		RX_BUFF[i] = 0x00;
	}
}

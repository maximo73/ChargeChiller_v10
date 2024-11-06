void FND_2d1b(int number)		// Pressure Display
{
     unsigned int i;      
     
	i = number / 100;	
	if(i == 0) CHAR_FND[0] = ' ';						//   0.0
	else		 CHAR_FND[0] = (unsigned char)(i + '0');	//  00.0
		
	number = number % 100;
	i = number / 10;      
	CHAR_FND[1] = (unsigned char)i;					// *.
	
	i = number % 10;
	CHAR_FND[2] = (unsigned char)(i + '0');
	
	CHAR_FND[3] = 'b';		// bar 표시
} 

void FND_2b(int number)		// Pressure Display
{
     unsigned int i;      
     
	CHAR_FND[0] = ' ';						// Nul
		
	i = number / 10;      
	CHAR_FND[1] = (unsigned char)(i + '0');
	
	i = number % 10;
	CHAR_FND[2] = (unsigned char)(i + '0');
	
	CHAR_FND[3] = 'b';		// bar 표시
} 
	
void FND_s1d1b(int number)		// Pressure Offset Display
{
	if(number < 5) {
		CHAR_FND[0] = '-';
		CHAR_FND[1] = 0;
		CHAR_FND[2] = (unsigned char)((5-number) + '0');
     }else {
		CHAR_FND[0] = ' ';
		CHAR_FND[1] = 0;
		CHAR_FND[2] = (unsigned char)((number-5) + '0');
	}
	CHAR_FND[3] = 'b';		// bar 표시
} 

void FND_s2c(int number)		// Temperature Display
{
     unsigned int i;    
     
     if(number < 0) {
     	number = number * (-1);
		CHAR_FND[0] = '-';
     }else {
		CHAR_FND[0] = ' ';
	}
	
	i = number / 10;	
	if(i == 0) CHAR_FND[1] = ' ';				//   0
	else CHAR_FND[1] = (unsigned char)(i + '0');	// 000
	
	i = number % 10;
	CHAR_FND[2] = (unsigned char)(i + '0');
	
	CHAR_FND[3] = 'c';		// 'C 표시
}

void FND_s1c(int number)		// Temperature Offset Display
{
	if(number < 5) {
		CHAR_FND[0] = '-';
		CHAR_FND[1] = ' ';
		CHAR_FND[2] = (unsigned char)((5-number) + '0');
     }else {
		CHAR_FND[0] = ' ';
		CHAR_FND[1] = ' ';
		CHAR_FND[2] = (unsigned char)((number-5) + '0');
	}
	
	CHAR_FND[3] = 'c';		// 'C 표시
}

void FND_2ds(int number)		// Time(Second) Display
{
     unsigned int i;      
     
	CHAR_FND[0] = ' ';
	
	i = number / 10;	
	if(i == 0) CHAR_FND[1] = ' ';				//   0
	else CHAR_FND[1] = (unsigned char)(i + '0');	// 000
		
	i = number % 10;
	CHAR_FND[2] = (unsigned char)(i + '0');
	
	CHAR_FND[3] = 's';		// sec 표시
}

void FND_1d(int number)		// Operation Mode Display
{
	CHAR_FND[0] = ' ';
	CHAR_FND[1] = ' ';		
	CHAR_FND[2] = ' ';		
	CHAR_FND[3] = (unsigned char)(number + '0');		// 값 표시
}

void FND_2d(int number)		// address Display
{
     unsigned int i;      
     
	CHAR_FND[0] = ' ';						//  
	CHAR_FND[1] = ' ';						//  
		
	i = number / 10;      
     if(i == 0) CHAR_FND[2] = ' ';				//   0
	else CHAR_FND[2] = (unsigned char)(i + '0');
	
	i = number % 10;
	CHAR_FND[3] = (unsigned char)(i + '0');
} 
     
// Status Display and Parameter Setting
unsigned int KEY_INPUT(void)
{
	register unsigned char key;
	
	// 이전화면의 상위메뉴로 돌아가라는 명령이 입력되었을 때
     if(Two_Key) {
     	Two_Key = CLR;
     	return KEY;
     }
	
	// 위의 두가지 항목이 아닌 Key 입력을 요청하였을 때
	if(!Start_KeyScan) {						// 100ms Task에서 Key를 스캔 하라는 플래그 발생
		Start_KeyScan = SET;
		return 0x00;
	}else if((Start_KeyScan) && (Finish_KeyScan)){	// 100ms Task에서 Key 스캔이 완료되었음을 알리는 프래그 발생
		key = KeyScan;
		KEY = key;
		KeyScan = 0x00;
		Start_KeyScan   = CLR;
		Finish_KeyScan  = CLR;
		return key;
	}else {		
		return 0x00;
	}
}

// *************************************************************************************************
//  설정    
// *************************************************************************************************
void SetModeModify00(void)		// System Run Delay
{
	int temp = 0;

     temp = SystemRunDelay;

     bQuit = CLR;
	
	FND_2ds(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			SystemRunDelay = temp;
			EE_PUT(100,(unsigned char)SystemRunDelay); 							
			EE_PUT(101,(unsigned char)(SystemRunDelay>>8)); 
			HoReg[0] = (unsigned int)SystemRunDelay;
			break;
		case Kup:
			temp++;
			if(temp > 10) temp = 10;
			break;
		case Kdown:			
			temp--;
			if(temp < 1) temp = 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2ds(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify01(void)		// System Stop Delay
{
	int temp = 0;

     temp = SystemStopDelay;

     bQuit = CLR;
	
	FND_2ds(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			SystemStopDelay = temp;
			EE_PUT(102,(unsigned char)SystemStopDelay); 							
			EE_PUT(103,(unsigned char)(SystemStopDelay>>8)); 
			HoReg[1] = (unsigned int)SystemStopDelay;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 120) temp = 120;
			break;
		case Kdown:			
			temp--;
			if(temp < 1) temp = 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2ds(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify02(void)		// Delay for Warning & Alarm after Power ON
{
	int temp = 0;

     temp = StartAlarmDelay;

     bQuit = CLR;
	
	FND_2ds(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			StartAlarmDelay = temp;
			EE_PUT(104,(unsigned char)StartAlarmDelay); 							
			EE_PUT(105,(unsigned char)(StartAlarmDelay>>8)); 
			HoReg[2] = (unsigned int)StartAlarmDelay;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 20) temp = 20;
			break;
		case Kdown:			
			temp--;
			if(temp < 1) temp = 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2ds(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify03(void)		// Delay for Warning & Alarm in Running
{
	int temp = 0;

     temp = RunAlarmDelay;

     bQuit = CLR;
	
	FND_2ds(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			RunAlarmDelay = temp;
			EE_PUT(106,(unsigned char)RunAlarmDelay); 							
			EE_PUT(107,(unsigned char)(RunAlarmDelay>>8)); 
			HoReg[3] = (unsigned int)RunAlarmDelay;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 10) temp = 10;
			break;
		case Kdown:			
			temp--;
			if(temp < 1) temp = 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2ds(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify04(void)		// Oil temperature for warning
{
	int temp = 0;

     temp = OilWarningTemp;

     bQuit = CLR;
	
	FND_s2c(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			OilWarningTemp = temp;
			EE_PUT(108,(unsigned char)OilWarningTemp); 							
			EE_PUT(109,(unsigned char)(OilWarningTemp>>8)); 
			HoReg[4] = (unsigned int)OilWarningTemp;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp >= OilAlarmTemp) temp = OilAlarmTemp - 1;
			break;
		case Kdown:			
			temp--;
			if(temp < 50) temp = 50;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s2c(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify05(void)		// Oil temperature for alarm
{
	int temp = 0;

     temp = OilAlarmTemp;

     bQuit = CLR;
	
	FND_s2c(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			OilAlarmTemp = temp;
			EE_PUT(110,(unsigned char)OilAlarmTemp);		 
     		EE_PUT(111,(unsigned char)(OilAlarmTemp>>8));
			HoReg[5] = (unsigned int)OilAlarmTemp;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 99) temp = 99;
			break;
		case Kdown:			
			temp--;
			if(temp <= OilWarningTemp) temp = OilWarningTemp + 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s2c(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify06(void)		// Fan Run Temperature 
{
	int temp = 0;

     temp = FanRunTemp;

     bQuit = CLR;
	
	FND_s2c(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			FanRunTemp = temp;
			EE_PUT(116,(unsigned char)FanRunTemp);		 
     		EE_PUT(117,(unsigned char)(FanRunTemp>>8));				
			HoReg[6] = (unsigned int)FanRunTemp;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 50) temp = 50;
			break;
		case Kdown:			
			temp--;
			if(temp <= FanStopTemp) temp = FanStopTemp + 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s2c(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify07(void)		// Fan Stop Temperature
{
	int temp = 0;

     temp = FanStopTemp;

     bQuit = CLR;
	
	FND_s2c(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			FanStopTemp = temp;
			EE_PUT(118,(unsigned char)FanStopTemp);		 
     		EE_PUT(119,(unsigned char)(FanStopTemp>>8));				
			HoReg[7] = (unsigned int)FanStopTemp;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp >= FanRunTemp) temp = FanRunTemp - 1;
			break;
		case Kdown:			
			temp--;
			if(temp <= 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s2c(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify08(void)		// Temperature Offset
{
	int temp = 0;

     temp = OffsetTP;

     bQuit = CLR;
	
	FND_s1c(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			OffsetTP = temp;
			EE_PUT(120,(unsigned char)OffsetTP);	
			HoReg[8] = (unsigned int)OffsetTP;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 10) temp = 10;
			break;
		case Kdown:			
			temp--;
			if(temp < 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s1c(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}


void SetModeModify09(void)		// Lowest Pressure 
{
	int temp = 0;

     temp = LowPR;

     bQuit = CLR;
	
	FND_2d1b(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			LowPR = temp;
			EE_PUT(122,(unsigned char)LowPR);		 
     		EE_PUT(123,(unsigned char)(LowPR>>8));
			HoReg[9] = (unsigned int)LowPR;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 40) temp = 40;
			break;
		case Kdown:			
			temp--;
			if(temp < 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2d1b(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify10(void)		// Highest pressure
{
	int temp = 0;

     temp = HighPR;

     bQuit = CLR;
	
	FND_2d1b(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			HighPR = temp;
			EE_PUT(124,(unsigned char)HighPR);		 
     		EE_PUT(125,(unsigned char)(HighPR>>8));
			HoReg[10] = (unsigned int)HighPR;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 160) temp = 160;
			break;
		case Kdown:			
			temp--;
			if(temp < 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2d1b(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify11(void)		// Setpoint Pressure
{
	int temp = 0;

     temp = SetPR;

     bQuit = CLR;
	
	FND_2d1b(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			SetPR = temp;
			EE_PUT(126,(unsigned char)SetPR);		 
     		EE_PUT(127,(unsigned char)(SetPR>>8));
			HoReg[11] = (unsigned int)SetPR;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp >= HighPR) temp = HighPR - 1;
			break;
		case Kdown:			
			temp--;
			if(temp <= LowPR) temp = LowPR + 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2d1b(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify12(void)		// Pressure Offset
{
	int temp = 0;

     temp = OffsetPR;

     bQuit = CLR;
	
	FND_s1d1b(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			OffsetPR = temp;
			EE_PUT(128,(unsigned char)OffsetPR);
			HoReg[12] = (unsigned int)OffsetPR;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 10) temp = 10;
			break;
		case Kdown:			
			temp--;
			if(temp < 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_s1d1b(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify13(void)		// Pump Control Mode
{
	int temp = 0;

     temp = PumpMode & 0x01;

     bQuit = CLR;
	
	FND_1d(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			PumpMode = (unsigned char)temp & 0x01;
			EE_PUT(50,(unsigned char)PumpMode);
			HoReg[13] = (unsigned int)PumpMode;
			bQuit = SET;
			break;
		case Kup:
		case Kdown:	
			if(temp == 0x01) temp = 0x00;
			else			  temp = 0x01;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_1d(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify14(void)		// Select Pressure Sensor
{
	int temp = 0;

     temp = SensorMode;

     bQuit = CLR;
	
	FND_2b(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			SensorMode = temp;
			EE_PUT(130,(unsigned char)SensorMode);	
			HoReg[14] = (unsigned int)SensorMode;
			bQuit = SET;
			break;
		case Kup:
		case Kdown:	
			if(temp == 1)  temp = 0;      // 10bar
			else		     temp = 1;      // 16bar
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2b(SensorRange[temp]);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify15(void)		// Communicaiton Addres
{
	int temp = 0;

     temp = ComAddr;

     bQuit = CLR;
	
	FND_2d(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			ComAddr = temp;
			EE_PUT(132,(unsigned char)ComAddr);
			HoReg[15] = (unsigned int)ComAddr;
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 31) temp = 31;
			break;
		case Kdown:			
			temp--;
			if(temp < 1) temp = 1;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_2d(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify16(void)		// Communicaiton Baudrate
{
	int temp = 0;

     temp = IndBaud;

     bQuit = CLR;
	
	FND_1d(temp);
           		
     while(!bQuit) {
		switch(KEY_INPUT()) {
		case Kenter:
			IndBaud = temp;
			EE_PUT(134,(unsigned char)IndBaud);	
			HoReg[16] = (unsigned int)IndBaud;
			UBRR1L = Baudrate[IndBaud];	// 0:4800bps, 1:9600bps, 2:14,4kbps, 3:19.2kbps, 4:28.8kbps
									// 5:38.4kbps, 6:57.6kbps, 7:76.8kbps, 8:115.2kbps
			bQuit = SET;
			break;
		case Kup:
			temp++;
			if(temp > 8) temp = 8;
			break;
		case Kdown:			
			temp--;
			if(temp < 0) temp = 0;
			break;
		case Kmode:
			bQuit = SET;
			break;
		default:
			if(TimeSchFlag & 0x04){
				FND_1d(temp);
				TimeSchFlag &= ~0x04;			// 400ms Time-Flag Clear
			}
			break;
		}
     }
	bQuit = CLR;
}

void SetModeModify(char item)
{
	switch(item){
		case 0: 						// System Run Delay
				SetModeModify00();
				break;
		case 1:						// System Stop Delay
				SetModeModify01();
				break;
		case 2:						// Delay for Warning & Alarm after Power ON
				SetModeModify02();
				break;
		case 3:						// Delay for Warning & Alarm in Running
				SetModeModify03();
				break;
		case 4:						// Oil temperature for warning
				SetModeModify04();
				break;
		case 5:						// Oil temperature for alarm
				SetModeModify05();
				break;
		case 6:						// Fan Run Temperature
				SetModeModify06();
				break;
		case 7:						// Fan Stop Temperature
				SetModeModify07();
				break;
		case 8:						// Temperature Offset
				SetModeModify08();
				break;
		case 9:						// Lowest Pressure
				SetModeModify09();
				break;
		case 10:						// Highest pressure
				SetModeModify10();
				break;
		case 11:						// Setpoint Pressure
				SetModeModify11();
				break;
		case 12:						// Pressure Offset
				SetModeModify12();
				break;
		case 13:						// Pump Control Mode
				SetModeModify13();
				break;
		case 14:						// Select Pressure Sensor
				SetModeModify14();
				break;;
		case 15:						// Communicaiton Addres
				SetModeModify15();
				break;
		case 16:						// Communication Baudrate
				SetModeModify16();
				break;
		default:	break;
	}
}
void SetModeDisplay(char item)
{
	switch(item){
		case 0: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'S';
			CHAR_FND[2] = 'r';
			CHAR_FND[3] = 'd';
			break;			
		case 1: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'S';
			CHAR_FND[2] = 'S';
			CHAR_FND[3] = 'd';
			break;
		case 2: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'S';
			CHAR_FND[2] = 'a';
			CHAR_FND[3] = 'd';
			break;			
		case 3: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'r';
			CHAR_FND[2] = 'a';
			CHAR_FND[3] = 'd';
			break;
		case 4: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'o';
			CHAR_FND[2] = 'W';
			CHAR_FND[3] = 't';
			break;
		case 5: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'o';
			CHAR_FND[2] = 'a';
			CHAR_FND[3] = 't';
			break;
		case 6: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'F';
			CHAR_FND[2] = 'r';
			CHAR_FND[3] = 't';
			break;
		case 7: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'F';
			CHAR_FND[2] = 'S';
			CHAR_FND[3] = 't';
			break;
		case 8: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'o';
			CHAR_FND[2] = 'F';
			CHAR_FND[3] = 't';
			break;
		case 9: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'L';
			CHAR_FND[2] = 'P';
			CHAR_FND[3] = 'r';
			break;
		case 10: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'h';
			CHAR_FND[2] = 'P';
			CHAR_FND[3] = 'r';
			break;
		case 11: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'S';
			CHAR_FND[2] = 'P';
			CHAR_FND[3] = 'r';
			break;
		case 12: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'o';
			CHAR_FND[2] = 'F';
			CHAR_FND[3] = 'p';
			break;
		case 13: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'M';
			CHAR_FND[2] = 'o';
			CHAR_FND[3] = 'd';
			break;
		case 14: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'P';
			CHAR_FND[2] = 'r';
			CHAR_FND[3] = 'S';
			break;
		case 15: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'a';
			CHAR_FND[2] = 'd';
			CHAR_FND[3] = 'r';
			break;
		case 16: 	
			CHAR_FND[0] = ' ';
			CHAR_FND[1] = 'b';
			CHAR_FND[2] = 'P';
			CHAR_FND[3] = 'S';
			break;
		default:	break;
	}
}

void SetMode(void)
{
	int iSetMode = 0, preSetMode = 0;				// 설정 보기 인덱스
	bPASS = CLR;
	
	SetModeDisplay(iSetMode);		// 운전설정 첫번째 항목(기동압력)
		
     while(!bPASS) {
		switch(KEY_INPUT()) {
		case Kenter:
			SetModeModify(iSetMode);
			preSetMode = 17;
			break;
		case Kup:
			iSetMode--;
			if(iSetMode < 0) iSetMode = 16;
			break;
		case Kdown:			
			iSetMode++;
			if(iSetMode > 16) iSetMode = 0;
			break;
		case Kmode:
			bPASS = SET;
			break;
		default:
			if(preSetMode != iSetMode){
				SetModeDisplay(iSetMode);
				preSetMode = iSetMode;
			}
			break;
		}
	}
}

// *************************************************************************************************
//  Main Menu
// *************************************************************************************************
void ErrorModeDisplay(char item)
{
  switch(item){
	case 0:
		CHAR_FND[0] = 't';
		CHAR_FND[1] = 'p';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'W';
		break;			
	case 1: 	
		CHAR_FND[0] = 't';
		CHAR_FND[1] = 'p';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'a';
		break;
	case 2: 	
		CHAR_FND[0] = 'p';
		CHAR_FND[1] = 'r';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'L';
		break;			
	case 3: 	
		CHAR_FND[0] = 'p';
		CHAR_FND[1] = 'r';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'h';
		break;
	case 4: 	
		CHAR_FND[0] = 'L';
		CHAR_FND[1] = 'v';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'W';
		break;
	case 5: 	
		CHAR_FND[0] = 'L';
		CHAR_FND[1] = 'v';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'a';
		break;
	case 6: 	
		CHAR_FND[0] = 'L';
		CHAR_FND[1] = 'v';
		CHAR_FND[2] = '-';
		CHAR_FND[3] = 'F';
		break;
	default:
		break;
  }
}

void AlarmMode(void)
{
  	int i;
	bPASS = CLR;
	
	if(NowERROR){
		for(i=0;i<7;i++) {
			if(NowERROR & MaskW[i]) {
				Error_index = i;	// 현재 고장이 발생한 항목이 있을때까지
				break;
			}
		}
		ErrorModeDisplay(Error_index);
	}else {
		Flag_Disp = 0;
		return;
	}
	PreERROR = NowERROR;
	
     while(!bPASS) {
		switch(KEY_INPUT()) {
		case Kenter:
			break;
		case Kreset:
			CNT_WarningTP = 0;		// Temperature Warning Count
			CNT_AlarmTP = 0;		// Temperature Alarm Count
			CNT_LowPR = 0;			// Pressure Lowest Count
			CNT_HighPR = 0;		// Pressure Highest Count
			CNT_WarningLV = 0;		// Level Warning Count
			CNT_AlarmLV = 0;		// Level Alarm Count
			NowERROR = 0x00;
			PORTF_Bit5 = 0;
			PORTB_Bit2 = 0;
			break;
		case Kup:
			if(NowERROR) {  		// 고장이 발생한 경우에만
				do {
					Error_index--;
					if(Error_index < 0) Error_index = 6;
				} while( (NowERROR & MaskW[Error_index]) == 0 );
				ErrorModeDisplay(Error_index);
			}else {
				Flag_Disp = 0;
				bPASS = SET;
			}
			break;
		case Kdown:			
			if(NowERROR) {  		// 고장이 발생한 경우에만
				do {
					Error_index++;
					if(Error_index > 6) Error_index = 0;
				} while( (NowERROR & MaskW[Error_index]) == 0 );
				ErrorModeDisplay(Error_index);
			}else {
				Flag_Disp = 0;
				bPASS = SET;
			}
			break;
		case Kmode:
			bPASS = SET;
			break;
		default:
			if(NowERROR){
				if(PreERROR != NowERROR){
					Two_Key = SET;
					KEY = Kup;
					PreERROR = NowERROR;
				}
			}else {
				Flag_Disp = 0;
				bPASS = SET;
			}
			break;
		}
	}
}

// *************************************************************************************************
//  Main Menu
// *************************************************************************************************
void MainMenu(void)
{     
	char toggle = 1;
	int data = 0;
		
	while(1) {		
		switch(KEY_INPUT()) {
		case Kenter:
		  	// When it turn on warning & alarm LED, we can confirm its alarm contents.
			if(Flag_Disp) {
				AlarmMode();
				toggle = 0;
			}	
			break;
		case Kup:
		case Kdown:
		  	toggle ^= 1;
		  	break;
		case Kmode:
			SetMode();
			toggle = 0;
			break;
		default:
			if(TimeSchFlag & 0x10){
				if(!Flag_Disp && NowERROR) {
					Flag_Disp = 1;
					Two_Key = SET;
					KEY = Kenter;
				}else {
					if(toggle){		// Pressure 
						data = NowPR;
						FND_2d1b(data);
					}else { 			// Temperature
						data = NowTP;
						FND_s2c(data);
					}
					
					if(Flag_Disp && (NowERROR == 0x00)) Flag_Disp = 0;
				}
				TimeSchFlag &= ~0x10;			// 400ms Time-Flag Clear
			}				
			break;
		}
	}
} 

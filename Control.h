//######### 1.3ms Interrupt Service Routine ###########
#pragma vector= TIMER0_COMP_vect  	// 1.3 ms TASK for Overload & External Control Signal
__interrupt void T0_COMP(void)
{	
	TCNT0 = 0;
	
	// FND Display
	PORTF |= 0xF0;						// FND Digit All Deselect
	PORTC = A_Font[CHAR_FND[CNT_FND]];		// FND Data Transmission
	cbi(PORTF,CNT_FND+4);				// FND Digit Select
		
	CNT_FND++;						// FND Display Count
	if(CNT_FND > 3) CNT_FND = 0;
	

	//-------------------------------- MODBUS RTU Communication for Inverter -------------------------------------
	Count_Query++;			     						// Inveter로부터 데이터 수신이 되지 않을 경우 카운터
	if(Count_Query > 10) Count_Query = 10;					// 카운터가 5 이상(13ms) 될 경우, 이를 초과하지 않음
	 
	if(!Flag_EndQuery && (Count_Query >= 4) && Flag_Query) {	// 이전 수신된 데이터를 처리하였는가?, 데이터가 수신되었는가?,
		Flag_Query = CLR;								// 수신후 5.2ms 가 경과하였는가? 맞다면 데이터 수신 플래그를 클리어하고
		Flag_EndQuery = SET;   							// 마지막 데이터가 수신되었다는 플래그를 발생시킴
		NumByte = Count_NumByte;							// 수신된 바이트 수 저장
		Count_NumByte = 0;   							// UART ISR에서 사용된 수신된 바이트 카운트 초기화
	}
}

//################## Error Detection Function ###################
void Task_Temperture(void)				// Check Temperature
{
	if(NowTP >= OilAlarmTemp) {			// Oil Temperature Alarm - System Stop
		CNT_AlarmTP++;
		if(CNT_AlarmTP >= RunAlarmDelay){
			CNT_AlarmTP = RunAlarmDelay;
			NowERROR |=  0x02;
			CNT_WarningTP = 0;			// Temperature Warning Reset
			NowERROR &= ~0x01;
		}
	}else if(NowTP >= OilWarningTemp) {	// Oil Temperature Warning - System Run
		CNT_WarningTP++;
		if(CNT_WarningTP >= RunAlarmDelay){
			CNT_WarningTP = RunAlarmDelay;
			NowERROR |=  0x01;
		}
		CNT_AlarmTP = 0;				// Temperature Alarm Reset
		NowERROR &= ~0x02;
	}
	else {							// Temperature Warning & Alarm Reset
		CNT_AlarmTP = 0;
		CNT_WarningTP = 0;
		NowERROR &= ~0x03;
	}
}

void Task_Pressure(void)					// Check Pressure
{
	if(NowPR <= LowPR) {				// Lowest Pressure - Alarm & System Stop
		CNT_LowPR++;
		if(CNT_LowPR >= RunAlarmDelay){
			CNT_LowPR = RunAlarmDelay;
			NowERROR |=  0x04;
		}
	}
	
	if(NowPR >= HighPR) {				// Highest Pressure - Alarm & System Run
		CNT_HighPR++;
		if(CNT_HighPR >= RunAlarmDelay){
			CNT_HighPR = RunAlarmDelay;
			NowERROR |=  0x08;
		}
	}
}

void Task_Level(void)					// Check Tank Level
{
	unsigned char temp = 0x00;
	
	temp = PINA & 0x60;					// Tank Level
	if(temp & 0x20) InReg[0] |=  0x02;		// Tank Level 1 status(middle level)
	else			 InReg[0] &= ~0x02;
	if(temp & 0x40) InReg[0] |=  0x04;		// Tank Level 2 status(low level)
	else			 InReg[0] &= ~0x04;
	
	if(temp & 0x40) {					// Low Level(tank level signal 2) - Alarm & System Stop
		CNT_AlarmLV++;
		if(CNT_AlarmLV >= RunAlarmDelay){
			CNT_AlarmLV = RunAlarmDelay;
			NowERROR |=  0x20;
			PORTB_Bit5 = 1;			// Tank Low Level Status Relay ON (TL-ARM1 Relay Port)
			if((temp & 0x20) == 0x00) {	// Level Sensor Fault
				NowERROR |=  0x40;
			}
		}
	}

		
	if(temp & 0x20) {					// Middle Level(tank level signal 1) - Warning & System Run
		CNT_WarningLV++;
		if(CNT_WarningLV >= RunAlarmDelay){
			CNT_WarningLV = RunAlarmDelay;
			NowERROR |=  0x10;			// No alarm at Middle Level
			PORTB_Bit4 = 1;			// Tank Middle Level Status Relay ON (TL-ARM2 Relay Port)
		}
	}

}

void Task_Pump(void)				// Pump Fault Check
{
	unsigned char temp = 0x00;
	
	temp = PINA & 0x80;
	if(temp & 0x80) {
		CNT_PumpFault++;
		if(CNT_PumpFault > 2){
			CNT_PumpFault = 2;
			NowERROR |= 0x80;
		}
	}else {
		CNT_PumpFault = 0;
		NowERROR &= ~0x80;
	}
}


void Error_Check(void)
{
//	Task_Temperture();				// Check Temperature
	if(Ctrl & 0x01){				// 펌프 기동 이후에 
		CNT_PrErr++;
		if(CNT_PrErr > 30) {		// 펌프 제어를 시작하고 30초 지연 후에 압력체크
			Task_Pressure();		// Check Pressure
			CNT_PrErr = 30;
		}
	}else {
		CNT_PrErr = 0;
	}
	
	Task_Level();					// Check Tank Level
	Task_Pump();					// Pump Fault Check
	
	if(NowERROR) PORTD_Bit6 = 1;
	else 	   PORTD_Bit6 = 0;
	
	if(NowERROR & 0xAC) {			// Pump Fault, Pressure Low/High, Low Level Alarm
		PORTB_Bit3 = 1;			// Total Alarm Relay ON (ARM Relay Port)
	}else{
		PORTB_Bit3 = 0;			// Total Alarm Relay OFF
	}
	
	InReg[2] = (unsigned int)NowERROR;
}

//################## Control Function ###################
// *************************************************************************************************
// PID제어에 관련한 루틴
// *************************************************************************************************
void PID(void)			// 속도제어할 경우에만 사용함. 
{	
	// PID 제어 루틴
	e2 = e1; e1 = e0;
			
	if( !(Ctrl & 0x01) ) {						// 자동운전 OFF인 경우
		if(PWM_OUT > 20) PWM_OUT -= 20;			// 운전중인 속도에 따라 5초 이내에 시스템 출력 = 0
		else PWM_OUT = 0;
	}else {									// 자동운전 이고, 시스템 정지조건이 아닌 겨우
		if(Flag_Pump){
			e0 = ((float)SetPR - (float)NowPR)/10.;
			
			u0 = KP*(e0 - e1) + KI*e0 + KD*(e0 - 2*e1 + e2);
			if(u0 >  20.) u0 =  20.;                                       //%%%%%%%%%%%%%%%%%%%%
			if(u0 < -20.) u0 = -20.;
			
			PWM_OUT += (int)u0;
		}else {			
			PWM_OUT += 6;						// 6rpm/100ms --> 60rpm/1sec
			if(PWM_OUT > 300) Flag_Pump = SET;		// take 5 second to 300rpm
		}
	}
	
	if(PWM_OUT > 1023) PWM_OUT = 1023;
	if(PWM_OUT < 0) PWM_OUT = 0;
	
	OCR3A = PWM_OUT;
}

void Main_Control(void)
{	
	unsigned char temp = 0x00;
	
	temp = PINA & 0x10;			// System OFF/ON Signal(DI-1, PA.4)
	if(temp & 0x10) InReg[0] |=  0x01;
	else			 InReg[0] &= ~0x01;
	
	// Fan Control (팬은 전원만 투입되면 무조건 운전함.)
	if(NowTP > FanRunTemp){
		PORTD_Bit7 = 1;							// Fan LED ON
		PORTB_Bit7 = 1;							// Fan Relay ON
		InReg[1] |=  0x02;
	}else if(NowTP < FanStopTemp){
		PORTD_Bit7 = 0;							// Fan LED OFF
		PORTB_Bit7 = 0;							// Fan Relay OFF
		InReg[1] &= ~0x02;
	}
	
	// Pump Run/Stop Check	
	if(NowERROR & 0xAC){							// 저수위, 상한/하한압력 고장이 발생하면 자동운전 OFF
		Ctrl &= ~0x01;
	}else {
		if(Ctrl & 0x01){							// if, 자동운전 중
			if((temp & 0x10) == 0x00){				// 자동운전 OFF 신호가 입력되면
				CNT_StopDelay++;
				CNT_RunDelay = 0;
				if(CNT_StopDelay > SystemStopDelay * 10){		// 시스템 정지 신호 지연
					Ctrl &= ~0x01;
					CNT_StopDelay = 0;
				}
			} else CNT_StopDelay = 0;
		}else {									// if, 자동운전 대기
			if((temp & 0x10) == 0x10){				// 자동운전 ON 신호가 입력되면
				CNT_RunDelay++;
				CNT_StopDelay = 0;
				if(CNT_RunDelay > SystemRunDelay*10) {			// 시스템 운전 신호 지연
					Ctrl |= 0x01;
					CNT_RunDelay = 0;
					if(PumpMode & 0x01){
						e0 = 0.; e1 = 0.;
						PWM_OUT = 0;
						Flag_Pump = CLR;
					}
				}
			} else CNT_RunDelay = 0;
		}
	}
	
	// Pump ON/OFF Control
	if(Ctrl & 0x01) {								// Auto ON 
		PORTD_Bit4 = 1;							// Run LED ON	
		PORTD_Bit5 = 1;							// Pump LED ON	
		PORTB_Bit6 = 1;							// Pump Relay ON
		InReg[1] |= 0x01;
	}else {										// Auto OFF		
		if(PumpMode & 0x01){
			if((PWM_OUT < 20)) {					// Speed Control Mode에서 속도출력이 0이 되면 모든 상태 정지로...
				e0 = 0.; e1 = 0.;
				PWM_OUT = 0;
				PORTD_Bit5 = 0;					// Pump LED OFF
				PORTB_Bit6 = 0;					// Pump Relay OFF
				InReg[1] &= ~0x01;
				PORTD_Bit4 = 0;					// Run LED OFF
			}
		}else {
			PORTD_Bit5 = 0;						// Pump LED OFF
			PORTB_Bit6 = 0;						// Pump Relay OFF
			InReg[1] &= ~0x01;
			PORTD_Bit4 = 0;						// Run LED OFF
		}
	}
}

//	if( !(NowERROR & 0x2C) && !(Ctrl & 0x01) && (PINA & 0x10) ){		// 저수위, 상한/하한압력 고장이 없고, 자동운전이 OFF 된 상태에서 ON 신호가 입력되면
//		CNT_RunDelay++;
//		CNT_StopDelay = 0;
//		if(CNT_RunDelay > SystemRunDelay*10) {			// 시스템 운전 신호 지연
//			Ctrl |= 0x01;
//			CNT_RunDelay = 0;
//			if(PumpMode & 0x01){
//				e0 = 0.; e1 = 0.;
//				PWM_OUT = 0;
//				Flag_Pump = CLR;
//			}
//		}
//	}else if( (Ctrl & 0x01) && !(PINA & 0x10) ){			// 자동운전이 ON 된 상태에서 OFF 신호가 입력되면
//		CNT_StopDelay++;
//		CNT_RunDelay = 0;
//		if(CNT_StopDelay > SystemStopDelay * 10){		// 시스템 정지 신호 지연
//			Ctrl &= ~0x01;
//			CNT_StopDelay = 0;
//		}
//	}else {
//		CNT_RunDelay = 0;
//		CNT_StopDelay = 0;
//	}
	
	
void Process_KeyScan(void)
{
	unsigned char key = 0x00;
	
	if(Start_KeyScan){
		key = ~PINA & 0x0F;
		
		if(key){
			if(key != PreKeyScan){
				PreKeyScan = key;
				SameKeyRepeat = 0;
//				SameModeKey = 0;
//				NotUpDownKey = 0;
			}
			
			SameKeyRepeat++;
			
			if(SameKeyRepeat >= 2){				// 0.2초 동안 동일한 Key가 입력되면
//				if(key == Kmode){
//					SameModeKey++;
//					if(SameModeKey >= 4) {		// 2초 Delay
//						KeyScan = key;
//						Finish_KeyScan = SET;
//						SameModeKey = 0;
//						FLG_BUZZ = SET;
//					}
//				}else {
					if((key == Kup) && (key ==Kdown)){
						KeyScan = key;
						Finish_KeyScan = SET;
						FLG_BUZZ = SET;
					}else {
						NotUpDownKey++;
						if(NotUpDownKey >= 2){	// 0.6초 Delay
							KeyScan = key;
							Finish_KeyScan = SET;
							FLG_BUZZ = SET;
							NotUpDownKey = 0;
						}
					}
//				}
				SameKeyRepeat = 0;
			}
		}else {
			PreKeyScan = 0x00;
			SameKeyRepeat = 0;
			SameModeKey = 0;
			NotUpDownKey = 0;
		}
	}
}

// *************************************************************************************************
// Timer1 Interrupt Service Routine for Main Control                                
// *************************************************************************************************
#pragma vector=TIMER1_COMPA_vect  
__interrupt void T1A_COMP(void)
{    
//	 register int i;

     TCNT1 = 0;                              			// 카운터 클리어 for 100ms Task  
     SREG |= 0x80;									// Global Interrupt Enable  

	asm (" WDR ");
	
	Cnt_Task1++;                                                               
     if(Cnt_Task1 > 600) Cnt_Task1 = 0;  				// 1분
		
      					   TimeSchFlag |= 0x01;
     if( (Cnt_Task1%2)    == 0 ) TimeSchFlag |= 0x02;		// 200 ms
     if( (Cnt_Task1%4)    == 0 ) TimeSchFlag |= 0x04;		// 400 ms
     if( (Cnt_Task1%10)   == 0 ) TimeSchFlag |= 0x08;		// 1 sec
     if( (Cnt_Task1%20)   == 0 ) TimeSchFlag |= 0x10;  	// 2 sec
     if( (Cnt_Task1%100)  == 0 ) TimeSchFlag |= 0x20;  	// 10 sec
     
	Process_KeyScan();								// Key Scan
	
	TASK_temperature();								// Temperature	
	TASK_pressure();								// Pressure
	
	if(FLG_BUZZ){									// Turn off when passed 200ms after Buzzer On
		PORTG_Bit0 = 1;
		CNT_BUZZER++;
		if(CNT_BUZZER >= 2) {
			FLG_BUZZ = CLR;
			PORTG_Bit0 = 0;
			CNT_BUZZER = 0;
		}
	}
	
	if(FLG_CheckAlarm == CLR) {
		CNT_CheckAlarm++;
		if(CNT_CheckAlarm > StartAlarmDelay*10) FLG_CheckAlarm = SET;
	}
			
	if((Cnt_Task1 % 10) == 9){						// 1 second Task
		if(FLG_CheckAlarm) Error_Check();				// ERROR Detection
	}	
	
	if(PumpMode & 0x01) PID();						// Speed Control at Jumper Short
			
	Main_Control();
	
	TASK_BMS_COMM();
}

// ************************************************************************************************ 
// 통신관련 인터럽터 서비스 루틴                                                                            
// ************************************************************************************************ 
#pragma vector=USART1_TX_vect  		// MCU-HMI TX ISR
__interrupt void USART1_TX(void)
{            
	INT8U c, err;
	c = CommGetTxChar(&err);		
	if(err == COMM_TX_EMPTY) {
		// 송신인터럽트 비활성화  
		UCSR1B &= ~0x40;
	} else {
		// 문자를 송신한다
		UDR1 = c;       
	}      
}  
 
#pragma vector=USART1_RX_vect  		// MCU-HMI RX ISR
__interrupt void USART1_RX(void)
{
	INT8U c;

	c = UDR1;
	CommPutRxChar(c); 

	Flag_Query = SET;
	Count_Query = 0;
	Count_NumByte++;
}   
 
void	CommTxIntEn(void)			// 송신 인터럽트 인에이블 
{
	INT8U c, err;
     
     if(UCSR1A & 0x20) {
          UCSR1B |= 0x40;
          c = CommGetTxChar(&err);		// trigger data 
          if(err != COMM_TX_EMPTY) UDR1 = c;
     }
}  
 
void TXD_char(unsigned char data)
{
	while((UCSR1A & 0x20) == 0x00);		// data register empty ?
	UDR1 = data;
}
 
//%%%%%%%%%%%%%%  CRC  %%%%%%%%%%%%%%
__flash unsigned char aucCRCHi[256] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};
 
__flash unsigned char aucCRCLo[256] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};
 
unsigned int usMBCRC16( unsigned char * pucFrame, unsigned int usLen )
{
    unsigned char           ucCRCHi = 0xFF;
    unsigned char           ucCRCLo = 0xFF;
    int             iIndex = 0;
 
    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = (unsigned char)( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return (unsigned int)( ucCRCLo << 8 | ucCRCHi );
}


//%%%%%%%%%%%%% Packet Configuration %%%%%%%%%%%%%%

void TX_ModBus(void)
{
	int i;
	
	for(i=0;i<Index_ModBusTX;i++) TXD_char(TX_BUFF[i]);
	Delay_ms(1);
	
	Index_ModBusTX = 0;
	Flag_ModBusTX = CLR;
}

/* ----------------------------------------------------------------------------------------- */
/* 통신관련 서브루틴                                                                         */
/* ----------------------------------------------------------------------------------------- */
//%%%%%%%%%%%%% Packet Configuration %%%%%%%%%%%%%%
int HWrite_HoReg(unsigned int RegAddr, unsigned int data)
{
	int max = 0, min=0;
	
	switch(RegAddr) {
	case 0:		// System Run Delay
		max = 10;	
		min = 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		SystemRunDelay = (int)data;
		EE_PUT(100,(unsigned char)SystemRunDelay); 							
		EE_PUT(101,(unsigned char)(SystemRunDelay>>8)); 
		HoReg[0] = (unsigned int)SystemRunDelay;
		break;
	case 1:		// System Stop Delay
		max = 120;	
		min = 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		SystemStopDelay = (int)data;
		EE_PUT(102,(unsigned char)SystemStopDelay); 							
		EE_PUT(103,(unsigned char)(SystemStopDelay>>8)); 
		HoReg[1] = (unsigned int)SystemStopDelay;
		break;		
	case 2:		// Delay for Warning & Alarm after Power ON
		max = 20;	
		min = 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		StartAlarmDelay = (int)data;
		EE_PUT(104,(unsigned char)StartAlarmDelay); 							
		EE_PUT(105,(unsigned char)(StartAlarmDelay>>8)); 
		HoReg[2] = (unsigned int)StartAlarmDelay;
		break;	
	case 3:		// Delay for Warning & Alarm in Running
		max = 10;	
		min = 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		RunAlarmDelay = (int)data;
		EE_PUT(106,(unsigned char)RunAlarmDelay); 							
		EE_PUT(107,(unsigned char)(RunAlarmDelay>>8)); 
		HoReg[3] = (unsigned int)RunAlarmDelay;
		break;	
	case 4:		// Oil temperature for warning
		max = OilAlarmTemp;	
		min = 50; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		OilWarningTemp = (int)data;
		EE_PUT(108,(unsigned char)OilWarningTemp); 							
		EE_PUT(109,(unsigned char)(OilWarningTemp>>8)); 
		HoReg[4] = (unsigned int)OilWarningTemp;
		break;	
	case 5:		// Oil temperature for alarm
		max = 99;	
		min = OilWarningTemp+1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		OilAlarmTemp = (int)data;
		EE_PUT(110,(unsigned char)OilAlarmTemp);		 
     	EE_PUT(111,(unsigned char)(OilAlarmTemp>>8));
		HoReg[5] = (unsigned int)OilAlarmTemp;
		break;			
	case 6:		// Fan Run Temperature 
		max = 50;	
		min = FanStopTemp + 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		FanRunTemp = (int)data;
		EE_PUT(116,(unsigned char)FanRunTemp);		 
     	EE_PUT(117,(unsigned char)(FanRunTemp>>8));				
		HoReg[6] = (unsigned int)FanRunTemp;
		break;	
	case 7:		// Fan Stop Temperature
		max = FanRunTemp - 1;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		FanStopTemp = (int)data;
		EE_PUT(118,(unsigned char)FanStopTemp);		 
     	EE_PUT(119,(unsigned char)(FanStopTemp>>8));				
		HoReg[7] = (unsigned int)FanStopTemp;
		break;		
	case 8:		// Temperature Offset
		max = 10;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		OffsetTP = (int)data;
		EE_PUT(120,(unsigned char)OffsetTP);	
		HoReg[8] = (unsigned int)OffsetTP;
		break;	
	case 9:		// Lowest Pressure
		max = 40;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		LowPR = (int)data;
		EE_PUT(122,(unsigned char)LowPR);		 
     	EE_PUT(123,(unsigned char)(LowPR>>8));
		HoReg[9] = (unsigned int)LowPR;
		break;	
	case 10:		// Highest pressure
		max = 160;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		HighPR = (int)data;
		EE_PUT(124,(unsigned char)HighPR);		 
     	EE_PUT(125,(unsigned char)(HighPR>>8));
		HoReg[10] = (unsigned int)HighPR;
		break;	
	case 11:		// Setpoint Pressure
		max = HighPR-1;	
		min = LowPR+1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		SetPR = (int)data;
		EE_PUT(126,(unsigned char)SetPR);		 
     	EE_PUT(127,(unsigned char)(SetPR>>8));
		HoReg[11] = (unsigned int)SetPR;
		break;	
	case 12:		// Pressure Offset
		max = 10;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		OffsetPR = (int)data;
		EE_PUT(128,(unsigned char)OffsetPR);
		HoReg[12] = (unsigned int)OffsetPR;
		break;	
	case 13:		// Pump Control Mode		
		PumpMode = (unsigned char)data & 0x01;
		EE_PUT(50,(unsigned char)PumpMode);
		HoReg[13] = (unsigned int)PumpMode;
		break;	
	case 14:		// Select Pressure Sensor
		if(((int)data == 0) || ((int)data ==  1)) return(0);	
		
		SensorMode = (int)data;
		EE_PUT(130,(unsigned char)SensorMode);	
		HoReg[14] = (unsigned int)SensorMode;
		break;	
	case 15:		// Communicaiton Addres
		max = 31;	
		min = 1; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		ComAddr = (int)data;
		EE_PUT(132,(unsigned char)ComAddr);
		HoReg[15] = (unsigned int)ComAddr;
		break;	
	case 16:		// System Stop Delay
		max = 8;	
		min = 0; 
		if(((int)data < min) || ((int)data >  max)) return(0);	
		
		IndBaud = (int)data;
		EE_PUT(134,(unsigned char)IndBaud);	
		HoReg[16] = (unsigned int)IndBaud;
		UBRR1L = Baudrate[IndBaud];	// 0:4800bps, 1:9600bps, 2:14,4kbps, 3:19.2kbps, 4:28.8kbps
								// 5:38.4kbps, 6:57.6kbps, 7:76.8kbps, 8:115.2kbps
		break;
	default:	return(0);
	}
	return(1);
}
//######################## 송신 루틴 ###############################
void TX_Read_HoReg(unsigned int RegAddr, unsigned int NumReg)		// Read Holding Register
{       
     unsigned int data = 0, temp = 0, RegInd = 0;
     int index = 0, i, j;
     int AddrLimit = 0;
	
	index = (int)RegAddr - 10;	
	AddrLimit = index + (int)NumReg;
	
	if((index < 0) && (index > 16)){					// Holding Register Quantity : 0 ~ 16
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x83;
	     // Exception Code
	     TX_BUFF[2] = 0x02;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
		
	     // Transmit the packet	     	
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
		return;
	}
	
	if((AddrLimit < 18) && (NumReg <= 17)){				// Holding Register 주소범위를 이내 이고, 읽을 갯수가 10개 이내인 경우 정상적인 응답
		// Slave ID
		TX_BUFF[0] = (unsigned char)ComAddr;
		// Function Code
		TX_BUFF[1] = 0x03;
		// Byte Count
		TX_BUFF[2] = (unsigned char)(NumReg*2);
		// Data
		for(i=0;i<NumReg;i++){
			data = HoReg[index];
			TX_BUFF[i*2 + 3] = (unsigned char)((data & 0xFF00) >> 8);
			TX_BUFF[i*2 + 4] = (unsigned char)( data & 0x00FF);
			index++;
		}
		// CRC
		RegInd = NumReg*2 + 3;
		temp = usMBCRC16(TX_BUFF,RegInd);
		
		j = (int)RegInd;
		TX_BUFF[j++] = (unsigned char)((temp & 0xFF00) >> 8);
		TX_BUFF[j++] = (unsigned char)( temp & 0x00FF);
		
		// Transmit the packet			
		Flag_ModBusTX = SET;
		Index_ModBusTX = j;
	}else {
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x83;
	     // Exception Code
	     TX_BUFF[2] = 0x02;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
		
	     // Transmit the packet	     	
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
	}
} 

void TX_Read_InReg(unsigned int RegAddr, unsigned int NumReg)		// Read Input Register
{       
     unsigned int data = 0, temp = 0, RegInd = 0;
     int i, j, index = 0;
     int AddrLimit = 0;
	
	index = (int)RegAddr;
	AddrLimit = index + (int)NumReg;
	
	if((index < 0) && (index > 4)){					// Input Register Quantity : 0 ~ 4
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x84;
	     // Exception Code
	     TX_BUFF[2] = 0x02;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
		
	     // Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
		return;
	}
	
	if((AddrLimit < 6) && (NumReg <= 5)){				// Input Register 주소범위를 이내 이고, 읽을 갯수가 10개 이내인 경우 정상적인 응답
		// Slave ID
		TX_BUFF[0] = (unsigned char)ComAddr;
		// Function Code
		TX_BUFF[1] = 0x04;
		// Byte Count
		TX_BUFF[2] = (unsigned char)(NumReg*2);
		 // Data
		for(i=0;i<NumReg;i++){
			data = InReg[index];
			TX_BUFF[i*2 + 3] = (unsigned char)((data & 0xFF00) >> 8);
			TX_BUFF[i*2 + 4] = (unsigned char)( data & 0x00FF);
			index++;
		}
		// CRC
		RegInd = NumReg*2 + 3;
		temp = usMBCRC16(TX_BUFF,RegInd);
		
		j = (int)RegInd;
		TX_BUFF[j++] = (unsigned char)((temp & 0xFF00) >> 8);
		TX_BUFF[j++] = (unsigned char)( temp & 0x00FF);
		
		// Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = j;
	}else {
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x84;
	     // Exception Code
	     TX_BUFF[2] = 0x02;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
		
	     // Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
	}
} 

void TX_Write_HoReg(unsigned int RegAddr, unsigned int RegValue)		// Write Holding Register
{       
     unsigned int data = 0, temp = 0;
     int index = 0;
	
	index = (int)RegAddr-10;	
	
	if((index < 0) && (index > 16)){	// Holding register Quantity : 0 ~ 16
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x86;
	     // Exception Code
	     TX_BUFF[2] = 0x02;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
		
	     // Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
		return;
	}
	
     if(HWrite_HoReg(index, RegValue)){
	     // Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Function Code
	     TX_BUFF[1] = 0x06;
	     // Byte Count
	     TX_BUFF[2] = (unsigned char)((RegAddr & 0xFF00) >> 8);
	     TX_BUFF[3] = (unsigned char)( RegAddr & 0x00FF);
	     // Data
	     data = HoReg[index];
	     TX_BUFF[4] = (unsigned char)((data & 0xFF00) >> 8);
	     TX_BUFF[5] = (unsigned char)( data & 0x00FF);
	     // CRC
	     temp = usMBCRC16(TX_BUFF,6);
	     TX_BUFF[6] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[7] = (unsigned char)(temp & 0x00FF);
	     // Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = 8;
	}else{
		// Slave ID
	     TX_BUFF[0] = (unsigned char)ComAddr;
	     // Error Code
	     TX_BUFF[1] = 0x86;
	     // Exception Code
	     TX_BUFF[2] = 0x03;
	     // CRC
	     temp = usMBCRC16(TX_BUFF,3);
	     TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	     TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
	     // Transmit the packet
		Flag_ModBusTX = SET;
		Index_ModBusTX = 5;
	}
} 

void TX_Exception_Code(unsigned char excode)
{
	unsigned int temp = 0;
	
	// Slave ID
	TX_BUFF[0] = (unsigned char)ComAddr;
	// Error Code
	TX_BUFF[1] = 0x80 + excode;
	// Exception Code
	TX_BUFF[2] = 0x01;
	// CRC
	temp = usMBCRC16(TX_BUFF,3);
	TX_BUFF[3] = (unsigned char)((temp & 0xFF00) >> 8);
	TX_BUFF[4] = (unsigned char)(temp & 0x00FF);
	// Transmit the packet
	Flag_ModBusTX = SET;
	Index_ModBusTX = 5;
}

void Comm_Reset(void)
{
	__disable_interrupt();   // Global Interrupt Disable
     CommInit();            	// Communication Init  
	
	Flag_Query = CLR;
	Count_Query = 0;
	Count_NumByte = 0;
	Flag_EndQuery = CLR;
	NumByte = 0;
	Flag_ModBusTX = CLR;
	Index_ModBusTX = 0;
	    
	__enable_interrupt();   	// Global Interrupt Enable
}

//######################## 수신 패킷분석 루틴 ###############################
void PACKET_CHK(void) 				// 디스플레이 보드와 통신
{
     INT8U err;
	
	unsigned int i, Cal_CRC, Rec_CRC, RegAddr, NumReg, RegValue;
	unsigned char c = 0x00, FunCode = 0x00;
	char PacketLen = 0, DataLen = 0;
	
	i = 0;
	c = 0x00;
	Cal_CRC = 0; 
	Rec_CRC = 0;
	
	//************** Enter the received packet and address check, CRC *************
	RegAddr = 0x0000;
	NumReg = 0x0000;
	RegValue = 0x0000;
	
	PacketLen = NumByte;
	DataLen = NumByte-2;
	NumByte = 0;
	
	for(i=0;i<40;i++) TX_BUFF[i] = 0x00;
	for(i=0;i<20;i++) RX_BUFF[i] = 0x00;
	
	if(PacketLen >= 20) {
		Comm_Reset();
		return;
	}
	
	for(i=0;i<PacketLen;i++) {				// 링버퍼로부터 데이터를 읽어옴
		c = 0x00;
     	c = CommGetChar(&err);
     	RX_BUFF[i] = c;					// 데이터 저장
     } 
     Flag_EndQuery = CLR;     
     
     if(RX_BUFF[0] != (unsigned char)ComAddr){
		Comm_Reset();
		return;
	}
     
     Cal_CRC = usMBCRC16(RX_BUFF,DataLen);
     Rec_CRC = ((unsigned int)RX_BUFF[PacketLen-2]<<8) | (unsigned int)RX_BUFF[PacketLen-1];
     
     if(Cal_CRC != Rec_CRC) {
		Comm_Reset();
		return;  
	}
     
     //************** Analsys for Packet *************
     FunCode = RX_BUFF[1];		// Function Code
     switch(FunCode){
	     case 0x03:			// Read Holding Register
     		RegAddr = (unsigned int)RX_BUFF[2]<<8 | (unsigned int)RX_BUFF[3];
     		NumReg  = (unsigned int)RX_BUFF[4]<<8 | (unsigned int)RX_BUFF[5];
	     	TX_Read_HoReg(RegAddr, NumReg);
	     	break;
	    	case 0x04:			// Read Input Register
     		RegAddr = (unsigned int)RX_BUFF[2]<<8 | (unsigned int)RX_BUFF[3];
     		NumReg  = (unsigned int)RX_BUFF[4]<<8 | (unsigned int)RX_BUFF[5];
	    		TX_Read_InReg(RegAddr, NumReg);
	    		break;
	    	case 0x06:			// Write Holding Register
	    		RegAddr  = (unsigned int)RX_BUFF[2]<<8 | (unsigned int)RX_BUFF[3];
	    		RegValue = (unsigned int)RX_BUFF[4]<<8 | (unsigned int)RX_BUFF[5];
	    		TX_Write_HoReg(RegAddr, RegValue);
	    		break;
		default:
			TX_Exception_Code(FunCode);
			break;
    	}
}

//######################## 통신 제어 루틴 ###############################
void TASK_BMS_COMM(void)			//100ms 단위로 수행
{
	if(Flag_ModBusTX){
		TX_ModBus();
	}else {
		if(Flag_EndQuery) {
			PACKET_CHK();				//패킷 분석
			Flag_EndQuery = CLR;
		}
	}
}

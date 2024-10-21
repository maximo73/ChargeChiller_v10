// ************************************************************************************************ 
// 통신관련 인터럽터 서비스 루틴                                                                            
// ************************************************************************************************ 
#pragma vector=USART0_TX_vect  		// MCU-Inverter TX ISR
__interrupt void USART0_TX(void)
{            
INT8U c, err;
c = CommGetTxChar(COMM1,&err);		
if(err == COMM_TX_EMPTY) {
// 송신인터럽트 비활성화  
UCSR0B &= ~0x40;
} else {
// 문자를 송신한다
UDR0 = c;    
}  
}    
 
#pragma vector=USART0_RX_vect  		// MCU-Inverter RX ISR
__interrupt void USART0_RX(void)
{
INT8U c;

c = UDR0;
CommPutRxChar(COMM1,c);

Flag_Query1 = SET;
Count_Query1 = 0;
Count_NumByte1++;
} 
 
#pragma vector=USART1_TX_vect  		// MCU-HMI TX ISR
__interrupt void USART1_TX(void)
{            
INT8U c, err;
c = CommGetTxChar(COMM2,&err);		
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
CommPutRxChar(COMM2, c); 

Flag_Query2 = SET;
Count_Query2 = 0;
Count_NumByte2++;
}   
 
void	CommTxIntEn(INT8U ch)			// 송신 인터럽트 인에이블 
{
INT8U c, err;

switch (ch) {                                       	// 통신 채널에 대한 포인터를 얻는다
case COMM1:
if(UCSR0A & 0x20) {
UCSR0B |= 0x40;
c = CommGetTxChar(COMM1,&err);		// trigger data 
if(err != COMM_TX_EMPTY) UDR0 = c;
}
break;
case COMM2:
if(UCSR1A & 0x20) {
UCSR1B |= 0x40;
c = CommGetTxChar(COMM2, &err);		// trigger data 
if(err != COMM_TX_EMPTY) UDR1 = c;
}
break;
}
}  
 
void TXD_char(unsigned char ch, unsigned char data)
{
switch(ch) {
case COMM1: 
while((UCSR0A & 0x20) == 0x00);		// data register empty ?
UDR0 = data;
               break;
case COMM2: 
while((UCSR1A & 0x20) == 0x00);		// data register empty ?
UDR1 = data;
break;
default:  return;
}
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
int Update_Register(unsigned int *RxPacket)
{
	//	unsigned char temp0 = 0x00;
	unsigned int RegAddr = 0, data = 0;
	unsigned char AutoRunBit = 0x00;
	char Flag_Schedule = CLR;
	int temp = 0;
	int SetPR_dp = 0;
	int max, min;	

	RegAddr = RxPacket[0]-100;					// 주소값
	data = RxPacket[1];						// 주소값의 변경된 값
	AutoRunBit = (unsigned char)RxPacket[2];	// 자동운전 ON/OFF

	if((RmtCtrl & 0x01) == 0x00){		// 원격자동운전 Enable이 되었을 경우에는 HMI에 의한 제어 안됨.
		if(ControlBit != AutoRunBit){	// 현재 자동운전제어와 입력된 자동운전제어가 다르면
			if(!(ControlBit & 0x01) && (AutoRunBit & 0x01)){			// 현재 자동운전제어는 OFF 인데, 입력 자동운전제어가 ON 이면
				if(SetupBit & MaskW[1]){							// 배관채움 기능이 활성화 되어있는지 판단하고,
					if(iCurPR <= PipeFillPR) ResetBit |= 0x01;		// 활성화 되어 있으면, 현재압력이 배관채움압력 이하이면 배관채움 Flag 발생
				}
			}else if((ControlBit & 0x01) && !(AutoRunBit & 0x0)){		// 먄약, 현재 자동운전이 ON 인데, 입력 자동운전제어가 OFF 인 경우에
				if(ResetBit & 0x01) {								// 배관채움 Flag가 발생되어 배관채움 중이면 
					ResetBit &= ~0x01;						// Flag와 Count를 초기화 한다. 
					Cnt_PipeFill = 0;
				}
			}				
			
			ControlBit = AutoRunBit;				// 자동제어 Start(1)/Stop(0)
			EE_PUT(1,ControlBit);
			HoReg[0] = (unsigned int)ControlBit;
		}
	}

	switch(RegAddr) {
		case 1:										// 제어모드(0: Step, 1: Single Inv, 2: Multi Inv-series, 3: Multi Inv-parellel)
			if(ControlMode != (unsigned char)data){
				ControlMode = (unsigned char)data;
				EE_PUT(3, (unsigned char)ControlMode);
				HoReg[1] = (unsigned int)ControlMode;
				asm ("JMP 0");
			}
			break;

//	############### 중량 ####################

		case 59:														// EEPROM에 토출센서 1 압력 오차보정 span 갱신
			if(AI2_min != data){
			max = 100;											// 최대: 1.00 bar
			min = 0;     											// 최소: 0.00 bar

			if(((int)data < min) || ((int)data >  max)) return(0);			// 설정범위를 벗어나면 리턴.
			AI2_min = (int)data;
			EE_PUT(240, (unsigned char)AI2_min); 							
			EE_PUT(241, (unsigned char)(AI2_min>>8));
			HoReg[59] = (unsigned int)AI2_min;
			asm ("JMP 0");
			}
			break;
		default:	return(0);
		}
	
		return(1);
}
 
void Comm_Reset2(void)
{
	//	int i;

	__disable_interrupt();   // Global Interrupt Disable
		CommInit(2);            	// Communication Init  

	Flag_Query2 = CLR;
	Count_Query2 = 0;
	Count_NumByte2 = 0;
	Flag_EndQuery2 = CLR;
	NumByte2 = 0;
	Flag_ModBusTX2 = CLR;
	Index_ModBusTX2 = 0;
	    
	//	for(i=0;i<50;i++) TX_BUFF2[i] = 0x00;
	//	for(i=0;i<20;i++) RX_BUFF[i] = 0x00;

	__enable_interrupt();   	// Global Interrupt Enable
}
 
void ReadReg_TX(unsigned int StartAddr, unsigned int TxNumber)
{
	int i;
	unsigned int temp = 0x0000;
	unsigned char data[3] = {0x00,};

	data[0] = (unsigned char)((StartAddr & 0xFF00) >> 8);
	data[1] = (unsigned char)( StartAddr & 0x00FF);
	data[2] = (unsigned char) TxNumber;

	// Slave ID
	TX_BUFF2[0] = 0x01;
	// Function Code
	TX_BUFF2[1] = 0x03;
	// Address
	TX_BUFF2[2] = data[0];
	TX_BUFF2[3] = data[1];	
	// Number of Data		
	TX_BUFF2[4] = 0x00;
	TX_BUFF2[5] = data[2];
	// CRC
	temp = usMBCRC16(TX_BUFF2,6);
	TX_BUFF2[6] = (unsigned char)((temp & 0xFF00) >> 8);
	TX_BUFF2[7] = (unsigned char)(temp & 0x00FF);

	for(i=0;i<8;i++) TXD_char(COMM2, TX_BUFF2[i]);
}
 
void WriteReg_TX(unsigned int StartAddr, unsigned int TxNumber, unsigned int *TxData)
{
	int i, j = 0;
	unsigned int data1= 0, temp = 0;
	unsigned char data[4] = {0x00,};

	data[0] = (unsigned char)((StartAddr & 0xFF00) >> 8);
	data[1] = (unsigned char)( StartAddr & 0x00FF);
	data[2] = (unsigned char) TxNumber;
	data[3] = (unsigned char)(TxNumber * 2);

	// Slave ID
	TX_BUFF2[0] = 0x01;	
	// Function Code
	TX_BUFF2[1] = 0x10;	
	// Address
	TX_BUFF2[2] = data[0];
	TX_BUFF2[3] = data[1];	
	// Number of Data		
	TX_BUFF2[4] = 0x00;
	TX_BUFF2[5] = data[2];	
	// Byte of Data
	TX_BUFF2[6] = data[3];	
	// Data
	for(i=0;i<(int)data[2];i++){
		data1 = TxData[i];
		TX_BUFF2[2*i+7] = (unsigned char)((data1 & 0xFF00) >> 8);
		TX_BUFF2[2*i+8] = (unsigned char)( data1 & 0x00FF);
	}
	// CRC
	j = (int)data[2]*2+7;
	temp = usMBCRC16(TX_BUFF2,j);
	TX_BUFF2[j] = (unsigned char)((temp & 0xFF00) >> 8);
	TX_BUFF2[j+1] = (unsigned char)(temp & 0x00FF);

	for(i=0;i<j+2;i++) TXD_char(COMM2, TX_BUFF2[i]);
}
 
// ############### Normal HMI-PLC Communciaton ################
void TX_Control(void)
{
	int i;
	unsigned int TotalData[20] = {0,};

	if(Flag_TotalTx){	// Transmission for Total Data(Register)
		switch(CNT_TotalParameter){
			case 0:						// Transmission for Input Register 0(System Version)~17(펌프6운전율)
				for(i=0;i<18;i++) TotalData[i] = InReg[i];
				Tx_StartAddr = 0x0000;		// Start Register Address: 0
				Tx_NumberAddr = 0x0012;		// Number of Tx Register: 18
				break;
			case 1:						// Transmission for Input Register 18(펌프1운전시간)~30(펌프6운전횟수)
				for(i=0;i<12;i++) TotalData[i] = InReg[18+i];
				Tx_StartAddr = 0x0012;		// Start Register Address: 18
				Tx_NumberAddr = 0x000C;		// Number of Tx Register: 12
				break;
			case 2:						// Transmission for Holding Register 0(자동제어비트)~19(최고회전율)
				for(i=0;i<20;i++) TotalData[i] = HoReg[i];
				Tx_StartAddr = 0x0064;		// Start Register Address: 100
				Tx_NumberAddr = 0x0014;		// Number of Tx Register: 20
				break;
			case 3:						// Transmission for Holding Register 20(교대가능 회전율)~39(전원제어 운전지연)
				for(i=0;i<20;i++) TotalData[i] = HoReg[20+i];
				Tx_StartAddr = 0x0078;		// Start Register Address: 120
				Tx_NumberAddr = 0x0014;		// Number of Tx Register: 20
				break;
			case 4:						// Transmission for Holding Register 40(전원제어 정지지연)~59(예비)
				for(i=0;i<20;i++) TotalData[i] = HoReg[40+i];
				Tx_StartAddr = 0x008C;		// Start Register Address: 140
				Tx_NumberAddr = 0x0014;		// Number of Tx Register: 20
				break;
		}
		CNT_TotalParameter++;
		if(CNT_TotalParameter > 4) CNT_TotalParameter = 0;
		WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);
	}else {							// Transmission for Writing Periodical Data and Reading Setup Data
		if(Flag_TimeCountClr & Flag_TimeCountClr_Fin){			// 해당 펌프의 운전시간이나 운전횟수를 초기화 하고 나서 HMI의 91, 92를 초기화 함.
			TotalData[0] = 0;
			TotalData[1] = 0;
			Tx_StartAddr = 98;
			Tx_NumberAddr = 2;
			WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);

			Flag_TimeCountClr = CLR;
			Flag_TimeCountClr_Fin = CLR;
			CNT_HoReg = 75;
		}else if(Flag_Return_MSG){							// 원래 파라미터 전송 at Failure for Parameter Update
			TotalData[0] = ReturnVal;
			Tx_StartAddr = RetrunAdd;
			Tx_NumberAddr = 1;
			WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);

			Flag_Return_MSG = CLR;
			RetrunAdd = 0;
			ReturnVal = 0;	
		}else {
			if(toggle_HMI){							// 400ms 마다 실시간 주기적 데이터(디지털입력 ~ 펌프6 운전율) 전송
				for(i=0;i<17;i++) TotalData[i] = InReg[1+i];
				Tx_StartAddr = 0x0001;					// Start Register Address: 1
				Tx_NumberAddr = 0x0011;					// Number of Tx Register: 17
				WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);
			}else {
				if(CNT_HoReg == 75){					// 30초 마다 HMI의 설정변경 사항 체크 대신에 쓰기(펌프1운전시간~펌프6운전횟수) 전송
					for(i=0;i<12;i++) TotalData[i] = InReg[18+i];
					Tx_StartAddr = 0x0012;				// Start Register Address: 18
					Tx_NumberAddr = 0x000C;				// Number of Tx Register: 12
					WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);
					CNT_HoReg = 0;
				}else { 
					if(Flag_SetPR_Change){				// 설정압력과 연동된 설정 변경
						for(i=0;i<7;i++) TotalData[i] = HoReg[11+i];
						Tx_StartAddr = 0x006F;			// Start Register Address: 111
						Tx_NumberAddr = 0x0007;			// Number of Tx Register: 7
						WriteReg_TX(Tx_StartAddr, Tx_NumberAddr, TotalData);
						Flag_SetPR_Change = CLR;
					}else {							// Read HMI's Changed Set Parameter
						Tx_StartAddr = 0x0062;			// Start Register Address: 98
						Tx_NumberAddr = 0x0003;			// Number of Tx Register: 3
						ReadReg_TX(Tx_StartAddr, Tx_NumberAddr);
					}
				}
				CNT_HoReg++;
			}
			toggle_HMI ^= 1;
		}
	}

	Flag_ReplyWait = SET;
}
 
//######################## 수신 패킷분석 루틴 ###############################
void PACKET_CHK2(void) 				// 디스플레이 보드와 통신
{
     INT8U err;

	unsigned int i, Cal_CRC, Rec_CRC, RegAddr, NumReg;
	unsigned int MHoValue[3] = {0,0,0};
	unsigned char c = 0x00, FunCode = 0x00;
	char PacketLen = 0, DataLen = 0, flag_newTX = CLR;
	int temp = 0;

	i = 0;
	c = 0x00;
	Cal_CRC = 0; 
	Rec_CRC = 0;

	//************** Enter the received packet and address check, CRC *************
	RegAddr = 0x0000;
	NumReg = 0x0000;

	PacketLen = NumByte2;
	DataLen = NumByte2-2;
	NumByte2 = 0;

	for(i=0;i<50;i++) TX_BUFF2[i] = 0x00;
	for(i=0;i<20;i++) RX_BUFF2[i] = 0x00;

	if(PacketLen >= 20) {
		Comm_Reset2();
		return;
	}

	for(i=0;i<PacketLen;i++) {				// 링버퍼로부터 데이터를 읽어옴
		c = 0x00;
		c = CommGetChar(COMM2, &err);
		RX_BUFF2[i] = c;					// 데이터 저장
	} 
	Flag_EndQuery2 = CLR;     
		
	if(RX_BUFF2[0] != 0x01){
		Comm_Reset2();
		return;
	}
		
	Cal_CRC = usMBCRC16(RX_BUFF2,DataLen);
	Rec_CRC = ((unsigned int)RX_BUFF2[PacketLen-2]<<8) | (unsigned int)RX_BUFF2[PacketLen-1];
		
	if(Cal_CRC != Rec_CRC) {
		Comm_Reset2();
		return;  
	}
     
     //************** Analsys for Packet *************
        
     FunCode = RX_BUFF2[1];			// Function Code
     switch(FunCode){
		case 0x03:				// Read Holding Register
			if(RX_BUFF2[2] == 0x06){
				MHoValue[0] = (unsigned int)RX_BUFF2[3]<<8 | (unsigned int)RX_BUFF2[4];	// 수정된 Parameter Address
				MHoValue[1] = (unsigned int)RX_BUFF2[5]<<8 | (unsigned int)RX_BUFF2[6];	// 수정된 Parameter Value
				MHoValue[2] = (unsigned int)RX_BUFF2[7]<<8 | (unsigned int)RX_BUFF2[8];	// ControlBit
				Tx_StartAddr = 0x0000;					// Start Register Address: Clear
				Tx_NumberAddr = 0x0000;					// Number of Tx Register: Clear

				if(preMHoValue[0] != MHoValue[0]) flag_newTX = SET;
				preMHoValue[0] = MHoValue[0];
				if(preMHoValue[1] != MHoValue[1]) flag_newTX = SET;
				preMHoValue[1] = MHoValue[1];
				if(preMHoValue[2] != MHoValue[2]) flag_newTX = SET;
				preMHoValue[2] = MHoValue[2];

				if(MHoValue[0] == 98){
					TimeCountClr = MHoValue[2];					
					Flag_TimeCountClr = SET;
					Flag_TimeCountClr_Fin = CLR;
				}else {
					if(flag_newTX){							// addr.98~addr.100 중에 달라진게 있으면 아래의 업데이트 실시.
						temp = Update_Register(MHoValue);
						if(temp == 0){							// 설정값 Update를 실패한 경우, 원래 값을 HMI에 쓰야 함.
							Flag_Return_MSG = SET;
							RetrunAdd = MHoValue[0];				// HMI에서 쓰기를 한 설정값의 주소
							ReturnVal = HoReg[(int)MHoValue[0]-100];	// 원래 설정된 값을 읽어옴.
						}
					}
				}
			}
			Flag_ReplyWait = CLR;
     		break;
    		case 0x10:			// Write Multiple Holding Register
    			RegAddr  = (unsigned int)RX_BUFF2[2]<<8 | (unsigned int)RX_BUFF2[3];
    			NumReg   = (unsigned int)RX_BUFF2[4]<<8 | (unsigned int)RX_BUFF2[5];
					
			if(Flag_TotalTx){
				if((Tx_StartAddr == RegAddr) && (Tx_NumberAddr == NumReg)){
					if(RegAddr == 0x008C){					// 전체 데이터의 마지막 전송에 대한 응답이 들어오면 
						Flag_TotalTx = CLR;					// 전체 데이터 전송 플래그 Clear(마지막 데이터 전송주소: 140)
						CNT_TotalParameter = 0;				// 전체 데이터 전송을 위한 순차 카운터 Clear
					}					
					Tx_StartAddr = 0x0000;					// Start Register Address: Clear
					Tx_NumberAddr = 0x0000;					// Number of Tx Register: Clear
				}else {
					CNT_HMIcommErr++;						// 고장검출을 위한 카운트
					if(CNT_HMIcommErr > 15){
						CNT_HMIcommErr = 0;
						Flag_TotalTx = SET;					// 전체 데이터 전송 플래그 SET(다시 전체데이터 시작)
						CNT_TotalParameter = 0;				// 전체 데이터 전송을 위한 순차 카운터 1
					}
				}
			}else {
				if((Tx_StartAddr == RegAddr) && (Tx_NumberAddr == NumReg)){
					Tx_StartAddr = 0x0000;					// Start Register Address: Clear
					Tx_NumberAddr = 0x0000;					// Number of Tx Register: Clear
				}else {
					CNT_HMIcommErr++;						// 고장검출을 위한 카운트
					if(CNT_HMIcommErr > 15){
						CNT_HMIcommErr = 0;
						Flag_TotalTx = SET;					// 전체 데이터 전송 플래그 SET(다시 전체데이터 시작)
						CNT_TotalParameter = 0;				// 전체 데이터 전송을 위한 순차 카운터 1
					}
				}
				// 200ms 단위로 주기적인 데이터 쓰기(실제설정압력~펌프6운전율)에 대한 전송이 완료되었는지 체크
				// 1분 주기로 데이터 쓰기(펌프1운전시간~펌프6운전횟수)에 대한 전송이 완료되었는지 체크
			}	
			Flag_ReplyWait = CLR;
    			break;
		default:
			break;
	}
}
 
//######################## 통신 제어 루틴 ###############################
void TASK_HMI_COMM(void)						//200ms 단위로 수행
{
	if(Flag_EndQuery2) {
		PACKET_CHK2();					//패킷 분석
		Flag_EndQuery2 = CLR;
	}

	if(Flag_ReplyWait) {
		CNT_HMIreplywait++;				// 데이터 전송에 따른 응답 대기
		if(CNT_HMIreplywait > 15) {		// 3초 이상 응답이 없으면 전체 데이터 
			CNT_HMIreplywait = 0;
			Flag_TotalTx = SET;			// 전체 데이터 전송 플래그 SET(다시 전체데이터 시작)
			Flag_ReplyWait = CLR;
			CNT_TotalParameter = 0;		// 전체 데이터 전송을 위한 순차 카운터 초기화
		}
	}else	TX_Control();
}

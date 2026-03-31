 #include "can_header.h"
 #include "lcd_4bit.h"
int main()
{
	can_msg t1;
	IODIR0|=1<<7;
	IOSET0=1<<7;
	CAN_tx_init();
	UART0_init(115200);

	while(1)
	{
		CAN_rx(&t1);
		if(t1.id==0X000001AF)
		{
		lcd_string("buzzer ON");
		IOCLR0=1<<7;
		}
		else if(t1.id==0x000001BF)
		{
		IOSET0=1<<7;																   
		}
	}
}
void CAN_tx_init(void)
{
	PINSEL1|=0x00014000;//P0.23-->TD2 & P0.24-->RD2
	VPBDIV=1; //PCLK=60MHz
	C2MOD=0x1; //CAN1 into Reset  Mode 
	AFMR=0x2; //accept all receiving messages(data/remote)
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz
	C2MOD=0x0; //CAN1 into Normal Mode
}
void UART0_init(u32 baud)

{
	PINSEL0=0X05;
	U0LCR=0X83;
//	U0DLL=32;
	//U0DLM=0;
	
	switch(baud)
	{
		case 115200:				 //high speed for debugging 11.5kb
					U0DLL=32;
					U0DLM=0;
					break;
		case 921600:				  //higher than 115200
					U0DLL=4;
					U0DLM=0;
		
		default:
				U0DLL=32;

	}
     U0LCR=0X03;
}

void CAN_rx(can_msg *n1)
{
	while((C2GSR&0x1)==0);
	n1->id=C2RID;
	n1->dlc=(C2RFS>>16)&0xF;
	n1->rtr=(C2RFS>>30)&0x1;
	if(n1->rtr==0)
	{ //if data frame
		n1->byteA=C2RDA;
		n1->byteB=C2RDB;
	}
	C2CMR=(1<<2);		
}

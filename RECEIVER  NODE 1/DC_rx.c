#include "can_header.h"

int main()
{
	can_msg t1,t2;
	can_msg r1;
	IODIR0=1<<2;
	CAN_tx_init();
	UART0_init(115200);

	while(1)
	{
		CAN_rx(&r1);
		if(t1.id==0X1AF)
		{
			IOSET0=DC_MOTOR;
		}
		else if(t2.id==0X1BF)
		{
			IOCLR0=DC_MOTOR;
		}
	}
}
void CAN_tx_init(void)
{
	PINSEL1 |=0X00014000;
	C2MOD=0X1;
	AFMR=0X2;
	C2BTR=0X001C001D;
	C2MOD=0X0;
}
void UART0_init(u32 baud)
{
	PINSEL0=0X5;
	U0LCR=0X83;
	switch(baud)
	{
		case 115200:
		U0DLL=32;
		U0DLM=0;
		break;
	}
	U0LCR=0X03;
}
void CAN_rx(can_msg *t1)
{
	while((C2GSR&0X1)==0);
	t1->id=C2RID;	  //the recived message on this register(C2RID1) is assigned to r1->id
	t1->dlc=(C2RFS>>16)&0XF;
	t1->rtr=(C2RFS>>30)&0X1;
	if(t1->rtr==0)		  //rtr=0 data frame
	{
		t1->byteA=C2RDA;
		t1->byteB=C2RDB;
	}
	C2CMR=0X2;	//adort the transmission
		
}

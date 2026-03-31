#include "can_header.h"
int main()
{
 	can_msg m1;
	can_msg m2;
	float f;
	int n;
	lcd_init();
	CAN_tx_init();
	SPI_init(); //hw SPI initialisation
	IOSET0|= 1<<7;//cs initially high
	
	/*sending data frame*/
	m1.id=0x000001AF;
	m1.rtr=0;//data frame
	m1.dlc=4;
	m1.byteA=0;
	m1.byteB=0;
		
	m2.id=0x000001BF;
	m2.rtr=0;//data frame
	m2.dlc=4;
	m2.byteA=0;
	m2.byteB=0;
	lcd_cmd(0x80);
	lcd_string("TEMPERATURE");

 	while(1)
	{	
		//f=0;
		f=ADC_read(0);
		
		//lcd_cmd(0x01);
		lcd_cmd(0xc0);
		 lcd_float(f*100);
		 
		 n=f*10;
		 //m1.byteA=n;
		if(n>=32)
		{		
		CAN_tx(m1);//data-frame
		}
		else if(n<32)
		{
		CAN_tx(m2);//data-frame		
		} 
		lcd_string("      ");
		delay(200);
		} 

}
void SPI_init(void)
{
	PINSEL0 |=0X1500;  //0X00001500
	S0SPCCR=150;  //15000000MHZ % 100000bytes(100kbps)
	S0SPCR=(1<<5|mode);
	IODIR0|=1<<7;
}
float ADC_read(unsigned char chann)
{
	unsigned char hbyte,lbyte;
	int adc;

	IOCLR0=1<<7;
	SPIO(0X06);
	hbyte=SPIO(chann<<6);
	lbyte=SPIO(0X00);
	IOSET0=1<<7;

	adc=((hbyte&0x0f)<<8)|lbyte;
	return (((adc*5.0)/4096));
}
unsigned char SPIO(unsigned char data)
{
	unsigned char status=S0SPSR;
	S0SPDR=data;
	while(((S0SPSR>>7)&1)==0);
	return S0SPDR;
}
void CAN_tx_init(void)
{
	PINSEL1 |=0X00014000;
	VPBDIV=1;
	C2MOD=0X1;
	AFMR=0X2;
	C2BTR=0X001C001D;
	C2MOD=0X0;
}
void CAN_tx(can_msg t1)
{
	C2TID1=t1.id;
	C2TFI1=(t1.dlc<<16);  //DLC=4
	if(t1.rtr==0)			//DATA FRAME
	{
		C2TFI1 &= ~(1<<30);	 //rtr =0;
		C2TDA1=t1.byteA;
		C2TDB1=t1.byteB;
	}
	else
	{
		C2TFI1 |=(1<<30);  //REMOTE FRAME
	}
	C2CMR=((1<<0)|(1<<5));
	while((C2GSR&(1<<3)==0));
}

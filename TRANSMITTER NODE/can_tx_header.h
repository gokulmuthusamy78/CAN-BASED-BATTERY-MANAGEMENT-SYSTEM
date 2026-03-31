#include<lpc21xx.h>
#include<stdio.h>
#include "lcd_4bit.h"
#define mode 0X18
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct CAN
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}can_msg;
void CAN_tx_init(void);
void CAN_tx(can_msg );
void CAN_rx(can_msg *);
void UART0_init(void);
void UART_tx(u32 );
u8 UART0_rx(void);
void UART0_tx_str(char *);
void UART0_tx_hex(int );
void SPI_init(void);
float ADC_read(unsigned char );
unsigned char SPIO(unsigned char );

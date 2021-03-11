#include <xc.h>
#include <stdio.h>
#include "USART.h"


//---------------[ UART Routines ]------------------
//--------------------------------------------------

void UART_TX_Init(void)
{
  BRGH  = 0;   // Set For LOW-Speed Baud Rate
  SPBRG = 12;  // Set The Baud Rate To Be 9600 bps
  //--[ Enable The Ascynchronous Serial Port ]--
  SYNC = 0;
  SPEN = 1;
  //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
  TX_D = 1;
  RX_D = 1;
  TXEN = 1;  // Enable UART Transmission
}

void UART_Write(unsigned char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_String(char* buf)
{
    int i=0;
    while(buf[i] != '\0')
        UART_Write(buf[i++]);
}

int Read_USART(){
  while(!RCIF);
  return RCREG;
} 
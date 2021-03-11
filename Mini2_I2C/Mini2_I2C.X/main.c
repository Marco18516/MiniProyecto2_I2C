//Basado y editado de https://deepbluembedded.com/mpu6050-with-microchip-pic-accelerometer-gyroscope-interfacing-with-pic/
//******************************************************************************
// Palabra de configuración
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
// Importación de librerías
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include "USART.h"
#include "MPU.h"

//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 8000000

int LED;
void INTERRUP();

//******************************************************************************
// Ciclo principal
//******************************************************************************
void __interrupt() ISR(void) {
    if (PIR1bits.RCIF == 1) {
        PIR1bits.RCIF = 0; //Resetea bandera RCIF
        LED = RCREG;       //Se lee el registro y se guarda
    }
    if (LED == 0X0A) {      //Dependiendo del dato que entra en RX enciende o apaga las leds
        PORTDbits.RD0 == 1;          //depende de los botones en el IOT cloud
    } else if (LED == 0X0B) {
        PORTDbits.RD0 == 0;
    } else if (LED == 0X0C) {
        PORTDbits.RD1 == 1;
    } else if (LED == 0X0D) {
        PORTDbits.RD1 == 0;
    }
    return;
}

void main(void){
    TRISB = 0;     // Leds conectadas para demostrar la variable ay
    TRISD = 0;
    ANSELH = 0;
    INTERRUP();
    UART_TX_Init();
    
    //Se inica la configuracion del sensor
    MPU6050_Init();
    //**************************************************************************
    // Loop principal
    //**************************************************************************
    while(1)
	{
        RD2 = ~RD2;  // Blink LED verde
        //Se leen los valores del sensor
        MPU6050_Read();
        __delay_ms(50);
        
    }
    
    return;
}

void INTERRUP(void){
    //ENCENDEMOS INTERRUPCIONES
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0; //No se habilitan interrupciones en el envio
    PIR1bits.RCIF = 0; //Se apaga la interrupcion

    INTCONbits.GIE = 1; //Interrupciones del timer
    return;
}
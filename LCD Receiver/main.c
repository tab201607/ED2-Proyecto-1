/*
 * Este dispositivo recibe senales de otros pic y desplega datos
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define _XTAL_FREQ 8000000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#include "LCD.h"
#include "I2C.h"
#include "adclib.h"
#include "uart.h"
#include "dht11.h"

int bpm;
//char bpm[10];
char alcohol;
char check, humid, temp;

//float humid;
//float temperature;

//el interrupt revisa que se recibio del pic primario y desplega el valor que corresponda
void __interrupt() isr(void){
  //strcpy(bpm, UARTReadString(2));
    bpm = UART_read_char();
  PIR1bits.RCIF = 0;
  __delay_ms(10);
  
}

void main(void) {
    
     I2C_Master_Init(100000);
    
    OSCCONbits.SCS = 0b00;      // Oscillator mode defined by FOSC<2:0>
    OSCCONbits.IRCF = 0b111;    // Internal RC freq to 8 MHz
    
    //activar uart
    UART_RX_config(207);
    UART_TX_config(207);
    
        PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //I2C_Slave_Init(0x50); 
    __delay_ms(1000);
    
    dht11_config(); //Configura termoemtro
    
    adc_setup(0);
    adc_channel(0);
    
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    PORTC = 0;
  
  //inicializa LCD y puerto asociado
  TRISD = 0x00;
  PORTD = 0x00;
  __delay_ms(500);
  Lcd_Init();
  __delay_ms(100);
  
  __delay_ms(3000);
  
  Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("BPM:");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("Alcohol:");
    Lcd_Set_Cursor(1,10);
    Lcd_Write_String("T:");
  
  while(1)
  {
      __delay_ms(500);
      alcohol = adc_RO(0);
      
      char display_alc[10];
      char display_bpm[10];
      char display_temp[10];
      //char display_temp[30];
      
      dht11_read (&humid, &temp);
      
      char temperature = round(temp);
      
      sprintf(display_bpm, "%3u", bpm);
      sprintf(display_alc, "%3u", alcohol);
      sprintf(display_temp, "%3u", temperature);
      Lcd_Set_Cursor(2,8);
      Lcd_Write_String(display_alc);
      Lcd_Set_Cursor(1,5);
      Lcd_Write_String(display_bpm);
       Lcd_Set_Cursor(1,12);
      Lcd_Write_String(display_temp);
      
     
      
       UART_write_char(alcohol);
      
      /*
      I2C_Master_Start();
        I2C_Master_Write(0x11);
        temp = I2C_Master_Read(0);
        __delay_ms(10);
        I2C_Master_Stop();
        __delay_ms(200);*/
      
  }
    return;
}


/*
 * File:   main_M.c
 * Author: byron
 *
 * Created on February 9, 2023, 11:13 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits 
                                            // (INTOSC oscillator: I/O function 
                                            // on RA6/OSC2/CLKOUT pin, I/O 
                                            // function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
                                // can be enabled by SWDTEN bit of the WDTCON 
                                // register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                // pin function is digital input, MCLR 
                                // internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                // protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                // protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                // (Internal/External Switchover mode disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                // (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
                                // has digital I/O, HV on MCLR must be used for 
                                // programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                // Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                // (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "configINTOSC.h"
#include "configI2C.h"
#include "configLCD_4bits.h"
#include "DS3231.h"

#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

void setup(void);

unsigned char modo = 0;

void __interrupt() ISR (void) {
    
    if (INTCONbits.RBIF == 1) // Interrupción del PORTB
    {
        if(!PORTBbits.RB0)
        {
            modo++;
            
            if(modo == 6)
            {
            modo = 0;
            }
        }
        INTCONbits.RBIF = 0; // Bajamos la bandera de interrupción del PORTB
    }
    return;
}

void main(void) {
  
    setup();
    
    uint8_t VAL1, VAL1_U, VAL1_D, VAL1_C, SEG, MIN, HOR, DIA, MES, AO;

    char ADC1[9];
    char tiempo[8] = {"00:00:00"};
    char fecha[10] = {"01/01/2000"};
    
    Lcd_Clear_4bits();
    Lcd_Set_Cursor_4bits(1,1);
    Lcd_Write_String_4bits("S1:");

    while(1)
    {   
        
        PORTA = modo;
        
        I2C_Master_Start();
        I2C_Master_Write(0x11);
        VAL1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        VAL1_C = (VAL1/100);
        VAL1_D = (VAL1/10)%10;
        VAL1_U = VAL1%10;
        
        sprintf(ADC1, "%u", VAL1_C);
        Lcd_Set_Cursor_4bits(2,1);
        Lcd_Write_String_4bits(ADC1);
        
        sprintf(ADC1, "%u", VAL1_D);
        Lcd_Set_Cursor_4bits(2,2);
        Lcd_Write_String_4bits(ADC1);
        
        sprintf(ADC1, "%u", VAL1_U);
        Lcd_Set_Cursor_4bits(2,3);
        Lcd_Write_String_4bits(ADC1);
        
        SEG = leerRTC(REG_SEG);
        
        MIN = leerRTC(REG_MIN);
        
        HOR = leerRTC(REG_HOR);
        
        DIA = leerRTC(REG_DIA);
        
        MES = leerRTC(REG_MES);
        
        AO = leerRTC(REG_A);
        
        tiempo[7] = SEG % 10  + 48;
        tiempo[6] = SEG / 10  + 48;
        tiempo[4] = MIN % 10  + 48;
        tiempo[3] = MIN / 10  + 48;
        tiempo[1] = HOR % 10  + 48;
        tiempo[0] = HOR / 10  + 48;
        fecha[9] = AO % 10 + 48;
        fecha[8] = AO / 10  + 48;
        fecha[4]  = MES % 10 + 48;
        fecha[3]  = MES / 10 + 48;
        fecha[1]  = DIA % 10 + 48;
        fecha[0]  = DIA / 10 + 48;
        
        Lcd_Set_Cursor_4bits(1,7);
        Lcd_Write_String_4bits(tiempo);
        Lcd_Set_Cursor_4bits(2,7);
        Lcd_Write_String_4bits(fecha);
        
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************

void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISA = 0;              //Configuración del PORTA como output
    TRISB = 0b00000111;     //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output
    
    OPTION_REGbits.nRBPU = 0;   //Habilitamos los pull-ups del PORTB
    WPUBbits.WPUB0 = 1;         //Habilitamos el pull-up del RB0
    WPUBbits.WPUB1 = 1;         //Habilitamos el pull-up del RB1
    WPUBbits.WPUB2 = 1;         //Habilitamos el pull-up del RB2
    IOCB = 0b00000111;

    PORTA = 0;              //Limpiamos el PORTA
    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD

    I2C_Master_Init(100000);    // Inicializar Comuncación I2C
    setupINTOSC(FOSC_8MHZ);     //Configuramos el oscilador interno a 4MHZ
    Lcd_Init_4bits();           //Inicializamos la LCD en modo de 4 bits
    
    INTCONbits.GIE = 1;     //Habilitamos las interrupciones globales (GIE)
    INTCONbits.PEIE = 1;    //Habilitamos las interrupción del PEIE
    INTCONbits.RBIE = 1;    //Habilitamos las interrupciones del PORTB (RBIE)
    INTCONbits.RBIF = 0;    //Bajamos la bandera de interrupción del PORTB (RBIF)
}

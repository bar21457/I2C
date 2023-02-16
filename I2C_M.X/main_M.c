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

void main(void) {
  
    setup();
    
    uint8_t VAL1, VAL1_U, VAL1_D, VAL1_C;
    uint8_t SEG, SEG_U, SEG_D;
    uint8_t MIN, MIN_U, MIN_D;
    uint8_t HOR, HOR_U, HOR_D;
    uint8_t DIA, DIA_U, DIA_D;
    uint8_t MES, MES_U, MES_D;
    uint8_t AO, AO_U, AO_D, AO_C, AO_UM;

    char ADC1[9];
    char stringSEG[9];
    char stringMIN[9];
    char stringHOR[9];
    char stringDIA[9];
    char stringMES[9];
    char stringAO[9];
    
    Lcd_Clear_4bits();
    Lcd_Set_Cursor_4bits(1,1);
    Lcd_Write_String_4bits("S1:");
    Lcd_Set_Cursor_4bits(1,9);
    Lcd_Write_String_4bits(":");
    Lcd_Set_Cursor_4bits(1,12);
    Lcd_Write_String_4bits(":");
    Lcd_Set_Cursor_4bits(2,9);
    Lcd_Write_String_4bits("/");
    Lcd_Set_Cursor_4bits(2,12);
    Lcd_Write_String_4bits("/");

    while(1)
    {   
        
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
        
        SEG_D = (SEG/10)%10;
        SEG_U = SEG%10;
        
        sprintf(stringSEG, "%u", SEG_D);
        Lcd_Set_Cursor_4bits(1,13);
        Lcd_Write_String_4bits(stringSEG);
        
        sprintf(stringSEG, "%u", SEG_U);
        Lcd_Set_Cursor_4bits(1,14);
        Lcd_Write_String_4bits(stringSEG);
        
        MIN = leerRTC(REG_MIN);
        
        MIN_D = (MIN/10)%10;
        MIN_U = MIN%10;
        
        sprintf(stringMIN, "%u", MIN_D);
        Lcd_Set_Cursor_4bits(1,10);
        Lcd_Write_String_4bits(stringMIN);
        
        sprintf(stringMIN, "%u", MIN_U);
        Lcd_Set_Cursor_4bits(1,11);
        Lcd_Write_String_4bits(stringMIN);
        
        HOR = leerRTC(REG_HOR);
        
        HOR_D = (HOR/10)%10;
        HOR_U = HOR%10;
        
        sprintf(stringHOR, "%u", HOR_D);
        Lcd_Set_Cursor_4bits(1,7);
        Lcd_Write_String_4bits(stringHOR);
        
        sprintf(stringHOR, "%u", HOR_U);
        Lcd_Set_Cursor_4bits(1,8);
        Lcd_Write_String_4bits(stringHOR);
        
        DIA = leerRTC(REG_DIA);
        
        DIA_D = (DIA/10)%10;
        DIA_U = DIA%10;
        
        sprintf(stringDIA, "%u", DIA_D);
        Lcd_Set_Cursor_4bits(2,7);
        Lcd_Write_String_4bits(stringDIA);
        
        sprintf(stringDIA, "%u", DIA_U);
        Lcd_Set_Cursor_4bits(2,8);
        Lcd_Write_String_4bits(stringDIA);
        
        MES = leerRTC(REG_MES);
        
        MES_D = (MES/10)%10;
        MES_U = MES%10;
        
        sprintf(stringMES, "%u", MES_D);
        Lcd_Set_Cursor_4bits(2,10);
        Lcd_Write_String_4bits(stringMES);
        
        sprintf(stringMES, "%u", MES_U);
        Lcd_Set_Cursor_4bits(2,11);
        Lcd_Write_String_4bits(stringMES);
        
        AO = leerRTC(REG_A);
        
        AO_UM = 2;
        AO_C = ((AO/10)/10)%10;
        AO_D = (AO/10)%10;
        AO_U = AO%10;
        
        sprintf(stringAO, "%u", AO_UM);
        Lcd_Set_Cursor_4bits(2,13);
        Lcd_Write_String_4bits(stringAO);
        
        sprintf(stringAO, "%u", AO_C);
        Lcd_Set_Cursor_4bits(2,14);
        Lcd_Write_String_4bits(stringAO);
        
        sprintf(stringAO, "%u", AO_D);
        Lcd_Set_Cursor_4bits(2,15);
        Lcd_Write_String_4bits(stringAO);
        
        sprintf(stringAO, "%u", AO_U);
        Lcd_Set_Cursor_4bits(2,16);
        Lcd_Write_String_4bits(stringAO);
    }
    return;
}

//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************

void setup (void){
    
    ANSEL = 0;
    ANSELH = 0;

    TRISB = 0;              //Configuración del PORTB como input
    TRISD = 0;              //Configuración del PORTD como output

    PORTB = 0;              //Limpiamos el PORTB
    PORTD = 0;              //Limpiamos el PORTD

    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    setupINTOSC(FOSC_8MHZ);     //Configuramos el oscilador interno a 4MHZ
    Lcd_Init_4bits();           //Inicializamos la LCD en modo de 4 bits
    
}

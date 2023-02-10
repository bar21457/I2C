/*
 * File:   config_adc.c
 * Author: byron
 *
 * Created on January 19, 2023, 10:28 PM
 */

#include <xc.h>
#include "configADC.h"

#define _XTAL_FREQ 4000000

void setupADC (uint8_t canal){
    
    //Paso 1: Selección del puerto de entrada
    
    if(canal == 0)
    {
    TRISAbits.TRISA0 = 1;       //Configuración del RBA0 como input
    ANSELbits.ANS0 = 1;         //Configuración del pin RBA0 como análogo (AN0)
    }
    
    if(canal == 1)
    {
    TRISAbits.TRISA1 = 1;       //Configuración del RBA1 como input
    ANSELbits.ANS1 = 1;         //Configuración del pin RBA1 como análogo (AN1)
    }
    
    //Paso 2: Configuración del módulo ADC
    
    ADCON0bits.ADCS0 = 1;
    ADCON0bits.ADCS1 = 0;       //Fosc/8
    
    ADCON1bits.VCFG0 = 0;       //Ref VDD
    ADCON1bits.VCFG1 = 0;       //Ref VSS
    
    ADCON1bits.ADFM = 0;        //Justificado hacia la izquierda
    
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;        //Selección del canal análogo AN0
    
    ADCON0bits.ADON = 1;        //Habilitamos el ADC
    
    __delay_us(100);            //Delay para adquirir la lectura
}

void readADC (uint8_t canal){
    
    if(canal == 0)
    {
    ADCON0bits.CHS = 0b0000;    // Usamos el AN0

    __delay_us(100);
      
    ADCON0bits.GO = 1;      //Iniciamos la conversión en el ADC
    while (ADCON0bits.GO == 1){};
    ADIF = 0;               //Bajamos la bandera del ADC
    __delay_ms(10);
    }
    
    if(canal == 1)
    {
    ADCON0bits.CHS = 0b0001;    // Usamos el AN1

    __delay_us(100);
      
    ADCON0bits.GO = 1;      //Iniciamos la conversión en el ADC
    while (ADCON0bits.GO == 1){};
    ADIF = 0;               //Bajamos la bandera del ADC
    __delay_ms(10);
    }
    
}
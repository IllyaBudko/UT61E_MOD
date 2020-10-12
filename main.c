/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdio.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include <pic16f688.h>

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

//PORTA defines
//Inputs
#define DCAC        RA0
#define VHZ         RA1
#define BKOUT       RA2
//skipped MCLR

//Outputs
#define DCAC_HOOK   RA4
#define VHZ_HOOK    RA5

//PORTC defines
#define BKLIT       RC0
#define RS232       RC1
#define LPF         RC2
#define SLADC       RC3
#define BKLED       RC4
#define MAXMIN      RC5

void main(void)
{
    ANSEL = 0x00; //AN pins set as digital i/o
    CMCON0 = 0x07;//comparators are of, CxIN pins as digital i/o
    
    TRISA = 0x07; //RA0,1,2 nputs. others outputs
    PORTA = 0xF8; //PORTA outputs set to mcu logic high
    OPTION_REGbits.nRAPU = 0; //Enable PORTA individual internal weak pull up resistors
    WPUA  = 0x07; //RA0,1,2 enable weak internal pull ups
    
    TRISC = 0x00; //all outputs
    PORTC = 0xFF; // PORTC outputs set to mcu logic high
    
    //MCU logic level 1(high) is DMM logic level 0(low) and vice versa
    
    
    
    //check VHZ input on reset to set
    __delay_ms(100);
    
    
}


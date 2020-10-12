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
#define DCAC_HOOK   RA4 //Initial state mcu 1, dmm 0
#define VHZ_HOOK    RA5 //Initial state mcu 1, dmm 0

//PORTC defines
#define BKLIT       RC0 //Initial state mcu 1, dmm 0
#define RS232       RC1 //Initial state mcu 1, dmm 0
#define LPF         RC2 //Initial state mcu 1, dmm 0
#define SLADC       RC3 //Initial state mcu 0, dmm 1
#define BKLED       RC4 //Initial state mcu 0, dmm 1
#define MAXMIN      RC5 //Initial state mcu 1, dmm 0

#define _XTAL_FREQ 4000000UL

    //MCU logic level 1(high) is DMM logic level 0(low) and vice versa

void main(void)
{
    //Initial register setup
    ANSEL = 0x00; //AN pins set as digital IOs
    CMCON0 = 0x07;//Comparators are off, CxIN pins as digital IOs
    
    //PORTA setup
    TRISA = 0x07; //RA0,1,2 inputs. others outputs
    PORTA = 0x30; //PORTA outputs set to MCU logic high
    OPTION_REGbits.nRAPU = 0; //Enable PORTA individual internal weak pull up resistors
    WPUA  = 0x07; //RA0,1,2 enable weak internal pull ups
    
    //PORTC setup
    TRISC = 0x00; //all outputs
    PORTC = 0x67; // PORTC outputs set to MCU logic high
    
    //TIMER1 setup
    TMR1L = 0x00;
    TMR1H = 0x00;
    T1CON = 0x30; // set pre-scaler to 1:8 for 0.52 sec overflow
      
    //check VHZ input on reset
    if(!VHZ)
    {
        __delay_ms(100); //De-bounce
        T1CONbits.TMR1ON = 1;
        while(!VHZ);
        //If timer interrupt flag sets before VHZ button is de-pressed
        //This will turn on LPF feature of DMM chip
        if(TMR1IF)
        {
            PORTCbits.LPF = 0;
            PORTCbits.SLADC = 1;
            
            T1CONbits.TMR1ON = 0;
            TMR1IF = 0;
            //This is the loop for when the LPF feature is turned on
            //you can choose which measurement setting is used on the DMM
            while(1)
            {
                if(!DCAC)
                {
                    __delay_ms(100); //De-bounce
                    while(!DCAC);
                    PORTAbits.DCAC_HOOK = 0;
                    __delay_ms(50);
                    PORTAbits.DCAC_HOOK = 1;

                }
            }
        }
        T1CONbits.TMR1ON = 0;
        TMR1L = 0x00;
        TMR1H = 0x00;
        TMR1IF = 0;
    }
    
    //Main loop for added features when the LPF feature is not activated
    while(1)
    {
        //Long press on both buttons pressed will toggle RS232 feature 
        //Short press reserved for future use
        __delay_ms(150); //Button press catch
        if((!DCAC) && (!VHZ))
        {
            __delay_ms(50); //Button de-bounce
            T1CONbits.TMR1ON = 1;
            while(!(DCAC && VHZ));
            if(TMR1IF)
            {
                if(RS232)
                {
                    PORTCbits.RS232 = 0;
                }
                else
                {
                    PORTCbits.RS232 = 1;
                }
            }
            else
            {
                //For future purpose
                NOP();
            }
            T1CONbits.TMR1ON = 0;
            TMR1L = 0x00;
            TMR1H = 0x00;
            TMR1IF = 0;            
        }
        //Long press DCAC will enable MAXMIN feature
        //Short press will cycle through measurement mode select
        else if(!DCAC)
        {
            __delay_ms(50); // Button de-bounce
            T1CONbits.TMR1ON = 1;
            while(!DCAC);
            if(TMR1IF)
            {
                PORTCbits.MAXMIN = 0;
                __delay_ms(50);
                PORTCbits.MAXMIN = 1;
            }
            else
            {
                PORTAbits.DCAC_HOOK = 0;
                __delay_ms(50);
                PORTAbits.DCAC_HOOK = 1;
            }
            T1CONbits.TMR1ON = 0;
            TMR1L = 0x00;
            TMR1H = 0x00;
            TMR1IF = 0;
        }
        //Long press VHZ will turn on back light for display
        //Short press will cycle frequency measurement
        else if(!VHZ)
        {
            __delay_ms(50); //Button de-bounce
            T1CONbits.TMR1ON = 1;
            while(!VHZ);
            if(TMR1IF)
            {
                PORTCbits.BKLIT = 0;
                __delay_ms(50);
                PORTCbits.BKLIT = 1;
            }
            else
            {
                PORTAbits.VHZ_HOOK = 0;
                __delay_ms(50);
                PORTAbits.VHZ_HOOK = 1;
            }
            T1CONbits.TMR1ON = 0;
            TMR1L = 0x00;
            TMR1H = 0x00;
            TMR1IF = 0;
        }
        else
        {
            //Nothing happens when the buttons are not pressed
            NOP();
        }
        
        //Mirror BKOUT input to BKLED output
        if(BKOUT)
        {
            PORTCbits.BKLED = 1;
        }
        else
        {
            PORTCbits.BKLED = 0;
        }
    }   
}





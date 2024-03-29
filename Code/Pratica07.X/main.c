/*
 * File:   main.c
 * Author: Davi Marriel
 *
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>

#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HS
#pragma config PWRT = ON
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = OFF
#pragma config LVP = OFF
#pragma config XINST=OFF
 
#define _XTAL_FREQ 4000000
#define DISP3 LATAbits.LA2
#define DISP4 LATAbits.LA5
#define DISP2 LATEbits.LE0
#define DISP1 LATEbits.LE2
#define DIGIT(x) LATD = numbers[x]

char numbers[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
 		    0x77,0x7C,0x39,0x5E,0x79,0x71};

char tmrCounter = 0;
char unitCounter = 0;
char decCounter = 0;
char minCounter = 0;

void setDisplay (char dispNum){
    switch(dispNum){
        case 1:
            DISP1 = 1;
            DISP2 = 0;
            DISP3 = 0;
            DISP4 = 0;
            DIGIT(unitCounter);
            break;
        case 2:
            DISP1 = 0;
            DISP2 = 1;
            DISP3 = 0;
            DISP4 = 0;
            DIGIT(decCounter);
            break;
        case 3:
            DISP1 = 0;
            DISP2 = 0;
            DISP3 = 1;
            DISP4 = 0;
            DIGIT(minCounter);
            break;
        case 4:
            DISP1 = 0;
            DISP2 = 0;
            DISP3 = 0;
            DISP4 = 1;
            DIGIT(0);
            break;
        default:
            break;
    }
}


void setup(){
  	ADCON1 = 0x0F;
	//Configura��o I/Os
	PORTD = 0x00;
	LATD = 0x00;

	TRISA = 0x00;
	TRISC = 0x00;
	TRISE = 0x00;

	PORTAbits.RA2 = 0;
	PORTAbits.RA5 = 0;
	PORTCbits.RC2 = 0;
	PORTEbits.RE0 = 0;
	PORTEbits.RE2 = 0;

	LATAbits.LA2 = 0;
	LATAbits.LA5 = 0;
	LATCbits.LC2 = 0;
	LATEbits.LE0 = 0;
	LATEbits.LE2 = 0;

	//Configuração do Timer
	//PSC 1:2
	T0CONbits.T0PS0 = 1;
	T0CONbits.T0PS1 = 0
	T0CONbits.T0PS2 = 0;
		
	//PSA
	T0CONbits.PSA = 0;

	//FONTE DE CLOCK - Interna
	T0CONbits.T0CS = 0;

	//16bit
	T0CONbits.T08BIT = 0;

	//TMR0H:TMR0L
	TMR0H = 0xFF;
	TMR0L = 0x05;

	//Configuração das Interrupções
	INTCONbits.GIE = 1;
	INTCONbits.TMR0IE = 1;
	INTCONbits.TMR0IF = 0;
	}
void interrupt intTimer(void){
    static char dispToggle = 1;
	if(INTCONbits.TMR0IF == 1){
		INTCONbits.TMR0IF = 0;
		++tmrCounter;
        if(tmrCounter % 16 == 0){
            setDisplay(dispToggle++);
            if(dispToggle == 5){
                dispToggle = 1;
            }
        }
        if(tmrCounter == 1000){
            ++unitCounter;
            if(unitCounter == 10){
                unitCounter = 0;
                ++decCounter;
                if(decCounter == 6){
                    decCounter = 0;
                    ++minCounter;
                    if(minCounter = 9){
                        minCounter = 0;
                    }
                }
            }
            tmrCounter = 0;
        }
        
		TMR0H = 0xFF;
		TMR0L = 0x05;
		}
	}

void main(){
	setup();
	while(1){
	}
}


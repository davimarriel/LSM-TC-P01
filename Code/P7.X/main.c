/*
 * File:   main.c
 * Author: davij
 *
 * Created on 16 de Fevereiro de 2022, 00:07
 */


#include <xc.h>

#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config FOSC = HS
#pragma config PWRT = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDT = OFF         // Watchdog Timer Enable bit (WDT enabled)
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))



#define _XTAL_FREQ 4000000
#define DISP3 LATAbits.LA2
#define DISP4 LATAbits.LA5
#define DISP2 LATEbits.LE0
#define DISP1 LATEbits.LE2
#define DIGIT(x) LATD = numbers[x]

char numbers[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
 		    0x77,0x7C,0x39,0x5E,0x79,0x71};

unsigned short tmrCounter1 = 0;
unsigned short tmrCounter2 = 0;
unsigned short tmrCounter3 = 0;
short toggleCounter = 0;
char unitCounter = 0;
char decCounter = 0;
char minCounter = 0;
char dispToggle = 1;

void setDisplay (char dispNum){
    switch(dispNum){
        case 1:
            DISP1 = 0;
            DISP2 = 1;
            DISP3 = 1;
            DISP4 = 1;
//            DIGIT(unitCounter);
            break;
        case 2:
            DISP1 = 1;
            DISP2 = 0;
            DISP3 = 1;
            DISP4 = 1;
//            DIGIT(decCounter);
            break;
        case 3:
            DISP1 = 1;
            DISP2 = 1;
            DISP3 = 0;
            DISP4 = 1;
//            DIGIT(minCounter);
            break;
        case 4:
            DISP1 = 1;
            DISP2 = 1;
            DISP3 = 1;
            DISP4 = 0;
//            DIGIT(0);
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
	TRISD = 0x00;
	TRISE = 0x00;

	PORTAbits.RA2 = 0;
	PORTAbits.RA5 = 0;
	PORTEbits.RE0 = 0;
	PORTEbits.RE2 = 0;

	LATAbits.LA2 = 0;
	LATAbits.LA5 = 0;
	LATEbits.LE0 = 0;
	LATEbits.LE2 = 0;

	//Configuração do Timer
	//PSC 1:2
	T0CONbits.T0PS0 = 1;
	T0CONbits.T0PS1 = 0;
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

void __interrupt() intTimer(void){
	if(INTCONbits.TMR0IF == 1){
		INTCONbits.TMR0IF = 0;
		++tmrCounter1;
        ++tmrCounter2;
        ++tmrCounter3;
        ++toggleCounter;
        if(toggleCounter == 75){
            setDisplay(dispToggle++);
            if(dispToggle == 5){
                dispToggle = 1;
            }
            toggleCounter = 0;
        }
        if(tmrCounter1 == 1000){
            ++unitCounter;
            if(unitCounter == 10){
                unitCounter = 0;              
            }
            tmrCounter1 = 0;
        }
        if(tmrCounter2 == 10000){
            ++decCounter;
            if(decCounter == 6){
                decCounter = 0;              
            }
            tmrCounter2 = 0;
        }
        if(tmrCounter3 == 60000){
            ++minCounter;
            if(minCounter == 10){
                minCounter = 0;              
            }
            tmrCounter3 = 0;
        }
        
		TMR0H = 0xFF;
		TMR0L = 0x05;
		}
	}

void main(){
	setup();
	while(1){
        if(dispToggle == 1){
            DIGIT(unitCounter);
        }else if(dispToggle == 2){
            DIGIT(0);
        }else if(dispToggle == 3){
            DIGIT(minCounter);
        }else if(dispToggle == 4){
            DIGIT(decCounter);
        }
	}
}

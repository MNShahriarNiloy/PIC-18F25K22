#pragma config LVP = OFF  
#pragma config PLLCFG = OFF     
#pragma config PRICLKEN = ON    
#pragma config FCMEN = OFF    
#pragma config IESO = OFF     
#pragma config WDTEN = OFF      
#pragma config BOREN = OFF      
#pragma config FOSC = INTIO67
#define _XTAL_FREQ 8000000UL

#include <xc.h>
#include <stdio.h>

#define RS LATBbits.LATB0
#define EN LATBbits.LATB1
#define D4 LATBbits.LATB4
#define D5 LATBbits.LATB5
#define D6 LATBbits.LATB6
#define D7 LATBbits.LATB7

void Lcd_Init(void);
void Lcd_Cmd(unsigned char cmd);
void Lcd_Char(char data);
void Lcd_String(const char *str);
void Lcd_Set_Cursor(unsigned char row, unsigned char column);
void Lcd_Enable(void);

void main(void){
    OSCCONbits.IRCF = 0b110;   
    OSCCONbits.SCS = 0b10;  
    
    while (!OSCCONbits.HFIOFS);
    TRISB = 0x00; 
    LATB = 0x00;
    
    Lcd_Init();
    Lcd_Set_Cursor(1, 1);
        Lcd_String("HELLO");
        char buf1[5];
        sprintf(buf1,"THERE!");
}
void Lcd_Enable(void) {
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(50);  
}

void Lcd_Cmd(unsigned char cmd) {
    RS = 0;
    
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    Lcd_Enable();

    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    Lcd_Enable();
}

void Lcd_Char(char data) {
    RS = 1;
    
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    Lcd_Enable();

    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    Lcd_Enable();
}

void Lcd_String(const char *str) {
    while (*str) {
        Lcd_Char(*str++);
    }
}

void Lcd_Set_Cursor(unsigned char row, unsigned char column) {
    unsigned char pos;
    if (row == 1)
        pos = 0x80 + (column - 1);
    else
        pos = 0xC0 + (column - 1);

    Lcd_Cmd(pos);
}

void Lcd_Init(void) {
    __delay_ms(15); 
    Lcd_Cmd(0x02);
    Lcd_Cmd(0x28); 
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x06);
    Lcd_Cmd(0x01);
    __delay_ms(2);
}

// PIC16F877A Configuration Bit Settings

// CONFIG
#pragma config FOSC = EXTRC     
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = OFF      
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

#include <xc.h>
#define _XTAL_FREQ 6000000

void uart_init();
unsigned char value;

void main() {
    uart_init();

    TRISD = 0x00;//Set PORTD as output
    PORTD = 0x00;// Clear PORTD

    while(1) {
        value = PORTB;

        switch(value) {
            case 0xE0:
                TXREG = 'A';
                __delay_ms(100);
                break;
            case 0xD0:
                TXREG = 'B';
                __delay_ms(100);
                break;
            case 0xB0:
                TXREG = 'C';
                __delay_ms(100);
                break;
            case 0x70:
                TXREG = 'D';
                __delay_ms(100);
                break;
        }

        // Receive data 
        if (RCIF) {
            char received = RCREG;  
            switch(received) {
                case 'a':  
                    PORTD = 0x02;
                    break;
                case 'b':  
                    PORTD = 0x40;
                    break;
                case 'c':  
                    PORTD =0x00;
                    break;
                case 'd': 
                    PORTD = 0x42;
                    break;
            }

            __delay_ms(200); 
            PORTD = 0x00;
        }
    }
}

void uart_init() {
    TRISB = 0xF0; 
    PORTB = 0x00;
    OPTION_REG &= 0x7F;

    // UART initialize
    TRISC = 0xC0;      // RC6 and RC7 as input (TX, RX)
    TXSTA = 0x20;      // Enable transmitter, async mode
    SPBRG = 0x07;      // Baud rate setting (adjust as needed)
    RCSTA = 0x90;      // Enable receiver and serial port
}

#include <xc.h>
#define _XTAL_FREQ 6000000

void timer_1_init(void);
unsigned char count;

void main() {
    timer_1_init();
    while(1) {
        PORTC = 0xFF;
        __delay_ms(500);
        PORTC = 0x00;
        __delay_ms(500);
        
    }
}

void timer_1_init(void) {
    TRISC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    PORTC = 0x00;
    
    // timer1 register configuration 
    T1CON = 0x01;
    
    // timer 1 interrupt config
    INTCON |= (3 << 6);     // Enable GIE and PEIE
    PIE1 |= (1 << 0);       // TMR1IE enable 
    PIR1 &= ~(1 << 0);      // clear TMR1IF flag
}

void __interrupt() ISR(void) {
    if(PIR1 & 0x01) 
    {
        count++;
        if(PIR1 & 0x01)
        {
        PORTD = ~PORTD;
        count = 0;
        }
        PIR1 &= ~(1 << 0);
    } 
}

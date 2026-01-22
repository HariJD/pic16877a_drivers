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

void init();
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_string(const char*);
void number_convert_3(unsigned int);
void number_convert_4(unsigned int);
unsigned int adc_value,scaled,adc_low,adc_high;

void main() {
    init();

    while (1) {
        ADCON0 = 0x89;//for AN1 pin
        __delay_ms(10);
        ADCON0 |= 0x04; 
        while (ADCON0 & 0x04);
        adc_high = ADRESH;
        adc_low =ADRESL;
        adc_value = (adc_high << 8) + adc_low;
        
        lcd_command(0x80);  
        lcd_string("CH1: ");  
        number_convert_4(adc_value);  
        lcd_command(0x88);
        lcd_data(0x76); 

        ADCON0 = 0xA1;// for AN4 pin
        __delay_ms(10);
        ADCON0 |= 0x04;
        while (ADCON0 & 0x04);
        adc_value = (adc_high << 8) + adc_low;
        float voltage = (adc_value *5) / 1023;// float the input voltage value 
        scaled = (voltage /5) *135;

        lcd_command(0xC0);  
        lcd_string("CH4: ");  
        number_convert_3(scaled);  
        lcd_command(0xC7);
        lcd_data(0x76); 

        __delay_ms(200);
    }
}
void init() {
    TRISC = 0x00;  
    TRISD = 0x00;  
    PORTC = 0x00;  
    PORTD = 0x00;  
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
    lcd_command(0x30);
    __delay_ms(100);
   lcd_command(0x38); 
    __delay_ms(100);
    lcd_command(0x0C); 
    __delay_ms(100);
    lcd_command(0x06); 
    __delay_ms(100);
    lcd_command(0x01); 
    __delay_ms(100);
    ADCON1 = 0x82;// AN1-AN4 analog pin
    ADCON0 = 0x81; 
}
void lcd_command(unsigned char a) {
    PORTC &= ~0x08;  
    PORTD = a;
    PORTC |= 0x01;   
    __delay_ms(1);
    PORTC &= ~0x01;  
    __delay_ms(2);
}
void lcd_data(unsigned char b) {
    PORTC |= 0x08;   
    PORTD = b;
    PORTC |= 0x01;   
    __delay_ms(1);
    PORTC &= ~0x01;  
    __delay_ms(2);
}
void lcd_string(const char *str) {
    while(*str) {
        lcd_data(*str);  
        str++;            
    }
}
void number_convert_3(unsigned int i) {
    unsigned char digits[3];  
    digits[0] = i / 100; //hundered position            
    digits[1] = (i / 10) % 10;//ten position 
    digits[2] = i % 10;// one position                

    lcd_command(0xC4);  

    if (digits[0] > 0)
        lcd_data(0x30 + digits[0]);
    else
        lcd_data(' ');  
    lcd_command(0xC5);  
    if (digits[0] > 0 || digits[1] > 0) 
        lcd_data(0x30 + digits[1]);
    else
        lcd_data(' ');  
    lcd_command(0xC6);  
    lcd_data(0x30 + digits[2]);
}

void number_convert_4(unsigned int i) {
    unsigned char digits[4];  

    digits[0] = i /1000;              
    digits[1] = (i /100) % 10;        
    digits[2] = (i /10) % 10;         
    digits[3] = i %10;                
    lcd_command(0x84);  
    if (digits[0] >0)
        lcd_data(0x30 + digits[0]);
    else
        lcd_data(' ');  
    lcd_command(0x85);  
    if (digits[0] >0 || digits[1] > 0)
        lcd_data(0x30 + digits[1]);
    else
        lcd_data(' ');  
    lcd_command(0x86);  
    if (digits[0]> 0 || digits[1] > 0 || digits[2] > 0)
        lcd_data(0x30 + digits[2]);
    else
        lcd_data(' ');  
    lcd_command(0x87);  
    lcd_data(0x30 + digits[3]);  
}

#include <xc.h>
#define _XTAL_FREQ 20000000

// I2C & LCD Prototypes
void init(void);
void i2c_init(const unsigned long);
void i2c_write(unsigned char);
int i2c_read(int);
void i2c_start(void);
void i2c_stop(void);
void i2c_wait(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);

// Conversions
int dec_2_bcd(int);
int bcd_2_dec(int);

// RTC
void settime(void);
void update(void);

// Messages for LCD
char msg1[6] = {"TIME:"};
char msg2[6] = {"DATE:"};

// Variables
int sec, min, hour, date, month, year;
int sec1, sec2, min1, min2, hour1, hour2, date1, date2, month1, month2, year1, year2;
int i, j; // j = AM/PM flag (0 = AM, 1 = PM)

// Main
void main(void) {
    init();
    i2c_init(100);      // Initialize I2C with 100kHz
    settime();          // Set starting time/date

    __delay_ms(20);     // Wait for LCD power-up

    // LCD init
    TRISD = 0x00;
    lcd_command(0x30); __delay_ms(5);
    lcd_command(0x30); __delay_ms(5);
    lcd_command(0x30); __delay_ms(5);
    lcd_command(0x38); __delay_ms(5);
    lcd_command(0x06); __delay_ms(5);
    lcd_command(0x0C); __delay_ms(5);
    lcd_command(0x01); __delay_ms(5);

    while(1) {
        update();

        // Split into digits
        sec1 = sec / 10; sec2 = sec % 10;
        min1 = min / 10; min2 = min % 10;
        hour1 = hour / 10; hour2 = hour % 10;
        date1 = date / 10; date2 = date % 10;
        month1 = month / 10; month2 = month % 10;
        year1 = year / 10; year2 = year % 10;

        // Display TIME
        lcd_command(0x80);
        for (i = 0; i < 5; i++) lcd_data(msg1[i]);
        lcd_data(hour1 + '0');
        lcd_data(hour2 + '0');
        lcd_data(':');
        lcd_data(min1 + '0');
        lcd_data(min2 + '0');
        lcd_data(':');
        lcd_data(sec1 + '0');
        lcd_data(sec2 + '0');
        lcd_data(' ');
        lcd_data(j ? 'P' : 'A');
        lcd_data('M');

        // Display DATE
        lcd_command(0xC0);
        for (i = 0; i < 5; i++) lcd_data(msg2[i]);
        lcd_data(date1 + '0');
        lcd_data(date2 + '0');
        lcd_data('-');
        lcd_data(month1 + '0');
        lcd_data(month2 + '0');
        lcd_data('-');
        lcd_data(year1 + '0');
        lcd_data(year2 + '0');

        __delay_ms(500);
    }
}

// Init
void init(void) {
    TRISC = 0x18; // RC3=SCL, RC4=SDA as input
    TRISC0 = 0;   // EN pin output
    TRISC2 = 0;   // RS pin output
    TRISD = 0x00; // LCD data pins output
    RC0 = 0;      // EN low initially
    RC2 = 0;      // RS low initially
}

// I2C functions
void i2c_init(const unsigned long freq) {
    SSPCON = 0x28;
    SSPSTAT = 0;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ / (4 * freq * 100)) - 1;
}

void i2c_wait(void) {
    while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void i2c_start(void) {
    i2c_wait();
    SEN = 1;
}

void i2c_stop(void) {
    i2c_wait();
    PEN = 1;
}

void i2c_write(unsigned char data) {
    i2c_wait();
    SSPBUF = data;
}

int i2c_read(int ack) {
    int value;
    i2c_wait();
    RCEN = 1;
    i2c_wait();
    value = SSPBUF;
    i2c_wait();
    ACKDT = (ack) ? 0 : 1;
    ACKEN = 1;
    return value;
}

// BCD conversion
int dec_2_bcd(int temp) {
    return ((temp / 10) << 4) + (temp % 10);
}

int bcd_2_dec(int temp) {
    return ((temp >> 4) * 10) + (temp & 0x0F);
}

// Set time to 11:59:30 PM, Date = 21-07-2025 (12-hour mode)
void settime(void) {
    i2c_start();
    i2c_write(0xD0);  // RTC write address
    i2c_write(0x00);  // Start from register 0
    
    i2c_write(dec_2_bcd(30));      // Seconds
    i2c_write(dec_2_bcd(59));      // Minutes
    i2c_write(0x71 | 0x20);        // Hours: 12-hour mode (bit6=1) + PM (bit5=1) + hour=11 (0x11)
    i2c_write(1);                  // Day of week
    i2c_write(dec_2_bcd(21));      // Date
    i2c_write(dec_2_bcd(7));       // Month
    i2c_write(dec_2_bcd(25));      // Year
    i2c_stop();
}

// Read time/date from RTC
void update(void) {
    unsigned char hr_reg;

    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00);
    i2c_stop();

    i2c_start();
    i2c_write(0xD1);

    sec = bcd_2_dec(i2c_read(1));
    min = bcd_2_dec(i2c_read(1));

    hr_reg = i2c_read(1);
    if (hr_reg & 0x40) { // 12-hour mode
        hour = bcd_2_dec(hr_reg & 0x1F);
        j = (hr_reg & 0x20) ? 1 : 0; // PM=1, AM=0
    } else { // 24-hour mode fallback
        hour = bcd_2_dec(hr_reg & 0x3F);
        j = 0;
    }
 
    i2c_read(1); // Day of week (ignore)
    date = bcd_2_dec(i2c_read(1));
    month = bcd_2_dec(i2c_read(1));
    year = bcd_2_dec(i2c_read(0));
    i2c_stop();
}

// LCD functions
void lcd_command(unsigned char i) {
    RC2 = 0; // RS=0
    PORTD = i;
    RC0 = 1; // EN=1
    __delay_us(10); // Short delay to latch data
    RC0 = 0; // EN=0
    __delay_ms(2);  // Wait for command to process
}

void lcd_data(unsigned char i) {
    RC2 = 1; // RS=1
    PORTD = i;
    RC0 = 1;
    __delay_us(10);
    RC0 = 0;
    __delay_ms(2);
}

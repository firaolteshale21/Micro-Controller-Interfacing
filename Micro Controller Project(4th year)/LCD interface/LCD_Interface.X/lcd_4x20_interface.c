#include <xc.h>
#include <stdio.h> // Include stdio.h for sprintf function
#include <string.h> // Include string.h for strcat function

// LCD Pin Declarations
#define LCD_RS LATB4
#define LCD_EN LATB5
#define LCD_D7 LATB3
#define LCD_D6 LATB2
#define LCD_D5 LATB1
#define LCD_D4 LATB0

#define LCD_RS_Direction TRISB4
#define LCD_EN_Direction TRISB5
#define LCD_D7_Direction TRISB3
#define LCD_D6_Direction TRISB2
#define LCD_D5_Direction TRISB1
#define LCD_D4_Direction TRISB0

// Function prototypes
void Lcd_Init();
void Lcd_Cmd(unsigned char);
void Lcd_Out(unsigned char, unsigned char, const char *);
void ADC_Init();
unsigned int ADC_Read(unsigned char);
void Delay_ms(unsigned int);

#define _XTAL_FREQ 8000000 // Define oscillator frequency (8MHz)

void main() {
    // Variables
    unsigned int adcValue = 0;
    float temperature = 0.0;
    char tempStr[16]; // Buffer to hold temperature string

    // ADC configuration
    ADC_Init();    // Initialize ADC

    // Initializing and clearing the LCD screen
    Lcd_Init();
    Lcd_Cmd(0x01); // Clear display
    Lcd_Cmd(0x0C); // Turn off cursor

    while (1) {
        // Read the ADC value from the LM35 channel
        adcValue = ADC_Read(0);

        // Convert the ADC value to voltage (assuming 10-bit ADC and 5V reference voltage)
        // Voltage = (ADC Value / 1023) * 5.0
        // Temperature (C) = Voltage * 100
        temperature = (adcValue / 1023.0) * 5.0 * 100;

        // Display the temperature message on the LCD
        Lcd_Cmd(0x01); // Clear display
        Lcd_Out(1, 1, "The Temperature is:");
        sprintf(tempStr, "%.1f", temperature); // Convert temperature to string with one decimal place
        strcat(tempStr, " C"); // Append " C" to the temperature string
        Lcd_Out(2, 1, tempStr);

        // Delay for 1 second
        Delay_ms(1000);
    }
}

// LCD Initialization
void Lcd_Init() {
    LCD_RS_Direction = 0; // RS pin as output
    LCD_EN_Direction = 0; // EN pin as output
    LCD_D7_Direction = 0; // D7 pin as output
    LCD_D6_Direction = 0; // D6 pin as output
    LCD_D5_Direction = 0; // D5 pin as output
    LCD_D4_Direction = 0; // D4 pin as output

    LCD_RS = 0; // Initialize RS pin
    LCD_EN = 0; // Initialize EN pin

    // Initialization sequence for 4-bit mode
    Lcd_Cmd(0x33);
    Lcd_Cmd(0x32);
    Lcd_Cmd(0x28); // 4-bit mode, 2 lines, 5x7 font
    Lcd_Cmd(0x0C); // Display ON, Cursor OFF
    Lcd_Cmd(0x06); // Increment cursor
    Lcd_Cmd(0x01); // Clear display
    Delay_ms(2);
}

// Send command to LCD
void Lcd_Cmd(unsigned char cmd) {
    LCD_RS = 0; // RS low for command
    LCD_EN = 1; // Enable high
    LCD_D7 = (cmd & 0x80) >> 7;
    LCD_D6 = (cmd & 0x40) >> 6;
    LCD_D5 = (cmd & 0x20) >> 5;
    LCD_D4 = (cmd & 0x10) >> 4;
    LCD_EN = 0;
    Delay_ms(1);

    LCD_EN = 1;
    LCD_D7 = (cmd & 0x08) >> 3;
    LCD_D6 = (cmd & 0x04) >> 2;
    LCD_D5 = (cmd & 0x02) >> 1;
    LCD_D4 = cmd & 0x01;
    LCD_EN = 0;
    Delay_ms(1);
}

// Send data to LCD
void Lcd_Out(unsigned char row, unsigned char column, const char *text) {
    unsigned char pos;

    // Set cursor position
    pos = (row - 1) * 0x40 + (column - 1);
    Lcd_Cmd(0x80 | pos);

    // Send characters
    while (*text != '\0') {
        LCD_RS = 1; // RS high for data
        LCD_EN = 1; // Enable high
        LCD_D7 = (*text & 0x80) >> 7;
        LCD_D6 = (*text & 0x40) >> 6;
        LCD_D5 = (*text & 0x20) >> 5;
        LCD_D4 = (*text & 0x10) >> 4;
        LCD_EN = 0;
        Delay_ms(1);

        LCD_EN = 1;
        LCD_D7 = (*text & 0x08) >> 3;
        LCD_D6 = (*text & 0x04) >> 2;
        LCD_D5 = (*text & 0x02) >> 1;
        LCD_D4 = *text & 0x01;
        LCD_EN = 0;
        Delay_ms(1);

        text++;
    }
}

// ADC Initialization
void ADC_Init() {
    ADCON1 = 0x0E; // Configure AN0 as analog, the rest as digital
    ADCON2 = 0x80; // Right justified result, Fosc/2 ADC clock, 12 TAD
}

// Read ADC value
unsigned int ADC_Read(unsigned char channel) {
    ADCON0 = (channel << 2) | 0x01; // Select channel and enable ADC
    __delay_us(5); // Wait for ADC acquisition time
    GO_nDONE = 1; // Start conversion

    while (GO_nDONE); // Wait for conversion to complete

    return (ADRESH << 8) + ADRESL; // Return result
}

// Delay in milliseconds
void Delay_ms(unsigned int ms) {
    unsigned int i, j;

    for (i = 0; i < ms; i++) {
        for (j = 0; j < 1000; j++) {
            __delay_us(1); // Delay 1 microsecond
        }
    }
}

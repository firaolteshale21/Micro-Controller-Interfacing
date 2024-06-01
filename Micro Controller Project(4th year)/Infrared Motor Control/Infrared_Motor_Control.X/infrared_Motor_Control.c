// Configuration Bits
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (RE3 input pin disabled; MCLR pin enabled)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)

// Include necessary headers
#include <xc.h>

// Define clock frequency
#define _XTAL_FREQ 8000000  // 8 MHz

// Define pin mappings
#define IR_SENSOR PORTBbits.RB0  // IR sensor input
#define MOTOR_IN1 LATBbits.LATB1 // Motor driver input 1
#define MOTOR_IN2 LATBbits.LATB2 // Motor driver input 2

// Function to initialize ports
void init_ports(void) {
    TRISBbits.TRISB0 = 1;  // Set RB0 as input (IR sensor)
    TRISBbits.TRISB1 = 0;  // Set RB1 as output (Motor IN1)
    TRISBbits.TRISB2 = 0;  // Set RB2 as output (Motor IN2)

    // Initialize motor outputs to low
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
}

// Main function
void main(void) {
    // Initialize the ports
    init_ports();

    while (1) {
        // Check the state of the IR sensor
        if (IR_SENSOR == 1) {
            // IR sensor detected an object
            // Set motor direction (e.g., forward)
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 0;
        } else {
            // IR sensor did not detect an object
            // Set motor direction (e.g., backward)
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 1;
        }

        // Add a small delay to debounce the sensor input
        __delay_ms(100);
    }
}

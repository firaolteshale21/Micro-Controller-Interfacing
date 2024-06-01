#include <18F4550.h>

// Configuration bits
#fuses NOWDT, HSPLL, PLL5, CPUDIV1, USBDIV, VREGEN, NOLVP
#use delay(clock=48MHz)  
// Define pins
#define BUTTON0 PIN_B0 // Start motor low speed
#define BUTTON2 PIN_B2 // Increase speed further
#define BUTTON3 PIN_B3 // Stop motor
#define MOTOR_PWM PIN_C2 // Motor PWM output pin

void setup_pwm(int duty) {
    set_pwm1_duty(duty);
}

void main() {
    // Set pin directions
    set_tris_b(0x0F); // Set RB0-RB3 as input (Buttons), others as outputs
    set_tris_c(0xFB); // Set RC2 as output (Motor PWM), others as inputs

    // Initialize PWM
    setup_timer_2(T2_DIV_BY_16, 255, 1); // Timer2 setup
    setup_ccp1(CCP_PWM); // Configure CCP1 as PWM

    int duty_cycle = 0; // Variable to store duty cycle value

    while (1) {
        if (input(BUTTON0)) {
            // Start motor at low speed
            duty_cycle = 10;
            setup_pwm(duty_cycle);
        } else if (input(BUTTON2)) {
            // Increase speed further
            duty_cycle = 200;
            setup_pwm(duty_cycle);
        } else if (input(BUTTON3)) {
            // Stop motor
            duty_cycle = 0; // Stop motor (0% duty cycle)
            setup_pwm(duty_cycle);
        }

        delay_ms(100); // Small delay for debouncing
    }
}


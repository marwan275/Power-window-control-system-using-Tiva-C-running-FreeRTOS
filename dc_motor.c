#include "dc_motor.h"
#include "tm4c123gh6pm.h"

// Function to initialize the GPIO pins for the motor
void dc_motor_init(){
  // Enable the clock gate control bit for GPIO Port F
  SYSCTL_RCGCGPIO_R |= 0x02;
  
  // Enable the ability to modify Port F pins 2 and 3
  GPIO_PORTF_CR_R |= 0xc;
  
  // Set the direction of Port F pins 2 and 3 as output
  GPIO_PORTF_DIR_R |= 0xc;
  
  // Disable the pull-up resistor for Port F pin 0
  GPIO_PORTF_PUR_R |= 0;
  
  // Enable digital functionality for all pins of Port F
  GPIO_PORTF_DEN_R = 0xff;
}

// Function to rotate the motor clockwise
void dc_motor_cw(){
  // Set Port F pin 2 high and pin 3 low
  GPIO_PORTF_DATA_R |= (1<<2);
  GPIO_PORTF_DATA_R &= ~(1<<3);
}

// Function to rotate the motor counterclockwise
void dc_motor_ccw(){
  // Set Port F pin 3 high and pin 2 low
  GPIO_PORTF_DATA_R |= (1<<3);
  GPIO_PORTF_DATA_R &= ~(1<<2);
}

// Function to stop the motor
void dc_motor_stop(){
  // Set both Port F pins 2 and 3 low
  GPIO_PORTF_DATA_R &= ~(1<<2);
  GPIO_PORTF_DATA_R &= ~(1<<3);
}

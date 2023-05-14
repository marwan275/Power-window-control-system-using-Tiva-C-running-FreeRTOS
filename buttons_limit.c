#include "buttons.h"
#include "tm4c123gh6pm.h"

void buttons_limits_init(){
	
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
	__asm__("NOP; NOP; NOP; NOP;");
	
	//Jam button
	GPIO_PORTA_CR_R |= (1<<2)|(1<<7);
	GPIO_PORTA_DIR_R &=~ ((1<<2)|(1<<7));
	GPIO_PORTA_PUR_R |= (1<<2)|(1<<7);
	GPIO_PORTA_DEN_R |= (1<<2)|(1<<7);
	
	GPIO_PORTA_IS_R &= ~(1<<2);        /* make bit 4, 0 edge sensitive */
  GPIO_PORTA_IBE_R &=~(1<<2);         /* trigger is controlled by IEV */
  GPIO_PORTA_IEV_R &= ~(1<<2);        /* falling edge trigger */
  GPIO_PORTA_ICR_R |= (1<<2);          /* clear any prior interrupt */
  GPIO_PORTA_IM_R  |= (1<<2);          /* unmask interrupt */
    
  NVIC_PRI0_R |= (1<<5) | (1<<6) | (1<<7); // Set interrupt priority to 7 (lower priority)
  NVIC_EN0_R |= 0x00000001;  // Enable interrupt 0 (corresponds to Port A)
	
	//buttons and limit up on port D
  GPIO_PORTD_CR_R |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);
  GPIO_PORTD_DIR_R &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6));
	GPIO_PORTD_PUR_R |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6); 
	GPIO_PORTD_DEN_R |= (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);
	
	//lock on port f
	GPIO_PORTF_CR_R |= (1<<4);
  GPIO_PORTF_DIR_R &= ~(1<<4);
	GPIO_PORTF_PUR_R |= (1<<4); 
	GPIO_PORTF_DEN_R |= (1<<4);
	 
}
unsigned char read_btn_limit(unsigned char choice){
switch(choice){
	case driver_up: 	
		if(GPIO_PORTD_DATA_R & (1<<driver_up)){return 0;}else {return 1;}
		break;
	case driver_down: 	
		if(GPIO_PORTD_DATA_R & (1<<driver_down)){return 0;}else {return 1;}
		break;
	case pass_up: 	
		if(GPIO_PORTD_DATA_R & (1<<pass_up)){return 0;}else {return 1;}
		break;
	case pass_down: 	
		if(GPIO_PORTD_DATA_R & (1<<pass_down)){return 0;}else {return 1;}
		break;
	case limit_up: 	
		if(GPIO_PORTD_DATA_R & (1<<limit_up)){return 0;}else {return 1;}
		break;
	case limit_down: 	
		if(GPIO_PORTA_DATA_R & (1<<(limit_down ))){return 0;}else {return 1;}
		break;
	case lock:
		if(GPIO_PORTF_DATA_R & (1<<lock)){return 0;}else {return 1;}
		break;
}

}

void clear_flag_A(){
	GPIO_PORTA_ICR_R |=(1<<2);
}

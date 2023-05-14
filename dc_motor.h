#ifndef dc_motor
#define dc_motor

#define MOTOR_PORT PORTF
#define MOTOR_PIN_1 PIN_2
#define MOTOR_PIN_2 PIN_3

void dc_motor_init();
void dc_motor_cw();
void dc_motor_ccw();
void dc_motor_stop();
#endif
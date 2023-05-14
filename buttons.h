#ifndef buttons
#define buttons


#define driver_up 0
#define driver_down 1
#define pass_up 2
#define pass_down 3
#define limit_up 6
#define limit_down 7
#define lock 4

void buttons_limits_init();
unsigned char read_btn_limit(unsigned char choice);
void clear_flag_A();
#endif
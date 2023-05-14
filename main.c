#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include "macros.h"
#include "buttons.h"
#include "dc_motor.h"


xSemaphoreHandle jam_sem;
xSemaphoreHandle xMutex;
xTaskHandle passengerHandle;
xTaskHandle driverHandle;

typedef enum {LOCKED = 1,UNLOCKED = 2}modes;
uint8_t mode = UNLOCKED;
uint8_t bck_frm_jam = 0;

void driver_control(void* pvParameters);
void pass_control(void* pvParameters);
void jam(void* pvParameters);

uint8_t up = 1 , down = 2 , stop = 3;	

int main(void){
	buttons_limits_init();
	dc_motor_init();

	__ASM("CPSIE i");
	
	vSemaphoreCreateBinary(jam_sem);
	xMutex= xSemaphoreCreateMutex();

	
	if(jam_sem != NULL){
		
	xTaskCreate(driver_control, "Driver Control Task", configMINIMAL_STACK_SIZE, NULL, 1, &driverHandle);
	xTaskCreate(pass_control, "Passenger Control Task", configMINIMAL_STACK_SIZE, NULL, 1, &passengerHandle);
	xTaskCreate(jam, "Jam Window Task", configMINIMAL_STACK_SIZE, NULL, 4, NULL);

	vTaskStartScheduler();
	}else{}
	while(1){}
	return 0;
}


void driver_control(void* pvParameters) {
	while(1)
	{
		xSemaphoreTake(xMutex,portMAX_DELAY);
		if(!read_btn_limit(lock) && mode == LOCKED){
			mode = UNLOCKED;
			vTaskResume(passengerHandle);
		}
		
		if(read_btn_limit(lock) && mode == UNLOCKED){
			mode = LOCKED;
			dc_motor_stop();
			vTaskDelay(100/portTICK_PERIOD_MS);
			vTaskSuspend(passengerHandle);
		}
		
		if(read_btn_limit(driver_up) && read_btn_limit(limit_up)){
			dc_motor_cw();
			vTaskDelay(400/portTICK_RATE_MS);
			if(read_btn_limit(driver_up) && read_btn_limit(limit_up)){
			  while (read_btn_limit(driver_up) && read_btn_limit(limit_up) && !read_btn_limit(driver_down)){
					if(bck_frm_jam == 1){
						bck_frm_jam = 0;
					break;
					}
					if(mode == UNLOCKED && read_btn_limit(pass_down)){
						break;
					}
				}
					dc_motor_stop();
		 } else {
			 while(read_btn_limit(limit_up) && !read_btn_limit(driver_down)){
				 if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
				if(mode == UNLOCKED && read_btn_limit(pass_down)){
						break;
					}
			 }
						dc_motor_stop();
		 }
		}
		
	else if(read_btn_limit(driver_down) && read_btn_limit(limit_down)){
			dc_motor_ccw();
			vTaskDelay(400/portTICK_RATE_MS);
			if(read_btn_limit(driver_down) && read_btn_limit(limit_down)){
			  while (read_btn_limit(driver_down) && read_btn_limit(limit_down) && !read_btn_limit(driver_up)){
					if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
				if(mode == UNLOCKED && read_btn_limit(pass_up)){
						break;
					}
				}
					dc_motor_stop();
		 } else {
			 while(read_btn_limit(limit_down) && !read_btn_limit(driver_up)){
				 if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
			 if(mode == UNLOCKED && read_btn_limit(pass_up)){
						break;
					}
			 }
						dc_motor_stop();
				}
			}
		xSemaphoreGive(xMutex);
		vTaskDelay(100);
	}
}

void pass_control(void* pvParameters){
	while(1)
	{
		xSemaphoreTake(xMutex,portMAX_DELAY);
		if(read_btn_limit(pass_up) && read_btn_limit(limit_up)){
			dc_motor_cw();
			vTaskDelay(400/portTICK_RATE_MS);
			if(read_btn_limit(pass_up) && read_btn_limit(limit_up)){
			  while (read_btn_limit(pass_up) && read_btn_limit(limit_up) && !read_btn_limit(driver_down) && !read_btn_limit(lock)){
				if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
				}
					dc_motor_stop();
		 } else {
			 while(read_btn_limit(limit_up) && !read_btn_limit(pass_down)&& !read_btn_limit(driver_down) && !read_btn_limit(lock)){
			 if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
			 }
						dc_motor_stop();
		 }
		}
		
		if(read_btn_limit(pass_down) && read_btn_limit(limit_down)){
			dc_motor_ccw();
			vTaskDelay(400/portTICK_RATE_MS);
			if(read_btn_limit(pass_down) && read_btn_limit(limit_down)){
			  while (read_btn_limit(pass_down) && read_btn_limit(limit_down)&& !read_btn_limit(driver_up) && !read_btn_limit(lock)){
				if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
				}
					dc_motor_stop();
		 } else {
			 while(read_btn_limit(limit_down) && !read_btn_limit(pass_up) && !read_btn_limit(driver_up) && !read_btn_limit(lock) ){
			 if(bck_frm_jam == 1){
					 bck_frm_jam = 0;
					break;
					}
			 }
						dc_motor_stop();
		 }
		}
		xSemaphoreGive(xMutex);
		vTaskDelay(100);
	}

}

void jam(void* pvParameters){
	xSemaphoreTake(jam_sem,0);	
	while(1){
	xSemaphoreTake(jam_sem,portMAX_DELAY);	
	dc_motor_stop();  
	dc_motor_ccw();
	vTaskDelay(500/portTICK_RATE_MS);
	dc_motor_stop();  
	bck_frm_jam = 1;
	}
}
void GPIOA_Handler()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	clear_flag_A();
	xSemaphoreGiveFromISR(jam_sem,&xHigherPriorityTaskWoken );
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
} 

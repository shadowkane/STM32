#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* declare functions */
void My_GPIO_Init(void);
void delayMs(unsigned int);
/* End declare functions */

/* declare variables */
GPIO_InitTypeDef GPIO_initStruct;
/* End declare variables */

/* Begin main section */
int main(){
	My_GPIO_Init();
	
	/* Begin loop section */
	while(1){
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)){
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
		}else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
		}else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
		delayMs(10);
		
	}
	/* End loop section */
}
/* End main section */

void My_GPIO_Init(void){
	// Configure port A
	// Enable clock for the GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Set pin 0 as push-pull output 
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_initStruct);
	// Reset port
	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1);
	
	// Configure port B
	// Enable clock for the GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// Set pin 0 as pull-down input 
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_initStruct);
}

void delayMs(unsigned int delayTime){
	unsigned int i, j;
	for(i=0; i<delayTime; i++)
		for(j=0; j<0x2fff; j++);
}

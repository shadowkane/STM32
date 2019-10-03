#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void my_GPIO_Init(void);

void sysTickInit(unsigned int frequency){
	RCC_ClocksTypeDef clockStruct;
	RCC_GetClocksFreq(&clockStruct);
	// SysTick time generates an interrupt requests and to configure it we use the SysTick_Config function to set the time interval which the interruption should be called.
	// SysTick_Config(uint_32 ticks) where tick is the time unit to execute one instruction
	// to get the number of ticks we need to pass to the SysTick_Config we can get the total ticks we run for a second which is the CPU speed which is the HCLK and divided by a frequency we choose.
	SysTick_Config(clockStruct.HCLK_Frequency/frequency);
}

// the default callback funtion for the SysTick
volatile uint32_t taskInc = 0;
void SysTick_Handler (void){
	taskInc++;
}

// function returns time in milli seconds.
uint32_t millis(void){
	// because our taskInc increment each millisecond so we just return it.
	return taskInc;
}

// delay function
void delay_Millis(uint32_t delay_ms){
	uint32_t startTime = millis();
	uint32_t endTime = startTime + delay_ms;
	// if the they are no tick time overflowing that would cause the startTime to be greater than then endTime
	if(startTime<endTime){
		while((millis()>=startTime) && (millis()<endTime));
	}
	else{
		while((millis()<=startTime) || (millis()>endTime));
	}
}

int main(){
	
	my_GPIO_Init();
	// init the system tick to launch the callback every 1 milli second
	sysTickInit(1000);
	
	while(1){
		// toggle pin 0 of the gpio A every 1 sec
		GPIOA->ODR ^= GPIO_Pin_0;
		delay_Millis(1000);
	}
}

void my_GPIO_Init(void){
	GPIO_InitTypeDef gpioStructInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	gpioStructInit.GPIO_Pin = GPIO_Pin_0;
	gpioStructInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpioStructInit);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

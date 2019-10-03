#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

void my_GPIO_Config(void);

int main(void){
	my_GPIO_Config();
	int i;
	while(1){
		for(i=0;i<0x10000;i++);
	}
}

uint32_t getTimerClock(){
	RCC_ClocksTypeDef clockStruct;
	RCC_GetClocksFreq(&clockStruct);
	// check the stm32cubeMX for this equation.
	if(clockStruct.SYSCLK_Frequency == clockStruct.PCLK1_Frequency){
		return clockStruct.PCLK1_Frequency;
	}
	else{
		return clockStruct.PCLK1_Frequency * 2;
	}
}

void my_GPIO_Config(void){
	
	// config the Port A pin 8 as output for the timer to test the PWM
	GPIO_InitTypeDef my_GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&my_GPIO_InitStruct);
	my_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	my_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	my_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &my_GPIO_InitStruct);
	
	// config the Timer1	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef timer_InitStruct;
	TIM_TimeBaseStructInit(&timer_InitStruct);
	timer_InitStruct.TIM_Prescaler = getTimerClock() / 100;
	timer_InitStruct.TIM_Period = 1000;
	timer_InitStruct.TIM_ClockDivision = 0;
	timer_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer_InitStruct);
	
	TIM_OCInitTypeDef pwm_InitStruct;
	TIM_OCStructInit(&pwm_InitStruct);
	pwm_InitStruct.TIM_Pulse = 500;
	pwm_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	pwm_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	pwm_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &pwm_InitStruct);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
	
};


/*
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define PERIOD 1000
int main(void)
{
	int TIM_Pulse = 0;
	int i;

	GPIO_InitTypeDef port;
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef timerPWM;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_IPU;
	port.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &port);

	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Pin = GPIO_Pin_0;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &port);

	TIM_TimeBaseStructInit(&timer);
	timer.TIM_Prescaler = 720;
	timer.TIM_Period = PERIOD;
	timer.TIM_ClockDivision = 0;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &timer);

	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = 10;
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &timerPWM);

    TIM_Cmd(TIM2, ENABLE);

    while(1)
    {
    	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
    		if (TIM_Pulse < PERIOD)
    			TIM_Pulse++;
    		TIM2->CCR1 = TIM_Pulse;

    	}
    	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) {
    		if (TIM_Pulse > 0)
    			TIM_Pulse--;
    		TIM2->CCR1 = TIM_Pulse;
    	}

    
    	for(i=0;i<0x10000;i++);
    }
}*/

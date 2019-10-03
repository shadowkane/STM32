#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

void my_GPIO_Config(void);
void my_USART_Config(void);
void USART_SendSTR(char* str);


int main(void){
	my_GPIO_Config();
	my_USART_Config();
	USART_SendSTR("Start program\r");
	int i;
	while(1){
		for(i=0; i<100000; i++);
	}
}
void my_GPIO_Config(void){
	// enabble GPIO B port to control the led
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef my_GPIO_InitStruct;
	// config the led pin ( GPIO B pin 0)
	my_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	my_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	my_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &my_GPIO_InitStruct);
	
}
void my_USART_Config(void){
	// enable the GPIO A port for the usart 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef my_GPIO_InitStruct;
	// config the TX pin (GPIO A pin 9)
	my_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	my_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	my_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &my_GPIO_InitStruct);
	// config the RX pin (GPIO A pin 10)
	my_GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	my_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	my_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &my_GPIO_InitStruct);
	
	// enbale the usart peripheral
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef my_USART_InitStruct;
	my_USART_InitStruct.USART_BaudRate = 9600;
	my_USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	my_USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	my_USART_InitStruct.USART_Parity = USART_Parity_No;
	my_USART_InitStruct.USART_StopBits = USART_StopBits_1;
	my_USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &my_USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
	
	// enbale the usart receive (RXEN) interrupt. this interrupt is generated when the usart1 receive data which mean its register is not empty
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// enable the usart1 global interrupt
	NVIC_EnableIRQ(USART1_IRQn);
}

// the default method for the USART IT handler
void USART1_IRQHandler(void){
	if(!USART_GetFlagStatus(USART1, USART_IT_RXNE)){
		if((char) USART_ReceiveData(USART1) == 'o'){
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		}
		else if((char) USART_ReceiveData(USART1) == 'f'){
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		}
		else if((char) USART_ReceiveData(USART1) == 't'){
			GPIOB->ODR ^= GPIO_Pin_0;
		}
		USART_SendSTR("data received\r");
	}
}

void USART_SendSTR(char* str){
	while(*str){
		USART_SendData(USART1, *str++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}

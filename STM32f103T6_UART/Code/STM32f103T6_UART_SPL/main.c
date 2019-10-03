#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

void my_GPIO_Config(void);
void my_UART_Config(void);

int main(void){
	my_GPIO_Config();
	my_UART_Config();
	int i;
	while(1){
		// receive data
		// wait for data, flag indicate that data is received
		while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
		char str_RX = USART_ReceiveData(USART1);
		// send data
		char* str_TX = "data received\r";
		while(*str_TX){
			USART_SendData(USART1, *str_TX++);
			// wait for the data completely transferred
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		}
		if(str_RX == 'O'){
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
		}
		if(str_RX == 'F'){
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		}
		for(i=0; i<100000; i++);
	}
}

void my_GPIO_Config(void){
	// cmd led from uart
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void my_UART_Config(void){
	// enable the GPIOA port and setup the pin 9 and 10, pin 9 for TX and pin 10 for RX. (see datasheet) 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// pin 9 is TX which mean it's outpu AF
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// pin 10 is RX which mean it's input
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// enable the USART register
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; // world length is 8 bits (ascii)
	USART_Init(USART1, &USART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
}

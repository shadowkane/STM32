#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void my_GPIO_Init(void);
uint16_t readADC1(uint8_t);

int main(void){
	my_GPIO_Init();
	int i;
	volatile uint16_t temp = 0;
	while(1){
		temp = readADC1(1);
		for(i=0; i<1000000; i++);
		GPIOB->ODR ^= GPIO_Pin_0;
	}
}
void my_GPIO_Init(void){
	ADC_InitTypeDef adc_initStruct;
	
	// the PCLK2 is the ADC prescaler just after the APB2 prescaler which take the HCLK frequency.
	// divide atleast by 6 because the HCLK is 72Mhz and the APB2 prescale set to 1 (div 1) and the ADC frequency speed should be between 600kHz and 14Mhz so we divide by 6 the APB2 frequency which is in our case 72Mhz (72/6 = 12Mhz).
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	/* Enable ADC1 clock so that we can talk to it */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	// deinitialize ADC1 peripheral register to its default reset value
	ADC_DeInit(ADC1);
	
	// use single ADC (the other mode is dual mode which use tow ADCs joinlty like ADC1 and ADC2)
	adc_initStruct.ADC_Mode = ADC_Mode_Independent;
	// use only one channel
	adc_initStruct.ADC_NbrOfChannel = 1;
	// use single conversion mode which mean the adc will stop just after converting one simple
	adc_initStruct.ADC_ContinuousConvMode = DISABLE;
	// disable the scan conversion mode because we have only one channel. (scan conversion mode is for multi channel scan where the conversion is repeted for all channel by rank)
	adc_initStruct.ADC_ScanConvMode = DISABLE;
	// use no external trigger to start this conversion, which mean it will start by software. (injection conversion mode is  intended to trigger a channel to start scanning by interrupting the normal conversion channels)
	adc_initStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	// because the ADC convert an analog data to digital data in 12bit (12bit resolution) and we will store in the a register of 16bits so align the data to the right and the rest bits will have 0.
	adc_initStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	// init the ADC1 peripheral register
	ADC_Init(ADC1, &adc_initStruct);
	// enable ADC1
	ADC_Cmd(ADC1,ENABLE);
	// enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);
	// before start calibration the ADC must be in the power-down state for at least 2 clock cycles (ADON=0). so wait until the power is on again by waiting for the reset calibration status
	while(ADC_GetResetCalibrationStatus(ADC1));
	// now we can start the calibration
	ADC_StartCalibration(ADC1);
	// wait until the calibration ends
	while(ADC_GetCalibrationStatus(ADC1));
	
	// config GPIO B as output
	GPIO_InitTypeDef gpioStructInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpioStructInit.GPIO_Pin = GPIO_Pin_0;
	gpioStructInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStructInit.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpioStructInit);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

uint16_t readADC1(uint8_t channelNumber){
	ADC_RegularChannelConfig(ADC1, channelNumber, 1, ADC_SampleTime_1Cycles5);
	// start the sampling and the conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// wait until the conversion complet
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// return the conversion value
	return ADC_GetConversionValue(ADC1);
}

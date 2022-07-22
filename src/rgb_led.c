/*
/*
 * status_led.h
 *
 *  Created on: Apr 1, 2022
 *      Author: bartek
 */

#include "./rgb_led.h"

const inline uint32_t __RGBLed_ValueForFreq(const RGBLed_HandleTypeDef* handle, uint8_t val) {
	if (handle->blinkFreqHz > 0 && val > 0) {
		val = abs(2 * val * handle->clock / handle->clockPeriod - 255);
	}
	return (handle->Init.timer->Init.Period+1) - val * (handle->Init.timer->Init.Period+1) / 255;
}

void __RGBLed_UpdatePWMs(RGBLed_HandleTypeDef* handle) {
	__HAL_TIM_SET_COMPARE(handle->Init.timer, handle->Init.channelR, __RGBLed_ValueForFreq(handle, handle->valueR));
	__HAL_TIM_SET_COMPARE(handle->Init.timer, handle->Init.channelG, __RGBLed_ValueForFreq(handle, handle->valueG));
	__HAL_TIM_SET_COMPARE(handle->Init.timer, handle->Init.channelB, __RGBLed_ValueForFreq(handle, handle->valueB));
}

void RGBLed_SetFreq(RGBLed_HandleTypeDef* handle, uint16_t freqHz) {
	if(freqHz != handle->blinkFreqHz)
		handle->clock = 0;
	handle->blinkFreqHz = freqHz;
	handle->clockPeriod = (
			handle->Init.timerClockFreqHz/
			(handle->Init.timer->Init.Prescaler+1)
			/
			(handle->Init.timer->Init.Period+1)
		);
	__RGBLed_UpdatePWMs(handle);
}

void RGBLed_SetColour(RGBLed_HandleTypeDef* handle, uint8_t r, uint8_t g, uint8_t b) {
	handle->valueR = r * handle->multiplierR;
	handle->valueG = g * handle->multiplierG;
	handle->valueB = b * handle->multiplierB;
//	printf("LED(%3d, %3d, %3d)\r\n", r,g,b);
}

void RGBLed_FreqTick(RGBLed_HandleTypeDef* handle) {
	handle->clock += handle->blinkFreqHz;
	handle->clock %= handle->clockPeriod;
	__RGBLed_UpdatePWMs(handle);
}

void RGBLed_Init(RGBLed_HandleTypeDef* handle, RGBLed_InitTypeDef* config) {
	handle->Init = *config;
	handle->clock = 0;
	handle->multiplierR = 1;
	handle->multiplierG = 1;
	handle->multiplierB = 1;
	RGBLed_SetColour(handle,0,0,0);
	RGBLed_SetFreq(handle,0);
}

void RGBLed_Start(RGBLed_HandleTypeDef* handle) {
	HAL_TIM_Base_Start_IT(handle->Init.timer);
	HAL_TIM_PWM_Start(handle->Init.timer, handle->Init.channelR);
	HAL_TIM_PWM_Start(handle->Init.timer, handle->Init.channelG);
	HAL_TIM_PWM_Start(handle->Init.timer, handle->Init.channelB);
}

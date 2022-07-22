/*
 * status_led.h
 *
 *  Created on: Apr 1, 2022
 *      Author: bartek
 */

#include "main.h"

#ifndef RGB_LED_H_
#define RGB_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	TIM_HandleTypeDef *timer;
	__IO uint32_t channelR;
	__IO uint32_t channelG;
	__IO uint32_t channelB;
	uint32_t timerClockFreqHz;
} RGBLed_InitTypeDef;

typedef struct
{
	RGBLed_InitTypeDef Init;
	uint8_t valueR;
	uint8_t valueG;
	uint8_t valueB;
	float multiplierR;
	float multiplierG;
	float multiplierB;
	uint16_t blinkFreqHz;
	uint16_t clock;
	uint16_t clockPeriod;
} RGBLed_HandleTypeDef;

const inline uint32_t __RGBLed_ValueForFreq(const RGBLed_HandleTypeDef* handle, uint8_t val);
void __RGBLed_UpdatePWMs(RGBLed_HandleTypeDef* handle);
void RGBLed_SetFreq(RGBLed_HandleTypeDef* handle, uint16_t freqHz);
void RGBLed_SetColour(RGBLed_HandleTypeDef* handle, uint8_t r, uint8_t g, uint8_t b) ;
void RGBLed_FreqTick(RGBLed_HandleTypeDef* handle);
void RGBLed_Init(RGBLed_HandleTypeDef* handle, RGBLed_InitTypeDef* config);
void RGBLed_Start(RGBLed_HandleTypeDef* handle);


#ifdef __cplusplus
}
#endif

#endif /* STATUS_LED_H_ */

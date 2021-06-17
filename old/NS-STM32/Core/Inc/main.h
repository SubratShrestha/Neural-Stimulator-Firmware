/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "stm32wbxx_ll_spi.h"
#include "stm32wbxx_ll_bus.h"
#include "stm32wbxx_ll_cortex.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_utils.h"
#include "stm32wbxx_ll_pwr.h"
#include "stm32wbxx_ll_gpio.h"
#include "stm32wbxx_ll_dma.h"

#include "stm32wbxx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"
#include "command_queue.h"
#include "command_parse.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define COMMAND_TYPE 0 //0 IS STRING; 1 IS NUMBER

#define STIM_STATUS_STOP 0
#define STIM_STATUS_PHASE_ONE 1
#define STIM_STATUS_INTER_PHASE_GAP 2
#define STIM_STATUS_PHASE_TWO 3
#define STIM_STATUS_INTER_STIM_DEALY 4
#define STIM_STATUS_INTER_BURST_DELAY 5

#define STIM_MODE_UNI_CONT 0
#define STIM_MODE_UNI_NUM 1
#define STIM_MODE_BURST_CONT 2
#define STIM_MODE_BURST_NUM 3



/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
uint16_t STIM_AMP;
uint32_t PHASE_ONE_TIME;
uint32_t PHASE_TWO_TIME;
uint32_t INTER_PHASE_GAP;
uint32_t INTER_STIM_DELAY;
uint32_t PULSE_NUM;
uint8_t ANODIC_CATHODIC;
uint8_t STIM_TYPE;
uint32_t BURST_NUM;
uint32_t INTER_BURST_DELAY;
uint32_t PULSE_NUM_IN_ONE_BURST;
uint8_t RAMP_UP;
uint8_t SHORT_ELECTRODE;
//uint8_t BYPASS_CAP;


Queue COMMAND_QUEUE;


typedef struct ADC_DATA{
	uint16_t data[75];
}ADC_DATA;
ADC_DATA ADC_BUFFER[2];
uint8_t ENABLE_RECORD;
uint16_t RECORD_FREQ;
int32_t RECORD_START_OFFSET;
int32_t RECORD_END_OFFSET;



uint8_t STIM_MODE;
uint16_t DAC_PHASE_ONE_COMP;
uint16_t DAC_PHASE_TWO_COMP;
uint16_t TEMP_DAC_PHASE_ONE;
uint16_t TEMP_DAC_PHASE_TWO;
uint16_t TEMP_DAC_GAP;
uint32_t TEMP_PULSE_NUM;
uint32_t TEMP_PULSE_NUM_IN_BURST;
uint32_t TEMP_BURST_NUM;

uint16_t DAC_PHASE_ONE;
uint16_t DAC_PHASE_TWO;
uint16_t DAC_GAP;

uint8_t STIM_STATUS;
uint8_t PULSE_PROBE;

uint32_t PHASE_ONE_TIMER;
uint32_t PHASE_TWO_TIMER;
uint32_t PHASE_GAP_TIMER;
uint32_t STIM_DELAY_TIMER;
uint32_t BURST_DELAY_TIMER;

uint32_t PHASE_ONE_SAMPLE_TIMER;
uint32_t ELEC_OFFSET;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LDAC_Pin GPIO_PIN_9
#define LDAC_GPIO_Port GPIOA
#define STIM_EN_Pin GPIO_PIN_10
#define STIM_EN_GPIO_Port GPIOC
#define TOGGLE_Pin GPIO_PIN_5
#define TOGGLE_GPIO_Port GPIOB
#define SHORT_ELECTRODE_Pin GPIO_PIN_6
#define SHORT_ELECTRODE_GPIO_Port GPIOB
#define DUMMY_LOAD_Pin GPIO_PIN_7
#define DUMMY_LOAD_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

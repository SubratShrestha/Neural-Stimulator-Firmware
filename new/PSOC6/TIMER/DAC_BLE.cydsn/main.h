/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "project.h"
#include "inttypes.h"
#include "stdint.h"
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
uint16_t stim_amp;
uint32_t phase_1 = 10;
uint32_t inter_stim_gap = 0;
uint32_t phase_2 = 10;
uint32_t inter_stim_delay = 0;
uint32_t num_pulses;
uint8_t anodic_cathodic;
uint8_t stim_type;
uint32_t burst_num;
uint32_t inter_burst_delay;
uint32_t pulse_num_in_one_burst;
uint8_t ramp_up;
uint8_t short_electrode;
//uint8_t BYPASS_CAP;


// values writing to the dac
uint16_t dac_phase_one;
uint16_t dac_phase_two;
uint16_t dac_gap;


uint32_t stim_status;

/*
???
uint8_t stim_status;
uint8_t PULSE_PROBE;

uint32_t PHASE_ONE_TIMER;
uint32_t PHASE_TWO_TIMER;
uint32_t PHASE_GAP_TIMER;
uint32_t STIM_DELAY_TIMER;
uint32_t BURST_DELAY_TIMER;

uint32_t PHASE_ONE_SAMPLE_TIMER;
uint32_t ELEC_OFFSET;
???
*/

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* [] END OF FILE */

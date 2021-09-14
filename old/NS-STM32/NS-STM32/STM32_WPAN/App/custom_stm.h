/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/custom_stm.h
 * Description        : Header for custom_stm.c module.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_STM_H
#define __CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum {
	/* STIMULATION_COMMAND_SERVICE */
	CUSTOM_STM_SERIAL_CMD_CHAR, CUSTOM_STM_CMD_FB_CHAR, CUSTOM_STM_OTA_REQ,
	/* RECORDING_STREAM_SERVICE */
	CUSTOM_STM_REC_STREAM_CHAR,
} Custom_STM_Char_Opcode_t;

typedef enum {
	/* SERIAL_COMMAND_INPUT_CHAR */
	CUSTOM_STM_SERIAL_CMD_CHAR_READ_EVT,
	CUSTOM_STM_SERIAL_CMD_CHAR_WRITE_NO_RESP_EVT,
	/* COMMAND_FEEDBACK_CHAR */
	CUSTOM_STM_CMD_FB_CHAR_READ_EVT,
	CUSTOM_STM_CMD_FB_CHAR_NOTIFY_ENABLED_EVT,
	CUSTOM_STM_CMD_FB_CHAR_NOTIFY_DISABLED_EVT,
	/* OTA_REQ */
	CUSTOM_STM_OTA_REQ_READ_EVT,
	CUSTOM_STM_OTA_REQ_WRITE_NO_RESP_EVT,
	/* RECORDING_STREAM_CHAR */
	CUSTOM_STM_REC_STREAM_CHAR_READ_EVT,
	CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_ENABLED_EVT,
	CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_DISABLED_EVT,

	CUSTOM_STM_BOOT_REQUEST_EVT
} Custom_STM_Opcode_evt_t;

typedef struct {
	uint8_t *pPayload;
	uint8_t Length;
} Custom_STM_Data_t;

typedef struct {
	Custom_STM_Opcode_evt_t Custom_Evt_Opcode;
	Custom_STM_Data_t DataTransfered;
	uint16_t ConnectionHandle;
	uint8_t ServiceInstance;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void SVCCTL_InitCustomSvc(void);
void Custom_STM_App_Notification(
		Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode,
		uint8_t *pPayload);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*__CUSTOM_STM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

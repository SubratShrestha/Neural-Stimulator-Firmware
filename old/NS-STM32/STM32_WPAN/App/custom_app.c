/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/custom_app.c
 * Description        : Custom Example Application (Server)
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "command_parse.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* STIMULATION_COMMAND_SERVICE */
  uint8_t               Cmd_fb_char_Notification_Status;
  /* RECORDING_STREAM_SERVICE */
  uint8_t               Rec_stream_char_Notification_Status;
/* USER CODE BEGIN CUSTOM_APP_Context_t */

/* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */
uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

uint8_t SecureReadData;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
  /* STIMULATION_COMMAND_SERVICE */
static void Custom_Cmd_fb_char_Update_Char(void);
static void Custom_Cmd_fb_char_Send_Notification(void);
  /* RECORDING_STREAM_SERVICE */
static void Custom_Rec_stream_char_Update_Char(void);
static void Custom_Rec_stream_char_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
/* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

/* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch(pNotification->Custom_Evt_Opcode)
  {
/* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

/* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

  /* STIMULATION_COMMAND_SERVICE */
    case CUSTOM_STM_SERIAL_CMD_CHAR_READ_EVT:
/* USER CODE BEGIN CUSTOM_STM_SERIAL_CMD_CHAR_READ_EVT */

/* USER CODE END CUSTOM_STM_SERIAL_CMD_CHAR_READ_EVT */
      break;

    case CUSTOM_STM_SERIAL_CMD_CHAR_WRITE_NO_RESP_EVT:
/* USER CODE BEGIN CUSTOM_STM_SERIAL_CMD_CHAR_WRITE_NO_RESP_EVT */
    {
		if(pNotification->DataTransfered.Length != 0){
			EnQueue(&COMMAND_QUEUE, pNotification->DataTransfered.pPayload,pNotification->DataTransfered.Length);
		}
	}
/* USER CODE END CUSTOM_STM_SERIAL_CMD_CHAR_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_CMD_FB_CHAR_READ_EVT:
/* USER CODE BEGIN CUSTOM_STM_CMD_FB_CHAR_READ_EVT */

/* USER CODE END CUSTOM_STM_CMD_FB_CHAR_READ_EVT */
      break;

    case CUSTOM_STM_CMD_FB_CHAR_NOTIFY_ENABLED_EVT:
/* USER CODE BEGIN CUSTOM_STM_CMD_FB_CHAR_NOTIFY_ENABLED_EVT */

/* USER CODE END CUSTOM_STM_CMD_FB_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CMD_FB_CHAR_NOTIFY_DISABLED_EVT:
/* USER CODE BEGIN CUSTOM_STM_CMD_FB_CHAR_NOTIFY_DISABLED_EVT */

/* USER CODE END CUSTOM_STM_CMD_FB_CHAR_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_OTA_REQ_READ_EVT:
/* USER CODE BEGIN CUSTOM_STM_OTA_REQ_READ_EVT */

/* USER CODE END CUSTOM_STM_OTA_REQ_READ_EVT */
      break;

    case CUSTOM_STM_OTA_REQ_WRITE_NO_RESP_EVT:
/* USER CODE BEGIN CUSTOM_STM_OTA_REQ_WRITE_NO_RESP_EVT */
    	 *(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
		  NVIC_SystemReset();
/* USER CODE END CUSTOM_STM_OTA_REQ_WRITE_NO_RESP_EVT */
      break;

  /* RECORDING_STREAM_SERVICE */
    case CUSTOM_STM_REC_STREAM_CHAR_READ_EVT:
/* USER CODE BEGIN CUSTOM_STM_REC_STREAM_CHAR_READ_EVT */

/* USER CODE END CUSTOM_STM_REC_STREAM_CHAR_READ_EVT */
      break;

    case CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_ENABLED_EVT:
/* USER CODE BEGIN CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_ENABLED_EVT */

/* USER CODE END CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_DISABLED_EVT:
/* USER CODE BEGIN CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_DISABLED_EVT */

/* USER CODE END CUSTOM_STM_REC_STREAM_CHAR_NOTIFY_DISABLED_EVT */
      break;

    default:
/* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

/* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
/* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

/* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
/* USER CODE BEGIN CUSTOM_APP_Notification_1 */

/* USER CODE END CUSTOM_APP_Notification_1 */

  switch(pNotification->Custom_Evt_Opcode)
  {
/* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

/* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
  case CUSTOM_CONN_HANDLE_EVT :
/* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

/* USER CODE END CUSTOM_CONN_HANDLE_EVT */
    break;

    case CUSTOM_DISCON_HANDLE_EVT :
/* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

/* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
    break;

    default:
/* USER CODE BEGIN CUSTOM_APP_Notification_default */

/* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

/* USER CODE BEGIN CUSTOM_APP_Notification_2 */

/* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
/* USER CODE BEGIN CUSTOM_APP_Init */

/* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

  /* STIMULATION_COMMAND_SERVICE */
void Custom_Cmd_fb_char_Update_Char(void) /* Property Read */
{
  Custom_STM_App_Update_Char(CUSTOM_STM_CMD_FB_CHAR, (uint8_t *)UpdateCharData);
  /* USER CODE BEGIN Cmd_fb_char_UC*/

  /* USER CODE END Cmd_fb_char_UC*/
  return;
}

void Custom_Cmd_fb_char_Send_Notification(void) /* Property Notification */
 {
  if(Custom_App_Context.Cmd_fb_char_Notification_Status)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_CMD_FB_CHAR, (uint8_t *)NotifyCharData);
    /* USER CODE BEGIN Cmd_fb_char_NS*/

    /* USER CODE END Cmd_fb_char_NS*/
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n ");
  }
  return;
}

  /* RECORDING_STREAM_SERVICE */
void Custom_Rec_stream_char_Update_Char(void) /* Property Read */
{
  Custom_STM_App_Update_Char(CUSTOM_STM_REC_STREAM_CHAR, (uint8_t *)UpdateCharData);
  /* USER CODE BEGIN Rec_stream_char_UC*/

  /* USER CODE END Rec_stream_char_UC*/
  return;
}

void Custom_Rec_stream_char_Send_Notification(void) /* Property Notification */
 {
  if(Custom_App_Context.Rec_stream_char_Notification_Status)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_REC_STREAM_CHAR, (uint8_t *)NotifyCharData);
    /* USER CODE BEGIN Rec_stream_char_NS*/

    /* USER CODE END Rec_stream_char_NS*/
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n ");
  }
  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

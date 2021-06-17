/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/custom_stm.c
  * Description        : Custom Example Service.
  *
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomStim_Cmd_SvcsHdle;                   /**< STIMULATION_COMMAND_SERVICE handle */
  uint16_t  CustomSerial_Cmd_CharHdle;                   /**< SERIAL_COMMAND_INPUT_CHAR handle */
  uint16_t  CustomCmd_Fb_CharHdle;                   /**< COMMAND_FEEDBACK_CHAR handle */
  uint16_t  CustomOta_ReqHdle;                   /**< OTA_REQ handle */
  uint16_t  CustomRec_Stream_SvcsHdle;                   /**< RECORDING_STREAM_SERVICE handle */
  uint16_t  CustomRec_Stream_CharHdle;                   /**< RECORDING_STREAM_CHAR handle */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static const uint8_t SizeSerial_Cmd_Char=5;
static const uint8_t SizeCmd_Fb_Char=5;
static const uint8_t SizeOta_Req=3;
static const uint8_t SizeRec_Stream_Char=240;
/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_STIMULATION_COMMAND_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x40,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_SERIAL_COMMAND_INPUT_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x41,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_COMMAND_FEEDBACK_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x42,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_OTA_REQ_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x11,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_RECORDING_STREAM_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x50,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_RECORDING_STREAM_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x51,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blue_aci *blue_evt;
/* USER CODE BEGIN Custom_STM_Event_Handler_1 */
  aci_gatt_write_permit_req_event_rp0    * attribute_modified;
  Custom_STM_App_Notification_evt_t Notification;
/* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case EVT_VENDOR:
      blue_evt = (evt_blue_aci*)event_pckt->data;
      switch(blue_evt->ecode)
      {

        case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED */
        	attribute_modified= (aci_gatt_write_permit_req_event_rp0*)blue_evt->data;
        	if(attribute_modified->Attribute_Handle == (CustomContext.CustomOta_ReqHdle + 1))
			{
			  BLE_DBG_P2P_STM_MSG("-- GATT : REBOOT REQUEST RECEIVED\n");
			  Notification.Custom_Evt_Opcode = CUSTOM_STM_OTA_REQ_WRITE_NO_RESP_EVT;
			  Notification.DataTransfered.Length=3;
			  Notification.DataTransfered.pPayload = calloc(3,sizeof(uint8_t));
			  Notification.DataTransfered.pPayload[0]=attribute_modified->Data[3];
			  Notification.DataTransfered.pPayload[1]=attribute_modified->Data[4];
			  Notification.DataTransfered.pPayload[2]=attribute_modified->Data[5];
			  Custom_STM_App_Notification(&Notification);
			}
          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED */
          break;
        case EVT_BLUE_GATT_READ_PERMIT_REQ :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ */
          break;
        case EVT_BLUE_GATT_WRITE_PERMIT_REQ:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ */
			attribute_modified = (aci_gatt_write_permit_req_event_rp0*)blue_evt->data;
			if(attribute_modified->Attribute_Handle == (CustomContext.CustomSerial_Cmd_CharHdle + 1))
			{
			  Notification.Custom_Evt_Opcode = CUSTOM_STM_SERIAL_CMD_CHAR_WRITE_NO_RESP_EVT;
			  Notification.DataTransfered.Length=attribute_modified->Data_Length;
			  Notification.DataTransfered.pPayload=attribute_modified->Data;
			  Custom_STM_App_Notification(&Notification);
			  aci_gatt_write_resp(attribute_modified->Connection_Handle, attribute_modified->Attribute_Handle, 0, 0, attribute_modified->Data_Length, attribute_modified->Data);
			}
			return_value = SVCCTL_EvtAckFlowEnable;

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ */
          break;

        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* EVT_VENDOR */

    /* USER CODE BEGIN EVENT_PCKT_CASES*/

    /* USER CODE END EVENT_PCKT_CASES*/

    default:
      break;
  }

/* USER CODE BEGIN Custom_STM_Event_Handler_2 */

/* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
/* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

/* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

    /*
     *          STIMULATION_COMMAND_SERVICE
     *
     * Max_Attribute_Records = 1 + 2*3 + 1*no_of_char_with_notify_or_indicate_property
     * service_max_attribute_record = 1 for STIMULATION_COMMAND_SERVICE +
     *                                2 for SERIAL_COMMAND_INPUT_CHAR +
     *                                2 for COMMAND_FEEDBACK_CHAR +
     *                                2 for OTA_REQ +
     *                                1 for COMMAND_FEEDBACK_CHAR configuration descriptor +
     *                              = 8
     */

    COPY_STIMULATION_COMMAND_SERVICE_UUID(uuid.Char_UUID_128);
    aci_gatt_add_service(UUID_TYPE_128,
                      (Service_UUID_t *) &uuid,
                      PRIMARY_SERVICE,
                      8,
                      &(CustomContext.CustomStim_Cmd_SvcsHdle));

    /**
     *  SERIAL_COMMAND_INPUT_CHAR
     */
    COPY_SERIAL_COMMAND_INPUT_CHAR_UUID(uuid.Char_UUID_128);
    aci_gatt_add_char(CustomContext.CustomStim_Cmd_SvcsHdle,
                      UUID_TYPE_128, &uuid,
                      SizeSerial_Cmd_Char,
                      CHAR_PROP_WRITE_WITHOUT_RESP,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP,
                      0x10,
                      CHAR_VALUE_LEN_CONSTANT,
                      &(CustomContext.CustomSerial_Cmd_CharHdle));
    /**
     *  COMMAND_FEEDBACK_CHAR
     */
    COPY_COMMAND_FEEDBACK_CHAR_UUID(uuid.Char_UUID_128);
    aci_gatt_add_char(CustomContext.CustomStim_Cmd_SvcsHdle,
                      UUID_TYPE_128, &uuid,
                      SizeCmd_Fb_Char,
                      CHAR_PROP_NOTIFY,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE,
                      0x10,
                      CHAR_VALUE_LEN_CONSTANT,
                      &(CustomContext.CustomCmd_Fb_CharHdle));
    /**
     *  OTA_REQ
     */
    COPY_OTA_REQ_UUID(uuid.Char_UUID_128);
    aci_gatt_add_char(CustomContext.CustomStim_Cmd_SvcsHdle,
                      UUID_TYPE_128, &uuid,
                      SizeOta_Req,
                      CHAR_PROP_WRITE_WITHOUT_RESP,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE,
                      0x10,
                      CHAR_VALUE_LEN_CONSTANT,
                      &(CustomContext.CustomOta_ReqHdle));

    /*
     *          RECORDING_STREAM_SERVICE
     *
     * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property
     * service_max_attribute_record = 1 for RECORDING_STREAM_SERVICE +
     *                                2 for RECORDING_STREAM_CHAR +
     *                                1 for RECORDING_STREAM_CHAR configuration descriptor +
     *                              = 4
     */

    COPY_RECORDING_STREAM_SERVICE_UUID(uuid.Char_UUID_128);
    aci_gatt_add_service(UUID_TYPE_128,
                      (Service_UUID_t *) &uuid,
                      PRIMARY_SERVICE,
                      4,
                      &(CustomContext.CustomRec_Stream_SvcsHdle));

    /**
     *  RECORDING_STREAM_CHAR
     */
    COPY_RECORDING_STREAM_CHAR_UUID(uuid.Char_UUID_128);
    aci_gatt_add_char(CustomContext.CustomRec_Stream_SvcsHdle,
                      UUID_TYPE_128, &uuid,
                      SizeRec_Stream_Char,
                      CHAR_PROP_NOTIFY,
                      ATTR_PERMISSION_NONE,
                      GATT_DONT_NOTIFY_EVENTS,
                      0x10,
                      CHAR_VALUE_LEN_CONSTANT,
                      &(CustomContext.CustomRec_Stream_CharHdle));

/* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

/* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
/* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

/* USER CODE END Custom_STM_App_Update_Char_1 */

  switch(CharOpcode)
  {

    case CUSTOM_STM_SERIAL_CMD_CHAR:
      result = aci_gatt_update_char_value(CustomContext.CustomStim_Cmd_SvcsHdle,
                            CustomContext.CustomSerial_Cmd_CharHdle,
                            0, /* charValOffset */
                            SizeSerial_Cmd_Char, /* charValueLen */
                            (uint8_t *)  pPayload);
    /* USER CODE BEGIN CUSTOM_STM_SERIAL_CMD_CHAR*/

    /* USER CODE END CUSTOM_STM_SERIAL_CMD_CHAR*/
      break;

    case CUSTOM_STM_CMD_FB_CHAR:
      result = aci_gatt_update_char_value(CustomContext.CustomStim_Cmd_SvcsHdle,
                            CustomContext.CustomCmd_Fb_CharHdle,
                            0, /* charValOffset */
                            SizeCmd_Fb_Char, /* charValueLen */
                            (uint8_t *)  pPayload);
    /* USER CODE BEGIN CUSTOM_STM_CMD_FB_CHAR*/

    /* USER CODE END CUSTOM_STM_CMD_FB_CHAR*/
      break;

    case CUSTOM_STM_OTA_REQ:
      result = aci_gatt_update_char_value(CustomContext.CustomStim_Cmd_SvcsHdle,
                            CustomContext.CustomOta_ReqHdle,
                            0, /* charValOffset */
                            SizeOta_Req, /* charValueLen */
                            (uint8_t *)  pPayload);
    /* USER CODE BEGIN CUSTOM_STM_OTA_REQ*/

    /* USER CODE END CUSTOM_STM_OTA_REQ*/
      break;

    case CUSTOM_STM_REC_STREAM_CHAR:
      result = aci_gatt_update_char_value(CustomContext.CustomRec_Stream_SvcsHdle,
                            CustomContext.CustomRec_Stream_CharHdle,
                            0, /* charValOffset */
                            SizeRec_Stream_Char, /* charValueLen */
                            (uint8_t *)  pPayload);
    /* USER CODE BEGIN CUSTOM_STM_REC_STREAM_CHAR*/

    /* USER CODE END CUSTOM_STM_REC_STREAM_CHAR*/
      break;

    default:
      break;
  }

/* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

/* USER CODE END Custom_STM_App_Update_Char_2 */

  return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

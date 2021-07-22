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
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include <limits.h>

SemaphoreHandle_t bleSemaphore;

void genericEventHandler(uint32_t event, void *eventParameter) {
    cy_stc_ble_gatts_write_cmd_req_param_t *writeReqParameter;
    
    // Take an action based on current event
    switch (event) {
        case CY_BLE_EVT_STACK_ON:
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            printf("disconnected\r\n");
            Cy_GPIO_Write(P6_3_PORT, P6_3_PIN, 1);                           // RED is ON
            Cy_GPIO_Write(P7_1_PORT, P7_1_PIN, 0);                           // GREEN is OFF
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
            break;
        case CY_BLE_EVT_GATT_CONNECT_IND:
            printf("connected\r\n");
            Cy_GPIO_Write(P6_3_PORT, P6_3_PIN, 0);                           // RED is OFF
            Cy_GPIO_Write(P7_1_PORT, P7_1_PIN, 1);                           // GREEN is ON
            break;
        case CY_BLE_EVT_GATTS_WRITE_REQ:
            writeReqParameter = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParameter;
            uint8 command[4] = {0,0,0,0};
            
            if (CY_BLE_STIMULATION_COMMAND_SERVICE_SERIAL_COMMAND_INPUT_CHAR_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle) {
                uint8_t serial_command_input_char = writeReqParameter->handleValPair.value.val[0];
                command[0] = serial_command_input_char;
            }
            
            if (CY_BLE_STIMULATION_COMMAND_SERVICE_COMMAND_FEEDBACK_CHAR_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle) {
                uint8_t command_feedback_char = writeReqParameter->handleValPair.value.val[0];
                command[1] = command_feedback_char;
            }
            
            if (CY_BLE_STIMULATION_COMMAND_SERVICE_OTA_REQ_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle) {
                uint8_t ota_req = writeReqParameter->handleValPair.value.val[0];
                command[2] = ota_req;
            }
            
            if (CY_BLE_RECORDING_STREAM_SERVICE_RECORDING_STREAM_CHAR_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle) {
                uint8_t recording_stream_char = writeReqParameter->handleValPair.value.val[0];
                command[3] = recording_stream_char;
            }
            
            for (int i = 0; i < 4; i++) {
                printf("command[%d] = %d\r\n", i, command[i]);
            }
            
            
            Cy_BLE_GATTS_WriteRsp(writeReqParameter->connHandle);
            break;
        default:
            break;
    }
}

void bleInterruptNotify() {
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(bleSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void bleTask(void *arg) {
    (void) arg;
    printf("BLE Task Started\r\n");
    
    bleSemaphore = xSemaphoreCreateCounting(UINT_MAX, 0);
    
    Cy_BLE_Start(genericEventHandler);
    
    while (Cy_BLE_GetState() != CY_BLE_STATE_ON) {
        Cy_BLE_ProcessEvents();
    }
    
    Cy_BLE_RegisterAppHostCallback(bleInterruptNotify);
    
    for(;;) {
        xSemaphoreTake(bleSemaphore, portMAX_DELAY);
        Cy_BLE_ProcessEvents();
    }
}


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_1_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("System started\r\n");
    
    xTaskCreate(bleTask, "bleTask", 8*1024, 0, 2, 0);
    
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

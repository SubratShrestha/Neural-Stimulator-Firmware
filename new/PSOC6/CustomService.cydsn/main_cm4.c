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
#include "command_parser.h"
#include "command_queue.h"
#include "stdlib.h"
#include "main_cm4.h"

SemaphoreHandle_t bleSemaphore;

void genericEventHandler(uint32_t event, void *eventParameter) {
    cy_stc_ble_gatts_write_cmd_req_param_t *writeReqParameter;
    
    // Take an action based on current event
    switch (event) {
        case CY_BLE_EVT_STACK_ON:
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            printf("disconnected\n");
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
            break;
        case CY_BLE_EVT_GATT_CONNECT_IND:
            printf("connected\n");
            break;
        case CY_BLE_EVT_GATTS_WRITE_REQ:
            writeReqParameter = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParameter;
            
            if (CY_BLE_NS_NSCHARACTERISTIC_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle) {
                uint8_t *commands = writeReqParameter->handleValPair.value.val;
                
                
                
                for (int i=0; i < 5; i++) {
                    // please null check.
                    printf("command[%d] = %d\r\n", i, commands[i]);    
                }
                
                // error handling
                int array_size = (int)(sizeof(commands)/sizeof(commands[0]));
                if (array_size > 5 || array_size < 0) {
                    printf("invalid command");
                } else {
                    // passing to command parser
                    execute_command(commands);
                }
                
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

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

// Queue globals
static QueueHandle_t msg_queue;
static const uint8_t len = 5;

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

        case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
            writeReqParameter = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParameter;
            
            // Handle = 18, UUID = 0000fe41-8e22-4541-9d4c-21edae82ed19
            // this is one bytearray.
            uint8_t command[5];
            for (int i = 0; i < writeReqParameter->handleValPair.value.len; i++) {
                command[i] = writeReqParameter->handleValPair.value.val[i];
            }
            
            if (command[0] != 25) {
                if (xQueueSend(msg_queue, command, 0) != pdPASS) {
                    printf("queue full\r\n");
                    return;
                }
            }
            
            Cy_BLE_GATTS_WriteRsp(writeReqParameter->connHandle);
            break;
            
        case CY_BLE_EVT_GATTS_WRITE_REQ:
            writeReqParameter = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParameter;
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
        vTaskDelay(100);
    }
}

void dacTask(void *arg) {
    (void) arg;
    printf("DAC task started\r\n");
    
    for (;;) {
        uint8_t command[5];
        while(xQueueReceive(msg_queue, (void *)&command, 0) == pdPASS) {
            uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
            switch(command[0]) {
                case 0x01:
                    printf("start stimulation\r\n");
                    break;
                case 0x02:
                    printf("stop stimulation\r\n");
                    break;
                case 0x03:
                    printf("stim type = %s\r\n", value == 0 ? "uniform" : "burst");
                    break;
                case 0x04:
                    printf("anodic/cathodic = %s\r\n", value == 0 ? "anodic" : "cathodic");
                    break;
                case 0x05:
                    printf("phase 1 = %d\r\n", value);
                    
                    break;
                case 0x06:
                    printf("phase 2 = %d\r\n", value);
                    break;
                case 0x07:
                    printf("dac gap = %d\r\n", value);
                    break;
                case 0x08:
                    printf("phase 1 time = %d\r\n", value);
                    break;
                case 0x09:
                    printf("inter phase gap = %d\r\n", value);
                    break;
                case 0x0a:
                    printf("phase 2 time = %d\r\n", value);
                    break;
                case 0x0b:
                    printf("inter stim delay = %d\r\n", value);
                    break;
                case 0x0c:
                    printf("inter burst delay = %d\r\n", value);
                    break;
                case 0x0d:
                    printf("pulse number = %d\r\n", value);
                    break;
                case 0x0e:
                    printf("pulse number in one burst = %d\r\n", value);
                    break;
                case 0x0f:
                    printf("burst number = %d\r\n", value);
                    break;
                case 0x10:
                    printf("ramp up = %s\r\n", value == 0 ? "no" : "yes");
                    break;
                case 0x11:
                    printf("short electrode = %s\r\n", value == 0 ? "no" : "yes");
                    break;
                case 0x12:
                    printf("record frequency = %d\r\n", value);
                    break;
                case 0x13:
                    printf("start recording\r\n");
                    break;
                case 0x14:
                    printf("stop recording\r\n");
                    break;
                default:
                    break;
            }
        }
        
        if (xQueueIsQueueEmptyFromISR(msg_queue)) {
            printf("queue is empty, dac task going to sleep\r\n");
            vTaskDelay(5000);
        }
    }
    
}


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_1_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("System started\r\n");
    msg_queue = xQueueCreate(16, sizeof(uint8_t[5]));
    
    xTaskCreate(bleTask, "bleTask", 1024, NULL, 1, NULL);
    xTaskCreate(dacTask, "dacTask", 1024, NULL, 1, NULL);
    
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

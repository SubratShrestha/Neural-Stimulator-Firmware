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
#include <math.h>

// BLE globals
SemaphoreHandle_t bleSemaphore;

// Queue globals
static QueueHandle_t msg_queue;

// DAC globals
uint32_t dacWrite;
uint32_t comparevalue;
uint32_t phase_1_dac;
uint32_t phase_2_dac;
uint32_t dac_gap;
uint32_t phase_2_time;
uint32_t phases[4];
bool stim_type;
uint32_t num_pulses; /* could need to change to uint32_t based on how data format is sent/recieved */
uint32_t curr_num_pulses = 0;
int current_phase = 0;


// DAC timer interrupt handler
void TimerInterruptHandler(void)
{
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_CC);
    comparevalue = Cy_TCPWM_Counter_GetCounter(Timer_HW, Timer_CNT_NUM);
    
    if (current_phase == 0) {
        dacWrite = phase_1_dac;   
    } else if (current_phase == 1) {
        dacWrite = 0x000;   
    } else if (current_phase == 2) {
        dacWrite = phase_2_dac;
    } else if (current_phase == 3) {
        dacWrite = 0x000;  
    }
    VDAC_1_SetValue(dacWrite);
    
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);

    volatile float value = Cy_SAR_GetResult16(SAR, 0);
    volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);
    // printf("Value in V = %f\r\n", volts);
    
    while (1) {
        if (phases[current_phase] == 0) {
            if (current_phase == 3) {
                current_phase = 0;
                curr_num_pulses++;
            } else if (current_phase < 3) {
                current_phase++;
            }
        } else {
            break;
        }
    }
    
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[current_phase] - 1);
    
    if (current_phase == 3) {
        curr_num_pulses++;
        current_phase = 0;   
    } else {
        current_phase++;
    }
    
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    if (curr_num_pulses == num_pulses && num_pulses != 0) {
        Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);   
    }
}

// DAC Task
void dacTask(void *arg) {
    (void) arg;
    printf("DAC Task Started\r\n");
    
    VDAC_1_Start();
    ADC_Start();
    
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    
    /* Start the TCPWM component in timer/counter mode. The return value of the
    * function indicates whether the arguments are valid or not. It is not used
    * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */

    // parser
    for (;;) {
        uint8_t command[5];
        while(xQueueReceive(msg_queue, (void *)&command, 0) == pdPASS) {
            uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
            switch(command[0]) {
                case 0x01:
                    printf("start stimulation\r\n");                                  
                    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[0]);
                    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
                    break;
                case 0x02:
                    printf("stop stimulation\r\n");
                    Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
                    break;
                case 0x03:
                    printf("stim type = %s\r\n", value == 0 ? "uniform" : "burst");
                    stim_type = value;
                    break;
                case 0x05:
                    printf("phase 1 = %d\r\n", value) ;
                    //printf("phase 1 after scaling = %f\r\n", floor(((2047 - value) / 2047) * 4095)) ;
                    //printf("phase 1 after scaling with bitwise = %f\r\n", floor(value & value)) ;
                    //printf("phase 1 = 0x%x\r\n", ((2047 - value) / 2047) * 4095);
                    
                    phase_1_dac = value;
                    break;
                case 0x06:
                    printf("phase 2 = %d\r\n", value) ;
                    //printf("phase 2 after scaling = %f\r\n", floor(((value - 2047) / 2047) * 4095));
                    //printf("phase 2 = 0x%x\r\n", ((value - 2047) / 2047) * 4095);
                    
                    phase_2_dac = value;
                    break;
                case 0x07:
                    printf("dac gap = %d\r\n", value);
                    dac_gap = value;
                    break;
                case 0x08:
                    printf("phase 1 time = %d\r\n", value);
                    phases[0] = value;
                    break;
                case 0x09:
                    printf("inter phase gap = %d\r\n", value);
                    phases[1] = value;
                    break;
                case 0x0a:
                    printf("phase 2 time = %d\r\n", value);
                    phases[2] = value;
                    break;
                case 0x0b:
                    printf("inter stim delay = %d\r\n", value);
                    phases[3] = value;
                    break;
                case 0x0d:
                    printf("number of pulses = %d\r\n", value);
                    num_pulses = value;
                    break;
                default:
                    break;
            }
        }
        
        vTaskDelay(5000);
    }
}

// BLE event handler
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



int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_1_Start();
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("System started\r\n");
    msg_queue = xQueueCreate(20, sizeof(uint8_t[5]));
    
    xTaskCreate(bleTask, "bleTask", 1024, 0, 1, 0);
    xTaskCreate(dacTask, "dacTask", 1024, 0, 1, 0);
    
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

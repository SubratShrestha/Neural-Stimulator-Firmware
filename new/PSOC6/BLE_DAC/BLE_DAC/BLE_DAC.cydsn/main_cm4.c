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

// Internal switching
#define STIM_ENABLE P9_3_PORT
#define DUMMY_LOAD  P9_0_PORT
#define SHORT_ELECTRODE P9_1_PORT
#define TOGGLE_OUTPUT P9_2_PORT

#define STIM_ENABLE_NUM P9_3_NUM
#define DUMMY_LOAD_NUM  P9_0_NUM
#define SHORT_ELECTRODE_NUM P9_1_NUM
#define TOGGLE_OUTPUT_NUM P9_2_NUM

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
uint32_t phases[4];// 0 = phase 1 time, 1 = interphase gap, 2 = phase 2 time, 3 = interstim gap

uint32_t original_inter_stim_gap;

bool stim_type;
uint32_t num_pulses = 0; /* could need to change to uint32_t based on how data format is sent/recieved */
uint32_t curr_num_pulses = 0;
uint32_t curr_num_bursts = 0;
int current_phase = 0;
// Following are burst mode globals
uint32_t inter_burst_delay;
uint32_t pulse_num_in_one_burst;
uint32_t burst_num;
bool ramp_up;
int ramp_up_count1 = 10;
int ramp_up_count2 = 10;
bool finished = false;


// stim modes
uint32_t anodic_cathodic;

// DAC timer interrupt handler - called each time the timer reaches the next phase
void TimerInterruptHandler(void)
{   

    
    //function will always ready the values for the DAC according to the phase
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_CC);
    comparevalue = Cy_TCPWM_Counter_GetCounter(Timer_HW, Timer_CNT_NUM);
    
    uint32_t dac_val_1;
    uint32_t dac_val_2;
    if (anodic_cathodic) {
        // negative pulse first
        dac_val_1 = phase_2_dac;
        dac_val_2 = phase_1_dac;
    } else {
        // default positive
        dac_val_1 = phase_1_dac;
        dac_val_2 = phase_2_dac;
    }
    
    if (current_phase == 0) {//phase 1
        if (ramp_up) {
            ramp_up_count1 ++;
            if (ramp_up_count1 == 11) {
                //printf("This code is called and ramp up is %d\r\n", ramp_up);
                ramp_up_count1 = 1;
            }
        }
        Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM,0);
        Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 1);
        dacWrite = (ramp_up_count1 * dac_val_1)/10; 
        //printf("The value the dac was set in phase 1 was %d and ramp_up_count was %d \r\n", dacWrite, ramp_up_count1);
    } else if (current_phase == 1) {//interphase gap
        Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 0);
        Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 0);
        dacWrite = 0x800;   
    } else if (current_phase == 2) {//phase 2
        if (ramp_up) {
            ramp_up_count2 ++;
            if (ramp_up_count2 == 11) {
                ramp_up_count2 = 1;
            }
        }
        Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 0);
        Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 1);
        dacWrite = (ramp_up_count2 * dac_val_2)/10;
        //printf("The value the dac was set in phase 2 was %d \r\n", dacWrite);
    } else if (current_phase == 3) {//inter stim gap (changes if we get inbetween bursts)
        
        phases[3] = original_inter_stim_gap;
        if (curr_num_pulses % pulse_num_in_one_burst == 0) {//if one burst is finished
            phases[3] = inter_burst_delay;
            //printf("inside phase 3 the curr_num_pulses was %d and the pulse num in one burst was %d and the interstim gap was %d\r\n", curr_num_pulses,pulse_num_in_one_burst, phases[3]);
        }
        if (curr_num_pulses % pulse_num_in_one_burst == 0 && !finished) {
            curr_num_bursts ++;
        }
        Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 1);
        Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 0);
        //printf("The delay was %d \r\n", phases[3]);
        dacWrite = 0x800;  
    }
    


    
    
    
    //Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 1);
    //Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 0);
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT); //ADC

    volatile float value = Cy_SAR_GetResult16(SAR, 0);//ADC
    volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);//ADC
    // printf("Value in V = %f\r\n", volts);
    
    if (num_pulses == 0) {
        //printf("The number of pulses in continuous mode is %d\r\n", curr_num_pulses);
        VDAC_1_SetValue(dacWrite);
    }   else if (curr_num_pulses <= num_pulses) {
        //printf("The number of pulses in burst mode is %d\r\n", curr_num_pulses);
        VDAC_1_SetValue(dacWrite);
    }
    
    
    //handles 0 values for time periods so moves to next phase if value is 0
    while (1) {
        if (phases[current_phase] == 0) {
            if (current_phase == 3) {
                current_phase = 0;
            } else if (current_phase < 3) {
                current_phase++;
            }
        } else {
            break;
        }
    }
    
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[current_phase] - 1);
    
    if (current_phase == 3 && !finished) {
        curr_num_pulses++;
        current_phase = 0;   
    } else {
        current_phase++;
    }
    
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    if (curr_num_pulses > num_pulses && num_pulses != 0 && stim_type == 0) {
        printf("The number of pulses as required was %d (killed in if)\r\n", curr_num_pulses);
        Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
        finished = true;
        curr_num_pulses = 0;
        curr_num_bursts = 0;
    }   else if (curr_num_bursts > burst_num && burst_num != 0) {
        Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
        printf("The number of pulses as required was %d (killed in else if) \r\n", curr_num_pulses);
        finished = true;
        curr_num_pulses = 0;
        curr_num_bursts = 0;
    }//   else if (curr_num_pulses > num_pulses && num_pulses != 0) {
     //   Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
    //}
    if (finished) {
        curr_num_pulses = 0;
        curr_num_bursts = 0;
    }
    // printf("curr_num_pulses = %d, num_pulses = %d, stim_type = %d\r\n", curr_num_pulses, num_pulses, stim_type);
    // printf("curr_num_bursts = %d, burst_num = %d\r\n", curr_num_bursts, burst_num);

    
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
        //gets values from BLE and sets them accordingly
        while(xQueueReceive(msg_queue, (void *)&command, 0) == pdPASS) {
            uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
            switch(command[0]) {
                case 0x01://start command
                    printf("start stimulation\r\n");                                  
                    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[0]);
                    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
                    break;
                case 0x02://stop command
                    printf("stop stimulation\r\n");
                    Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
                    break;
                case 0x03://stim_type 0: uniform 1:burst
                    printf("stim type = %s\r\n", value == 0 ? "uniform" : "burst");
                    stim_type = value;
                    break;
                case 0x04://anodic_cathodic 0:anodic (phase 1 is negative) 1:cathodic (phase 1 is positive)
                    printf("anodic is 0 cathodic is 1, we got %d\r\n", (uint32_t) value) ;
                    anodic_cathodic = value;
                    // swap timing periods in the phases array
                    // based on the assumption that the phases array is filled before this command is called
                    // may need to reimplement.
                    if (anodic_cathodic) {
                        uint32_t tmp = phases[0];
                        phases[0] = phases[2];
                        phases[2] = tmp;
                    }
                    break;
                case 0x05://dac_phase_one (0-4095)
                    printf("phase 1 = %d\r\n", value) ;
                    //printf("phase 1 after scaling = %f\r\n", floor(((2047 - value) / 2047) * 4095)) ;
                    //printf("phase 1 after scaling with bitwise = %f\r\n", floor(value & value)) ;
                    //printf("phase 1 = 0x%x\r\n", ((2047 - value) / 2047) * 4095);
                    
                    phase_1_dac = value;
                    break;
                case 0x06://dac_phase_two (0-4095)
                    printf("phase 2 = %d\r\n", value) ;
                    //printf("phase 2 after scaling = %f\r\n", floor(((value - 2047) / 2047) * 4095));
                    //printf("phase 2 = 0x%x\r\n", ((value - 2047) / 2047) * 4095);
                    
                    phase_2_dac = value;
                    break;
                case 0x07://dac_gap (0-4095)
                    printf("dac gap = %d\r\n", value);
                    dac_gap = value;
                    break;
                case 0x08://phase_one_time
                    printf("phase 1 time = %d\r\n", value);
                    phases[0] = value;
                    break;
                case 0x09://inter_phase_gap
                    printf("inter phase gap = %d\r\n", value);
                    phases[1] = value;
                    break;
                case 0x0a://phase_two_time
                    printf("phase 2 time = %d\r\n", value);
                    phases[2] = value;
                    break;
                case 0x0b://inter_stim_delay
                    printf("inter stim delay = %d\r\n", value);
                    phases[3] = value;
                    original_inter_stim_gap = value;
                    break;
                case 0x0c://inter_burst_delay
                    inter_burst_delay = value;
                    printf("The interburst delay is %d\r\n", value);
                    break;
                case 0x0d://pulse_num
                    printf("number of pulses = %d\r\n", value);
                    num_pulses = value;
                    finished = false;//restart counting number of pulses
                    break;
                case 0x0e://pulse_num_in_one_burst
                    printf("number of pulses in one burst = %d\r\n", value);
                    pulse_num_in_one_burst = value;
                    break;
                case 0x0f://burst_num
                    printf("number of bursts = %d\r\n", value);
                    burst_num = value;
                    pulse_num_in_one_burst = pulse_num_in_one_burst/value;
                    printf("THE ACTUAL number of pulses in one burst = %d\r\n", pulse_num_in_one_burst);
                    break;
                case 0x10://ramp_up
                    printf("ramp up = %d\r\n", value);
                    ramp_up = value;
                    break;
                case 0x11://short_electrode
                    Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 1);
                    break;
                case 0x12://record_freq
                    
                    break;
                case 0x13://start_recording
                    
                    break;
                case 0x14://stop_recording
                    
                    break;
                case 0x15://electrode_voltage
                    
                    break;
                case 0x16://elec_offset
                    
                    break;
                case 0x17://show_dac
                    
                    break;
                case 0x18://return_idle
                    
                    break;
                case 0x19://check_state
                    
                    break;
                default:
                    break;
            }
        }
        
        vTaskDelay(5000);
    }
}

//Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 1);
//Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 0);


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
    
    // default states
    Cy_GPIO_Write(STIM_ENABLE, STIM_ENABLE_NUM, 1);//set stim enable to high 
    Cy_GPIO_Write(DUMMY_LOAD, DUMMY_LOAD_NUM, 0);//set dummy load to low
    Cy_GPIO_Write(SHORT_ELECTRODE, SHORT_ELECTRODE_NUM, 1);//set short electrode to high 
    Cy_GPIO_Write(TOGGLE_OUTPUT, TOGGLE_OUTPUT_NUM, 0);//set toggle output to low
   
    xTaskCreate(bleTask, "bleTask", 1024, 0, 1, 0);
    xTaskCreate(dacTask, "dacTask", 1024, 0, 1, 0);
    
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

#ifndef main_h
#define main_h
#include "ble_comms.h"
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include <esp32/rom/ets_sys.h>
#include <sys/time.h>
#include "driver/gpio.h"
#include "driver/dac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "recording.h"
#include "battery.h"
#include "utils.h"

#define DEVICE_NAME "NeuroStimulator"

uint8_t DEBUG_MODE_ENABLED;

uint8_t CHANNEL_NUM;
uint32_t MAX_FREQ;

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

bool calibrated;
uint32_t VREF_0; //in mV
uint32_t VREF_255;//in mV

//for debug purpose
uint8_t DAC_PHASE_ONE;
uint8_t DAC_PHASE_TWO;

TaskHandle_t STIM_TASK;
bool STIM_STATUS; //used to delete task
bool STIM_TASK_STATUS; //0 IDLE; 1 WORKING

void STIM_START();
void STIM_STOP();
void biphasic_loop_infinity();
void biphasic_loop_count();
void burst_biphasic_loop_infinity();
void burst_biphasic_loop_count();
#endif

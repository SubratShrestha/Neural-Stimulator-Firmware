#ifndef battery_h
#define battery_H
#include "main.h"
#include "driver/i2c.h"
#include "freertos/timers.h"

#define SDA_GPIO 21
#define SCL_GPIO 22
#define MAX17043_ADDRESS 0x36
#define REG_ADDRESS 0x04

i2c_cmd_handle_t cmd_handle;

bool i2c_connection_status; //0 is ok; -1 is failed

uint8_t BATTERY_LEVEL;
TickType_t BATTERY_UPDATE_TIME_INTERVAL; //in millisecond

/*
 * a timer handler which is used to update battery level
 */
xTimerHandle battery_update_timer_handler;

int battery_init(void);
void battery_update(void);

#endif
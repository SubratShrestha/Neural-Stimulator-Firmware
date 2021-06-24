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

#define LED_ON 0
#define LED_OFF 1

// to lock/unlock the BLE Task
SemaphoreHandle_t bleSemaphore;


void genericEventHandler(uint32_t event, void *eventParameter) {
    (void) eventParameter;
    switch(event) {
        case CY_BLE_EVT_STACK_ON:
            printf("Stack started\r\n");
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;
            
        case CY_BLE_EVT_GATT_CONNECT_IND:
            printf("Connected\r\n");
            Cy_GPIO_Write(P7_1_PORT, P7_1_PIN, LED_ON);   // GREEN LED ON: port=P7_1_PORT, pin=P7_1_PIN
        break;
            
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            printf("Disconnected\r\n");
            Cy_GPIO_Write(P7_1_PORT, P7_1_PIN, LED_OFF);   // GREEN LED OFF
            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
        break;
            
        default:
            break;
    }
}

void iasEventHandler(uint32_t eventCode, void *eventParam) {
    (void) eventParam;
    uint8_t alertLevel;
    
    if (eventCode == CY_BLE_EVT_IASS_WRITE_CHAR_CMD) {
        // Read updated Alert Level from GATT database
        Cy_BLE_IASS_GetCharacteristicValue(CY_BLE_IAS_ALERT_LEVEL, sizeof(alertLevel), &alertLevel);
        
        switch(alertLevel) {
            case CY_BLE_NO_ALERT:
                printf("No Alert\r\n");
            break;
                
            case CY_BLE_MILD_ALERT:
                printf("Mild Alert\r\n");
            break;
        }
    }
}


int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

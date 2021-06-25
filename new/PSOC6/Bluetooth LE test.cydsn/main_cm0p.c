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

// variable to store the data
static uint8 data[1] = {0};


// Bluetoothe works:
// Whenever an event happens there is an event handler,
// event handler decides what to do based on the bluetooth event
void genericEventHandler(uint32_t event, void *eventParameter)
{
    switch(event)
    {
        // Bluetooth boots up and is ready to start operating
        // Bluetooth is currently not connected ot anything
        case CY_BLE_EVT_STACK_ON:
        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
        {
            // Start advertising
            

            Cy_BLE_GAPP_StartAdvertisement(CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX);
            break;
        }
        // When bluetooth connects to phone
        case CY_BLE_EVT_GATT_CONNECT_IND:
        {
            // can add code here to make it do something
            // eg can turn on an LED / run a piece of code
            
            for(;;)
            {
                Cy_GPIO_Write(BLINKYMF_PORT, BLINKYMF_NUM, 1);
                CyDelay(500);
                Cy_GPIO_Write(BLINKYMF_PORT, BLINKYMF_NUM, 0);
            }
            break;   
        }
        // Triggers when smartphone tries to write characteristic created by inbound
        case CY_BLE_EVT_GATTS_WRITE_CMD_REQ:
        {
            cy_stc_ble_gatts_write_cmd_req_param_t *writeReqParameter = (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParameter;
            
            if(CY_BLE_DEVICE_INTERFACE_DEVICE_INBOUND_CHAR_HANDLE == writeReqParameter->handleValPair.attrHandle)
            {
                data[0] = writeReqParameter->handleValPair.value.val[0];
                Cy_BLE_GATTS_WriteRsp(writeReqParameter->connHandle);
            }
            break;
        }
    }
}

// Called anytime bluetoothe needs the processer to do something
void bleInterruptNotify()
{
    Cy_BLE_ProcessEvents();   
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // Starting BLE block (similar to how we start PWM)
    
    Cy_BLE_Start(genericEventHandler);
    
    while(Cy_BLE_GetState() != CY_BLE_STATE_ON)
    {
        Cy_BLE_ProcessEvents();   
    }
    Cy_BLE_RegisterAppHostCallback(bleInterruptNotify);

    for(;;)
    {
        /* Place your application code here. */
        
        // taking whatever is in data and increment it by 1
        // then writing it to our outbound characteristic
        
        cy_stc_ble_gatt_handle_value_pair_t serviceHandle;
        cy_stc_ble_gatt_value_t serviceData;
        
        serviceData.val = (uint8*)data;
        serviceData.len = 1;
        
        serviceHandle.attrHandle = CY_BLE_DEVICE_INTERFACE_DEVICE_OUTBOUND_CHAR_HANDLE;
        serviceHandle.value = serviceData;
        
        Cy_BLE_GATTS_WriteAttributeValueLocal(&serviceHandle);
        
        data[0]++;
        CyDelay(1000);
    }
}

/* [] END OF FILE */

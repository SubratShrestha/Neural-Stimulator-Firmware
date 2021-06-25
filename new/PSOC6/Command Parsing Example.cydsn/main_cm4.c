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
#include "stdio.h"

void execute_command(void *PvParameter){
    uint8_t *command = (uint8_t*) PvParameter;
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case 0x01:
            Cy_GPIO_Write(YELLOW_PORT, YELLOW_NUM, 1);
            vTaskDelay(500);
            Cy_GPIO_Write(YELLOW_PORT, YELLOW_NUM, 0);
            vTaskDelay(500);
			break;
		case 0x02:
            // testing multitasking capabilities
            
			break;
		case 0x03:
			break;
		case 0x04:
			break;
		case 0x05:
			break;
		case 0x06:
			break;
		case 0x07:
			break;
		case 0x08:
			break;
		case 0x09:
			break;
		case 0x0a:
			break;
		case 0x0b:
			break;
		case 0x0c:
			break;
		case 0x0d:
			break;
		case 0x0e:
			break;
		case 0x0f:
			break;
		case 0x10:
			break;
		case 0x11:
			break;
		case 0x12:
			break;
		case 0x13:
			break;
		case 0x14:
			break;
		case 0x15:
			break;
		case 0x16:
			break;
		case 0x17:
			break;
		case 0x18:
			break;
		case 0x19:
			break;
		default:break;
	}
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    // may need to change stack size for now will leave at 400
    // https://www.freertos.org/FAQMem.html#StackSize
    uint8_t dummy1[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    // uint8_t dummy2[5] = {0x02, 0x02, 0x03, 0x04, 0x05};
    UART_Start();
    setvbuf(stdin, NULL,_IONBF,0);
    xTaskCreate(execute_command, "Executes dummy1", 400, (void *) dummy1, 1,0);
    // xTaskCreate(execute_command, "Executes dummy2", 400, (void *) dummy2, 1,0);
    vTaskStartScheduler();
    printf("Tasks Complete :)\n");
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

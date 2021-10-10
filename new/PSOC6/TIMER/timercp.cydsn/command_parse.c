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

#include "command_parse.h"

void execute_command(uint8_t *command){
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case 0x01:
			//check whether stimulation has been started
			
			break;
		case 0x02:
			
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
		{
			
			break;
		}
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



/* [] END OF FILE */

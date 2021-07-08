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
#include "command_parser.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <string.h>
#include "supported_commands.h"

void execute_command(uint8_t *command){
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case START:
            printf("Command 1 executed... Starting stimulation");
            
            break;
        case STOP:
            // testing multitasking capabilities
            printf("Command 2 executed... Stopping stimulation");
			break;
		case STIM_TYPE:
            printf("Command 3 executed... set stimulation type");
			break;
		case ANODIC_CATHODIC:
            // anodic = 0, cathodic = 1
            printf("Command 4 executed... Anodic selected....");
			break;
		case DAC_PHASE_ONE:
            printf("Command 5 executed");
			break;
		case DAC_PHASE_TWO:
            printf("Command 6 executed");
			break;
		case DAC_GAP:
            printf("Command 7 executed");
			break;
		case PHASE_ONE_TIME:
            printf("Command 8 executed");
			break;
		case INTER_PHASE_GAP:
			break;
		case PHASE_TWO_TIME:
			break;
		case INTER_STIM_DELAY:
			break;
		case INTER_BURST_DELAY:
			break;
		case PULSE_NUM:
			break;
		case PULSE_NUM_IN_ONE_BURST:
			break;
		case BURST_NUM:
			break;
		case RAMP_UP:
			break;
		case SHORT_ELECTRODE:
			break;
		case RECORD_FREQ:
			break;
		case START_RECORDING:
			break;
		case STOP_RECORDING:
			break;
		case ELECTRODE_VOLTAGE:
			break;
		case ELEC_OFFSET:
			break;
		case SHOW_DAC:
			break;
		case RETURN_IDLE:
			break;
		case CHECK_STATE:
			break;
		default:break;
	}
}

/* [] END OF FILE */
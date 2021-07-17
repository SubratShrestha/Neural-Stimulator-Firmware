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
#include "main_cm4.h"

void execute_command(uint8_t *command){
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case START:
        // points: 50
            printf("Command 1 executed... Starting stimulation\n");
            // update global variables in cm4_main
            
            // handle phase 1 times (min 10)
            
            // handle phase 2 times (min 10)
            
            // handle inter phase gap time (default 5)
            
            // handle inter burst delay (min 50)
            
            // calculate timers
            
            // choose stim type
            
            // update stim status
            
            // ramp ups?
            
            // write data to dac
            
            // timers and iterrupts
            
            // write to led to indicate start of stimulation
            
            break;
        case STOP:
            // points 10
            // testing multitasking capabilities
            printf("Command 2 executed... Stopping stimulation");
			break;
		case STIM_TYPE:
            // points 2
            printf("Command 3 executed... set stimulation type");
			break;
		case ANODIC_CATHODIC:
            // points 2
            // anodic = 0, cathodic = 1
            printf("Command 4 executed... Anodic selected....");
			break;
		case DAC_PHASE_ONE:
            // points 2
            printf("Command 5 executed");
			break;
		case DAC_PHASE_TWO:
            // points 2
            printf("Command 6 executed");
			break;
		case DAC_GAP:
            // points 2
            printf("Command 7 executed");
			break;
		case PHASE_ONE_TIME:
            // points 2
            printf("Command 8 executed");
			break;
		case INTER_PHASE_GAP:
            // points 2
			break;
		case PHASE_TWO_TIME:
            // points 2
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
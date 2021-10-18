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

#include "main.h"
#include "command_parse.h"

void execute_command(uint8_t *command){
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case 0x01: // start stimulation
        
			//check whether stimulation has been started

				// enable stim cct

				// make sure all param are satisfy requirememt

				// init temp value for pulse num/burst num
				

				// choose stim mode by given parameters
				

				// init stim state to phase one
				

				// is ramp up?
				

				// WRITE DATA TO DAC
				
				// clear timer2 cnt and enable timer2 with interrupts
				
				// use PB0(LED2) to indicate the start ot stimulation
				
			break;
		case 0x02: // stop stimulation
			
			break;
		case 0x03: // stim type: 0 uniform, 1 burst
			stim_type = value;
			break;
		case 0x04: // anodic/cathodic: 0 anodic,  1 cathodic
			anodic_cathodic = value;
			break;
		case 0x05: // set dac phase 1
			dac_phase_one = value;
			break;
		case 0x06: // set dac phase 2
			dac_phase_two = value;
			break;
		case 0x07: // set dac gap
			dac_gap = value;
			break;
		case 0x08: // set phase one length
			phase_1 = value;
			break;
		case 0x09: // set interphase length
			inter_stim_delay = value;
			break;
		case 0x0a: // set phase 2 length
			phase_2 = value;
			break;
		case 0x0b: // set interstim delay length
			inter_stim_delay = value;
			break;
		case 0x0c: // set inter_burst_delay
			inter_burst_delay = value;
			break;
		case 0x0d: // set number of pulses
			num_pulses = value;
			break;
		case 0x0e: // set number of pulses per burst
			
			break;
		case 0x0f: // set number of bursts to stimulate
			
			break;
		case 0x10: // ramp_up 0: no, 1: yes
			
			break;
		case 0x11: // short electrod 0: no, 1:yes (depracted)
			
			break;
		case 0x12: // set recording frequency
			
			break;
		case 0x13: // start neural recording
			
			break;
		case 0x14: // start neural recording // get last recorded electrode voltage
			
			break;
		case 0x15: // get last recorded electrode voltage
		{
			
			break;
		}
		case 0x16: // get electrode voltage before end of phase one
			
			break;
		case 0x17: // show dac
			
			break;
		case 0x18: // returns idle deprecated
			
			break;
		case 0x19: // check state: 1 idle, 2 running;
			
			break;
		default:break;
	}
}



/* [] END OF FILE */

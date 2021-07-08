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
#ifndef supported_commands_h
#define supported_commands_h
    
    // Stimulation specific commands
    #define START 0x01
    #define STOP 0x02
    #define STIM_TYPE 0x03
    #define ANODIC_CATHODIC 0x04
    #define DAC_PHASE_ONE 0x05
    #define DAC_PHASE_TWO 0x06
    #define DAC_GAP 0x07
    #define PHASE_ONE_TIME 0x08
    #define INTER_PHASE_GAP 0x09
    #define PHASE_TWO_TIME 0x0a
    #define INTER_STIM_DELAY 0x0b
    #define INTER_BURST_DELAY 0x0c
    #define PULSE_NUM 0x0d
    #define PULSE_NUM_IN_ONE_BURST 0x0e
    #define BURST_NUM 0x0f
    
    
    #define RAMP_UP 0x10 // yes or no (1 or 0)
    #define SHORT_ELECTRODE 0x11 // yes or no (1 or 0)
    #define RECORD_FREQ 0x12
    
    // recording commands
    #define START_RECORDING 0x13
    #define STOP_RECORDING 0x14
    
    // Commands used to get information for the following fields
    #define ELECTRODE_VOLTAGE 0x15
    #define ELEC_OFFSET 0x16
    #define SHOW_DAC 0x17
    #define RETURN_IDLE 0x18
    
    // 1 is idle, 2 is running, can be used to check connection
    #define CHECK_STATE 0x19

#endif
/* [] END OF FILE */

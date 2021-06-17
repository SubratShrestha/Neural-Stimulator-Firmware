#include "command_parse.h"
#include "stm32wb55xx.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
extern LPTIM_HandleTypeDef hlptim2;
extern ADC_HandleTypeDef hadc1;

void execute_command(uint8_t *command){
	uint32_t value = (command[1]<<24) + (command[2]<<16) + (command[3]<<8) + command[4];
	switch (command[0]){
		case 0x01:
			//check whether stimulation has been started
			if(HAL_HSEM_Take(STIM_HSEM_ID, STIM_HSEM_PROCESS_ID)==HAL_OK){

				//enable stim cct
				HAL_GPIO_WritePin(STIM_EN_GPIO_Port, STIM_EN_Pin, SET);


				//make sure all param are satisfy requirememt
				if(PHASE_ONE_TIME < 9){
					PHASE_ONE_TIME = 10;
				}

				if(PHASE_TWO_TIME < 9){
					PHASE_ONE_TIME = 10;
				}

				if(INTER_PHASE_GAP < 5 && INTER_PHASE_GAP != 0){
					INTER_PHASE_GAP = 5;
				}

				if(INTER_STIM_DELAY < 5 && INTER_STIM_DELAY != 0){
					INTER_STIM_DELAY = 5;
				}

				if(INTER_BURST_DELAY < 50){
					INTER_BURST_DELAY = 50;
				}



				PHASE_ONE_TIMER = 4 * (PHASE_ONE_TIME - 1);
				PHASE_TWO_TIMER = 4 * (PHASE_TWO_TIME - 1);
				PHASE_GAP_TIMER = 4 * INTER_PHASE_GAP;
				STIM_DELAY_TIMER = 4 * INTER_STIM_DELAY;
				BURST_DELAY_TIMER = 4 * (INTER_BURST_DELAY - 1);

				PHASE_ONE_SAMPLE_TIMER = PHASE_ONE_TIMER - ELEC_OFFSET;

				//init temp value for pulse num/burst num
				TEMP_PULSE_NUM = PULSE_NUM;
				TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
				TEMP_BURST_NUM = BURST_NUM;

				//choose stim mode by given parameters
				if(STIM_TYPE == 0){
					if(PULSE_NUM == 0){
						STIM_MODE = STIM_MODE_UNI_CONT;
					}else{
						STIM_MODE = STIM_MODE_UNI_NUM;
					}
				}else{
					if(BURST_NUM == 0){
						STIM_MODE = STIM_MODE_BURST_CONT;
					}else{
						STIM_MODE = STIM_MODE_BURST_NUM;
					}
				}

				//init stim state to phase one
				STIM_STATUS = STIM_STATUS_PHASE_ONE;

				//is ramp up?
				if(RAMP_UP){
					if(DAC_PHASE_ONE < DAC_PHASE_TWO){
						ANODIC_CATHODIC = 1;
					}else{
						ANODIC_CATHODIC = 0;
					}

					if(ANODIC_CATHODIC){
						DAC_PHASE_ONE_COMP = DAC_PHASE_ONE + 110;
						DAC_PHASE_TWO_COMP = DAC_PHASE_TWO - 110;
					}else{
						DAC_PHASE_ONE_COMP = DAC_PHASE_ONE - 110;
						DAC_PHASE_TWO_COMP = DAC_PHASE_TWO + 110;
					}
					TEMP_DAC_PHASE_ONE = DAC_GAP;
					TEMP_DAC_PHASE_TWO = DAC_GAP;
					TEMP_DAC_GAP = DAC_GAP;
					HAL_LPTIM_Counter_Start_IT(&hlptim2, 50000);
				}else{
					TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
					TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
					TEMP_DAC_GAP = DAC_GAP;
				}


				//WRITE DATA TO DAC
				while((SPI1->SR & 2) == 0);
				SPI1->DR = TEMP_DAC_PHASE_ONE;

				//clear timer2 cnt and enable timer2 with interrupts
				TIM2->CNT = 0;
				TIM2->CCER |= 1;
				HAL_TIM_Base_Start_IT(&htim2);

				//use PB0(LED2) to indicate the start ot stimulation
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
			}
			break;
		case 0x02:
			if(HAL_HSEM_IsSemTaken(STIM_HSEM_ID)){
				while(PULSE_PROBE != 0);
				STIM_STATUS = STIM_STATUS_STOP;
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
			}
			break;
		case 0x03:
			STIM_TYPE = value;
			break;
		case 0x04:
			ANODIC_CATHODIC = value;
			break;
		case 0x05:
			DAC_PHASE_ONE = value;
			break;
		case 0x06:
			DAC_PHASE_TWO = value;
			break;
		case 0x07:
			DAC_GAP = value;
			break;
		case 0x08:
			PHASE_ONE_TIME = value;
			break;
		case 0x09:
			INTER_PHASE_GAP = value;
			break;
		case 0x0a:
			PHASE_TWO_TIME = value;
			break;
		case 0x0b:
			INTER_STIM_DELAY = value;
			break;
		case 0x0c:
			INTER_BURST_DELAY = value;
			break;
		case 0x0d:
			PULSE_NUM = value;
			break;
		case 0x0e:
			PULSE_NUM_IN_ONE_BURST = value;
			break;
		case 0x0f:
			BURST_NUM = value;
			break;
		case 0x10:
			RAMP_UP = value;
			break;
		case 0x11:
			SHORT_ELECTRODE = value;
			break;
		case 0x12:
			RECORD_FREQ = value;
			break;
		case 0x13:
			if(ENABLE_RECORD == 0 && HAL_TIM_Base_GetState(&htim1) == HAL_TIM_STATE_READY){
				TIM1->ARR = 100000 / RECORD_FREQ;
				HAL_TIM_Base_Start(&htim1);
				ENABLE_RECORD = 1;
			}
			break;
		case 0x14:
			if(ENABLE_RECORD && HAL_TIM_Base_GetState(&htim1) == HAL_TIM_STATE_BUSY){
				HAL_TIM_Base_Stop(&htim1);
				ENABLE_RECORD = 0;
			}
			break;
		case 0x15:
		{
			uint16_t adc_data = (uint16_t)ADC1->JDR1;
			command[3] = adc_data >> 8;
			command[4] = adc_data & 0xff;
			break;
		}
		case 0x16:
			ELEC_OFFSET = value;
			break;
		case 0x17:
			SPI1->DR = value;
			for(int x=0;x<2;x++);//short delay
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
			break;
		case 0x18:
			SPI1->DR = DAC_GAP;
			for(int x=0;x<2;x++);//short delay
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
			break;
		case 0x19:
			if(STIM_STATUS == STIM_STATUS_STOP){
				command[4] = 1;
			}else{
				command[4] = 2;
			}
			break;
		default:break;
	}
}

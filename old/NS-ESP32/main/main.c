#include "main.h"
#include "sys/queue.h"
#include "soc/sens_reg.h"
#include "soc/rtc_io_reg.h"

/**
 * GPIO 5,18,19,23 will be used to SPI comms; 5:cs;18:sck;19:miso;23:mosi
 * GPIO 21,22 will be used as I2C to read battery lvl
 * GPIO 25,26 will be used as DACs
 * GPIO 2,4,16,17 will be used as switches
*/

void app_main(){ // runs in cpu0
    
    
    //comment this if you want esp32 store ssid/password
    //clear_wifi_config();

    //enable debug mode here
    DEBUG_MODE_ENABLED = 1;
    DAC_PHASE_ONE = 0;
    DAC_PHASE_TWO = 255;

    calibrated = 1;
    VREF_0 = 82; 
    VREF_255 = 3180;

    BATTERY_UPDATE_TIME_INTERVAL = 10000; //10 second
    BATTERY_LEVEL = 0;
    CHANNEL_NUM = 1;
    MAX_FREQ = 100000;//10KHZ
    PHASE_ONE_TIME = 10;// default 10us
    PHASE_TWO_TIME = 10;// default 10us
    STIM_AMP = 2000;// default 0uA
    INTER_PHASE_GAP = 0;//default 0us
    INTER_STIM_DELAY = 100;//default 0us
    ANODIC_CATHODIC = 1;//default cathodic
    STIM_TYPE = 0;//default uniform stim
    PULSE_NUM = 0;//default 0 is forever in ms
    BURST_NUM = 0;// number of burst
    INTER_BURST_DELAY = 0;
    PULSE_NUM_IN_ONE_BURST = 0;
    RAMP_UP = 0;
    SHORT_ELECTRODE = 1;

    STIM_STATUS = 0;
    STIM_TASK_STATUS = 0;

    ENABLE_RECORD = false;
    RECORD_OFFSET = 0;

    ble_init();
    gpio_set_direction(GPIO_NUM_5,GPIO_MODE_OUTPUT);
    while(1){
        gpio_set_level(GPIO_NUM_5,1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_5,0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}


void STIM_START(){
    STIM_TASK_STATUS = 1;
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_2);
    if(ENABLE_RECORD){
        configure_i2s();
        xTaskCreatePinnedToCore(recording, "recording", 1024*4, NULL, 6, NULL, 0);
    }
    if(RECORD_OFFSET < 0){
        vTaskDelay( -RECORD_OFFSET / portTICK_PERIOD_MS);
    }

    if(STIM_TYPE){
        if(BURST_NUM){
            xTaskCreatePinnedToCore(burst_biphasic_loop_count, "burst count biphasic", 2048, NULL, 10, &STIM_TASK, 1);
        }else{
            xTaskCreatePinnedToCore(burst_biphasic_loop_infinity, "burst infinity biphasic", 2048, NULL, 10, &STIM_TASK, 1);
        }
    }else{
        if(PULSE_NUM){
            xTaskCreatePinnedToCore(biphasic_loop_count, "biphasic loop count", 2048, NULL, 10, &STIM_TASK, 1);
        }else{
            xTaskCreatePinnedToCore(biphasic_loop_infinity, "biphasic loop", 2048, NULL, 10, &STIM_TASK, 1);
        }
    }
}

void STIM_STOP(){
    STIM_STATUS = 0;
    printf("stopped!\n");
}

void IRAM_ATTR biphasic_loop_infinity(void *params)
{
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
    uint32_t phase_one = PHASE_ONE_TIME;
    uint32_t phase_two = PHASE_TWO_TIME;
    uint32_t phase_gap = INTER_PHASE_GAP;
    uint32_t stim_delay = INTER_STIM_DELAY;
    uint8_t dac_phase_one, dac_phase_two;
    uint8_t dac_gap;
    uint32_t period = PHASE_ONE_TIME + PHASE_TWO_TIME + INTER_PHASE_GAP + INTER_STIM_DELAY;
    uint32_t cycles = 1000000 / period;//how many cycles in one seconds
    if(DEBUG_MODE_ENABLED){
        dac_phase_one = DAC_PHASE_ONE;
        dac_phase_two = DAC_PHASE_TWO;
        dac_gap = 127;
    }else{
        float step_voltage = (VREF_255 - VREF_0)/255;//DAC's step volatge
        uint8_t steps = 3000/step_voltage;// how many steps from 0 to 3V (Vref_0 to 3 + Vref_0) dac is not perfect; maps to -3mA to 3mA
        dac_gap = steps/2; // median value -> 0mA; dac steps from 0 to 3mA or 0 to -3mA
        uint16_t amp_step = 3000/dac_gap; // uA/step
        if(ANODIC_CATHODIC){
            dac_phase_one = dac_gap - STIM_AMP/amp_step;
            dac_phase_two  =dac_gap + STIM_AMP/amp_step;
        }else{
            dac_phase_one = dac_gap + STIM_AMP/amp_step;
            dac_phase_two  =dac_gap - STIM_AMP/amp_step;
        }
    }
    if(stim_delay > 1)
        stim_delay--;//little offset

    dac_output_voltage(DAC_CHANNEL_2, dac_gap);
    STIM_STATUS = 1;//mark as stimulation begin
    uint8_t temp_dac_one = dac_gap;
    uint8_t temp_dac_two = dac_gap;
    if(RAMP_UP){
        if(ANODIC_CATHODIC){
            int c = 0;
            while(STIM_STATUS && temp_dac_two < dac_phase_two){
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_one);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_gap);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_two);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(stim_delay);
                if(c == cycles){
                    temp_dac_one--;
                    temp_dac_two++;
                    c = 0;
                }
                c++;
            }
        }else{
            int c = 0;
            while(STIM_STATUS && temp_dac_one < dac_phase_one){
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_one);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_gap);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_two);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(stim_delay);
                if(c == cycles){
                    temp_dac_one++;
                    temp_dac_two--;
                    c = 0;
                }
                c++;
            }
        }
    }

    while(STIM_STATUS){
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_one, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_one);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_gap);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_two, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_two);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(stim_delay);
    }
    dac_output_voltage(DAC_CHANNEL_1,dac_gap);//may need to change to fit elec team's circuit
    STIM_TASK_STATUS = 0;//mark as stimulation task finish
    vTaskDelete(NULL);
}

void IRAM_ATTR biphasic_loop_count(void *params)
{
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
    uint32_t temp = PULSE_NUM;
    uint32_t phase_one = PHASE_ONE_TIME;
    uint32_t phase_two = PHASE_TWO_TIME;
    uint32_t phase_gap = INTER_PHASE_GAP;
    uint32_t stim_delay = INTER_STIM_DELAY;
    uint8_t dac_phase_one, dac_phase_two;
    uint8_t dac_gap;
    uint32_t period = PHASE_ONE_TIME + PHASE_TWO_TIME + INTER_PHASE_GAP + INTER_STIM_DELAY;
    uint32_t cycles = 1000000 / period;//how many cycles in one seconds
    if(DEBUG_MODE_ENABLED){
        dac_phase_one = DAC_PHASE_ONE;
        dac_phase_two = DAC_PHASE_TWO;
        dac_gap = 127;
    }else{
        float step_voltage = (VREF_255 - VREF_0)/255;//DAC's step volatge
        uint8_t steps = 3000/step_voltage;// how many steps from 0 to 3V (Vref_0 to 3 + Vref_0) dac is not perfect; maps to -3mA to 3mA
        dac_gap = steps/2; // median value -> 0mA; dac steps from 0 to 3mA or 0 to -3mA
        uint16_t amp_step = 3000/dac_gap; // uA/step
        if(ANODIC_CATHODIC){
            dac_phase_one = dac_gap - STIM_AMP/amp_step;
            dac_phase_two  =dac_gap + STIM_AMP/amp_step;
        }else{
            dac_phase_one = dac_gap + STIM_AMP/amp_step;
            dac_phase_two  =dac_gap - STIM_AMP/amp_step;
        }
    }
    //printf("dac phase 1 is %u ;phase two is %u; dac_gap is %d\n",dac_phase_one,dac_phase_two,dac_gap);
    dac_output_voltage(DAC_CHANNEL_2, dac_gap);
    STIM_STATUS = 1;//mark as stimulation begin
    uint8_t temp_dac_one = dac_gap;
    uint8_t temp_dac_two = dac_gap;
    if(stim_delay > 1)
        stim_delay--;//little offset

    if(RAMP_UP){
        if(ANODIC_CATHODIC){
            int c = 0;
            while(STIM_STATUS && temp_dac_two < dac_phase_two && temp > 0){
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_one);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_gap);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_two);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(stim_delay);
                if(c == cycles){
                    temp_dac_one--;
                    temp_dac_two++;
                    c = 0;
                }
                temp--;
                c++;
            }
        }else{
            int c = 0;
            while(STIM_STATUS && temp_dac_one < dac_phase_one && temp > 0){
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_one);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_gap);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(phase_two);
                SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                ets_delay_us(stim_delay);
                if(c == cycles){
                    temp_dac_one++;
                    temp_dac_two--;
                    c = 0;
                }
                temp--;
                c++;
            }
        }
    }

    while(STIM_STATUS && temp > 0){
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_one, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_one);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_gap);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_two, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(phase_two);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(stim_delay);
        temp--;
    }
    dac_output_voltage(DAC_CHANNEL_1,dac_gap);//may need to change to fit elec team's circuit
    STIM_TASK_STATUS = 0;//mark as stimulation task finish
    STIM_STATUS = 0;
    vTaskDelete(NULL);
}


void IRAM_ATTR burst_biphasic_loop_infinity(void *params)
{
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
    uint32_t phase_one = PHASE_ONE_TIME;
    uint32_t phase_two = PHASE_TWO_TIME;
    uint32_t phase_gap = INTER_PHASE_GAP;
    uint32_t stim_delay = INTER_STIM_DELAY;
    uint32_t burst_delay = INTER_BURST_DELAY;
    uint8_t dac_phase_one, dac_phase_two;
    uint8_t dac_gap;
    uint32_t period = PHASE_ONE_TIME + PHASE_TWO_TIME + INTER_PHASE_GAP + INTER_STIM_DELAY;
    uint32_t cycles = 1000000 / period;//how many cycles in one seconds
    if(DEBUG_MODE_ENABLED){
        dac_phase_one = DAC_PHASE_ONE;
        dac_phase_two = DAC_PHASE_TWO;
        dac_gap = 127;
    }else{
        float step_voltage = (VREF_255 - VREF_0)/255;//DAC's step volatge
        uint8_t steps = 3000/step_voltage;// how many steps from 0 to 3V (Vref_0 to 3 + Vref_0) dac is not perfect; maps to -3mA to 3mA
        dac_gap = steps/2; // median value -> 0mA; dac steps from 0 to 3mA or 0 to -3mA
        uint16_t amp_step = 3000/dac_gap; // uA/step
        if(ANODIC_CATHODIC){
            dac_phase_one = dac_gap - STIM_AMP/amp_step;
            dac_phase_two  =dac_gap + STIM_AMP/amp_step;
        }else{
            dac_phase_one = dac_gap + STIM_AMP/amp_step;
            dac_phase_two  =dac_gap - STIM_AMP/amp_step;
        }
    }

    if(stim_delay > 1)
        stim_delay--;//little offset
    uint8_t temp_dac_one = dac_gap;
    uint8_t temp_dac_two = dac_gap;
    //printf("dac phase 1 is %u ;phase two is %u; dac_gap is %d\n",dac_phase_one,dac_phase_two,dac_gap);
    dac_output_voltage(DAC_CHANNEL_2, dac_gap);
    STIM_STATUS = 1;//mark as stimulation begin
    uint32_t temp_pulse = PULSE_NUM_IN_ONE_BURST;

    if(RAMP_UP){
        int c = 0;
        if(ANODIC_CATHODIC){
            while(STIM_STATUS && temp_dac_two < dac_phase_two){
                temp_pulse = PULSE_NUM_IN_ONE_BURST;
                while(STIM_STATUS && temp_pulse > 0){
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_one);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_gap);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_two);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(stim_delay);
                    if(c == cycles && temp_dac_two < dac_phase_two){
                        c = 0;
                        temp_dac_one--;
                        temp_dac_two++;
                    }
                    temp_pulse--;
                    c++;
                }
                ets_delay_us(burst_delay);
            }
        }else{
            while(STIM_STATUS && temp_dac_one < dac_phase_one){
                temp_pulse = PULSE_NUM_IN_ONE_BURST;
                while(STIM_STATUS && temp_pulse > 0){
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_one);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_gap);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_two);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(stim_delay);
                    if(c == cycles && temp_dac_one < dac_phase_one){
                        c = 0;
                        temp_dac_one++;
                        temp_dac_two--;
                    }
                    temp_pulse--;
                    c++;
                }
                ets_delay_us(burst_delay);
            }
        }  
    }



    while(STIM_STATUS){
        temp_pulse = PULSE_NUM_IN_ONE_BURST;
        while(STIM_STATUS && temp_pulse > 0){
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_one, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_one);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_gap);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_two, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_two);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(stim_delay);
            temp_pulse--;
        }
        ets_delay_us(burst_delay);
    }
    dac_output_voltage(DAC_CHANNEL_1,dac_gap);//may need to change to fit elec team's circuit
    STIM_TASK_STATUS = 0;//mark as stimulation task finish
    vTaskDelete(NULL);
}

void IRAM_ATTR burst_biphasic_loop_count(void *params)
{
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
    uint32_t phase_one = PHASE_ONE_TIME;
    uint32_t phase_two = PHASE_TWO_TIME;
    uint32_t phase_gap = INTER_PHASE_GAP;
    uint32_t stim_delay = INTER_STIM_DELAY;
    uint32_t burst_delay = INTER_BURST_DELAY;
    uint8_t dac_phase_one, dac_phase_two;
    uint8_t dac_gap;
    uint32_t period = PHASE_ONE_TIME + PHASE_TWO_TIME + INTER_PHASE_GAP + INTER_STIM_DELAY;
    uint32_t cycles = 1000000 / period;//how many cycles in one seconds
    if(DEBUG_MODE_ENABLED){
        dac_phase_one = DAC_PHASE_ONE;
        dac_phase_two = DAC_PHASE_TWO;
        dac_gap = 127;
    }else{
        float step_voltage = (VREF_255 - VREF_0)/255;//DAC's step volatge
        uint8_t steps = 3000/step_voltage;// how many steps from 0 to 3V (Vref_0 to 3 + Vref_0) dac is not perfect; maps to -3mA to 3mA
        dac_gap = steps/2; // median value -> 0mA; dac steps from 0 to 3mA or 0 to -3mA
        uint16_t amp_step = 3000/dac_gap; // uA/step
        if(ANODIC_CATHODIC){
            dac_phase_one = dac_gap - STIM_AMP/amp_step;
            dac_phase_two  =dac_gap + STIM_AMP/amp_step;
        }else{
            dac_phase_one = dac_gap + STIM_AMP/amp_step;
            dac_phase_two  =dac_gap - STIM_AMP/amp_step;
        }
    }
    if(stim_delay > 1)
        stim_delay--;//little offset
    uint8_t temp_dac_one = dac_gap;
    uint8_t temp_dac_two = dac_gap;
    //printf("dac phase 1 is %u ;phase two is %u; dac_gap is %d\n",dac_phase_one,dac_phase_two,dac_gap);
    dac_output_voltage(DAC_CHANNEL_2, dac_gap);
    uint32_t temp_burst = BURST_NUM;
    uint32_t temp_pulse = PULSE_NUM_IN_ONE_BURST;
    STIM_STATUS = 1;//mark as stimulation begin

    if(RAMP_UP){
        int c = 0;
        if(ANODIC_CATHODIC){
            while(STIM_STATUS && temp_dac_two < dac_phase_two &&temp_burst > 0){
                temp_pulse = PULSE_NUM_IN_ONE_BURST;
                while(STIM_STATUS && temp_pulse > 0){
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_one);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_gap);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_two);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(stim_delay);
                    if(c == cycles && temp_dac_two < dac_phase_two){
                        c = 0;
                        temp_dac_one--;
                        temp_dac_two++;
                    }
                    temp_pulse--;
                    c++;
                }
                ets_delay_us(burst_delay);
                temp_burst--;
            }
        }else{
            while(STIM_STATUS && temp_dac_one < dac_phase_one && temp_burst > 0){
                temp_pulse = PULSE_NUM_IN_ONE_BURST;
                while(STIM_STATUS && temp_pulse > 0){
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_one, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_one);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_gap);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, temp_dac_two, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(phase_two);
                    SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
                    ets_delay_us(stim_delay);
                    if(c == cycles && temp_dac_one < dac_phase_one){
                        c = 0;
                        temp_dac_one++;
                        temp_dac_two--;
                    }
                    temp_pulse--;
                    c++;
                }
                ets_delay_us(burst_delay);
                temp_burst--;
            }
        }  
    }




    while(STIM_STATUS && temp_burst > 0){
        temp_pulse = PULSE_NUM_IN_ONE_BURST;
        while(STIM_STATUS && temp_pulse > 0){
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_one, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_one);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_gap);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_phase_two, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(phase_two);
            SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, dac_gap, RTC_IO_PDAC1_DAC_S);
            ets_delay_us(stim_delay);
            temp_pulse--;
        }
        temp_burst--;
        ets_delay_us(burst_delay);
    }
    dac_output_voltage(DAC_CHANNEL_1,dac_gap);//may need to change to fit elec team's circuit
    STIM_TASK_STATUS = 0;//mark as stimulation task finish
    STIM_STATUS = 0;
    vTaskDelete(NULL);
}
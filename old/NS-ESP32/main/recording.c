#include "recording.h"
uint16_t i2s_read_buff[512];
void configure_i2s(){
  i2s_config_t i2s_config =
      {
          .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),                // I2S receive mode with ADC
          .sample_rate = 3440,                                                                        // is not accurate; need to be calibrated ;3440 is 2khz
          .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                                               // 16 bit I2S
          .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,                                                 // all the left channel
          .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB), // I2S format
          .intr_alloc_flags = 0,                                                                      // none
          .dma_buf_count = 2,                                                                         // number of DMA buffers ( maybe this is too much? )
          .dma_buf_len = 1024,                                                                        // number of samples
          .use_apll = 0,                                                                              // no Audio PLL
      };
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);
  adc1_config_width(ADC_WIDTH_12Bit);
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_6);
  SET_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_SAR1_INV);
  ets_delay_us(1000);
}

void IRAM_ATTR recording(void *params){
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    size_t bytes_read;
    while(STIM_STATUS != 1){
      printf("stuck here\n");
    }
    if(RECORD_OFFSET > 0){
      vTaskDelay( RECORD_OFFSET / portTICK_PERIOD_MS);
    }
    printf("recording start!\n");
    i2s_adc_enable(I2S_NUM_0);
    while(STIM_STATUS){
        i2s_read(I2S_NUM_0, (char*)i2s_read_buff,512 * sizeof(uint16_t), &bytes_read, portMAX_DELAY);
        /* for(uint32_t i = 0; i < 100; i++)
        {
            int16_t raw = i2s_read_buff[i] & 0xfff;
            ESP_LOGI("i2s", "adc dma i2s sample %d: raw %d, voltage %d mV", i, raw, esp_adc_cal_raw_to_voltage(raw, &adc_chars));
        } */ 
        //vTaskDelay(1000 / portTICK_PERIOD_MS); 
        //printf("received %u bytes\n",bytes_read);
        break;//testing purpose
    }
    for(uint32_t i = 0; i < 512; i++)
    {
        int16_t raw = i2s_read_buff[i] & 0xfff;
        ESP_LOGI("i2s", "adc dma i2s sample %d: raw %d", i, raw);
    }
    i2s_adc_disable(I2S_NUM_0);
    i2s_driver_uninstall(I2S_NUM_0);
    printf("recording ended!\n");
    vTaskDelete(NULL);
}
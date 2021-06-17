#include "battery.h"
int battery_init(){
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    cmd_handle = i2c_cmd_link_create();

    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (MAX17043_ADDRESS << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_stop(cmd_handle);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 50 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
    if (ret != ESP_OK)
    {
        printf("Please check i2c connection!\n");
        return -1;
    }
    return 0;
}

void battery_update(){
    cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (MAX17043_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_handle, REG_ADDRESS, true);
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (MAX17043_ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd_handle, &BATTERY_LEVEL, I2C_MASTER_ACK);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_handle);
    printf("%d\n",BATTERY_LEVEL);
}

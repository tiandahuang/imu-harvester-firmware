/**
 * application specific accelerometer driver for BMA400
 */

#include "app_accelerometer.h"
#include "bma400_defs.h"
#include "bma400.h"
#include "app_debug.h"
#include "app_spi.h"


BMA400_INTF_RET_TYPE bma400_spi_write(
        uint8_t reg_addr, 
        const uint8_t *reg_data, 
        uint32_t len, 
        void *intf_ptr);
BMA400_INTF_RET_TYPE bma400_spi_read(
        uint8_t reg_addr, 
        uint8_t *reg_data, 
        uint32_t len, 
        void *intf_ptr);
void bma400_delay_us(
        uint32_t period, 
        void *intf_ptr);


// #define SPI_BUFFER_LEN  256
// static uint8_t spi_tx_buf[SPI_BUFFER_LEN] = {0};


// int16_t accelerometer_buffer[ACCELEROMETER_BUFFER_SIZE];
// uint8_t fifo_buff[FIFO_SIZE_FULL] = { 0 };
// struct bma400_sensor_data accel_data[N_FRAMES_FULL] = { { 0 } };
// static uint16_t accel_frames_req = N_FRAMES_FULL;

static uint8_t              dev_addr    = IMU_CS;
struct bma400_dev           bma         = {
        .intf = BMA400_SPI_INTF,
        .intf_ptr = &dev_addr,
        .read = bma400_spi_read,
        .write = bma400_spi_write,
        .delay_us = bma400_delay_us,
        .read_write_len = APP_SPI_MAX_TRANSFER_LEN - 1
};


int accelerometer_init(void) {

    // check for device existence
    if (bma400_init(&bma) != BMA400_OK) {
        debug_log("bma400 not found!");
        debug_flush();
        return -1;
    }



    // /* Modify the desired configurations as per macros
    //  * available in bma400_defs.h file */
    // conf.param.accel.odr = BMA400_ODR_25HZ;
    // conf.param.accel.range = BMA400_RANGE_4G;
    // conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;

    // /* Set the desired configurations to the sensor */
    // bma400_set_sensor_conf(&conf, 1, &bma);

    // fifo_conf.type = BMA400_FIFO_CONF;

    // bma400_get_device_conf(&fifo_conf, 1, &bma);

    // fifo_conf.param.fifo_conf.conf_regs = BMA400_FIFO_X_EN | BMA400_FIFO_Y_EN | BMA400_FIFO_Z_EN;
    // fifo_conf.param.fifo_conf.conf_status = BMA400_ENABLE;
    // fifo_conf.param.fifo_conf.fifo_full_channel = BMA400_INT_CHANNEL_1;

    // bma400_set_device_conf(&fifo_conf, 1, &bma);

    // bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);

    // fifo_frame.data = fifo_buff;
    // fifo_frame.length = FIFO_SIZE_FULL;

    // int_en.type = BMA400_FIFO_FULL_INT_EN;
    // int_en.conf = BMA400_ENABLE;

    // bma400_enable_interrupt(&int_en, 1, &bma);
}

void accelerometer_wake(void) {

}

void accelerometer_sleep(void) {

}

void accelerometer_get_data(uint8_t **data_ptr, uint16_t *data_len) {

}




BMA400_INTF_RET_TYPE bma400_spi_write(
        uint8_t reg_addr, 
        const uint8_t *reg_data, 
        uint32_t len, 
        void *intf_ptr) {

    return (int8_t)app_spi_readwrite_reg(reg_addr, (uint8_t *)reg_data, NULL, len, NULL);
}

BMA400_INTF_RET_TYPE bma400_spi_read(
        uint8_t reg_addr, 
        uint8_t *reg_data, 
        uint32_t len, 
        void *intf_ptr) {
    
    return (int8_t)app_spi_readwrite_reg(reg_addr, NULL, reg_data, len, NULL);
}

void bma400_delay_us(uint32_t period, void *intf_ptr) {
    // not implemented
    return;
}

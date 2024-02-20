
#include "app_accelerometer.h"
#include "bma400_defs.h"
#include "bma400.h"

#define SPI_BUFFER_LEN 256


int16_t accelerometer_buffer[ACCELEROMETER_BUFFER_SIZE];
uint8_t fifo_buff[FIFO_SIZE_FULL] = { 0 };
struct bma400_sensor_data accel_data[N_FRAMES_FULL] = { { 0 } };
static uint16_t accel_frames_req = N_FRAMES_FULL;

struct bma400_int_enable    int_en;
struct bma400_fifo_data     fifo_frame;
struct bma400_device_conf   fifo_conf;
struct bma400_sensor_conf   conf;
struct bma400_dev   bma;
static uint8_t      dev_addr;

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
void bma400_delay_us(uint32_t period, void *intf_ptr);

int accelerometer_init(void) {
    // config device parameters
    dev_addr = CS_PIN;
    bma.read = bma400_spi_read;
    bma.write = bma400_spi_write;
    bma.intf = BMA400_SPI_INTF;

    bma.intf_ptr = &dev_addr;
    bma.delay_us = bma400_delay_us;
    bma.read_write_len = SPI_BUFFER_LEN - 1;

    // initialize device
    if (bma400_soft_reset(&bma) != BMA400_OK)   return -1;
    if (bma400_init(&bma) != BMA400_OK)         return -1;

    conf.type = BMA400_ACCEL;
    bma400_get_sensor_conf(&conf, 1, &bma);

    /* Modify the desired configurations as per macros
     * available in bma400_defs.h file */
    conf.param.accel.odr = BMA400_ODR_25HZ;
    conf.param.accel.range = BMA400_RANGE_4G;
    conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;

    /* Set the desired configurations to the sensor */
    bma400_set_sensor_conf(&conf, 1, &bma);

    fifo_conf.type = BMA400_FIFO_CONF;

    bma400_get_device_conf(&fifo_conf, 1, &bma);

    fifo_conf.param.fifo_conf.conf_regs = BMA400_FIFO_X_EN | BMA400_FIFO_Y_EN | BMA400_FIFO_Z_EN;
    fifo_conf.param.fifo_conf.conf_status = BMA400_ENABLE;
    fifo_conf.param.fifo_conf.fifo_full_channel = BMA400_INT_CHANNEL_1;

    bma400_set_device_conf(&fifo_conf, 1, &bma);

    bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);

    fifo_frame.data = fifo_buff;
    fifo_frame.length = FIFO_SIZE_FULL;

    int_en.type = BMA400_FIFO_FULL_INT_EN;
    int_en.conf = BMA400_ENABLE;

    bma400_enable_interrupt(&int_en, 1, &bma);
}

void accelerometer_power_on(void) {

}

void accelerometer_power_off(void) {

}

void accelerometer_read(void * p_event_data, uint16_t event_size) {

}




















BMA400_INTF_RET_TYPE bma400_spi_write(
    uint8_t reg_addr, 
    const uint8_t *reg_data, 
    uint32_t len, 
    void *intf_ptr) {

    uint8_t spi_buf[SPI_BUFFER_LEN];

    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    spi_buf[0] = reg_addr;
    memcpy(&spi_buf[1], reg_data, len);

    extended_spi_transfer(spi_buf, NULL, len+1, CS_PIN);
    
    return BMA400_INTF_RET_SUCCESS;
}

BMA400_INTF_RET_TYPE bma400_spi_read(
    uint8_t reg_addr, 
    uint8_t *reg_data, 
    uint32_t len, 
    void *intf_ptr) {
    
    uint8_t spi_tx_buf[SPI_BUFFER_LEN] = {0};
    uint8_t spi_rx_buf[SPI_BUFFER_LEN];

    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    spi_tx_buf[0] = reg_addr;
    
    extended_spi_transfer(spi_tx_buf, spi_rx_buf, len+1, CS_PIN);
    memcpy(reg_data, &spi_rx_buf[1], len);
    
    return BMA400_INTF_RET_SUCCESS;
}

void bma400_delay_us(uint32_t period, void *intf_ptr) {
    nrf_delay_us(period);
    return BMA400_INTF_RET_SUCCESS;
}

/**
 * application specific accelerometer driver for BMA400
 */

#include "app_accelerometer.h"
#include "bma400_defs.h"
#include "bma400.h"
#include "app_debug.h"
#include "app_spi.h"
#include "app_callbacks.h"
#include "nrfx_gpiote.h"


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



#define N_FRAMES 16
#define FIFO_SIZE ((N_FRAMES) + (BMA400_FIFO_BYTES_OVERREAD))

struct bma400_sensor_data accel_data[N_FRAMES] = { { 0 } };

struct bma400_int_enable int_en;
struct bma400_fifo_data fifo_frame;
struct bma400_device_conf fifo_conf;
struct bma400_sensor_conf conf;

uint16_t int_status = 0;
uint8_t fifo_buff[FIFO_SIZE] = { 0 };
uint16_t accel_frames_req = N_FRAMES;

static uint8_t              dev_addr    = IMU_CS;
struct bma400_dev           bma         = {
        .intf = BMA400_SPI_INTF,
        .intf_ptr = &dev_addr,
        .read = bma400_spi_read,
        .write = bma400_spi_write,
        .delay_us = bma400_delay_us,
        .read_write_len = APP_SPI_MAX_TRANSFER_LEN - 1
};

WEAK_CALLBACK_DEF(ACCELEROMETER_DATA_READY)

static void int1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    int_status = 1;
    CALLBACK_FUNC(ACCELEROMETER_DATA_READY)();
}

int accelerometer_init(void) {

    int8_t rslt;

    // check for device existence
    if (bma400_init(&bma) != BMA400_OK) {
        debug_log("bma400 not found!");
        debug_flush();
        return -1;
    }

    // set up GPIO interrupts
    nrfx_gpiote_in_config_t int1_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    nrfx_gpiote_in_init(IMU_INT1, &int1_config, int1_handler);
    nrfx_gpiote_in_event_enable(IMU_INT1, true);

    /* Select the type of configuration to be modified */
    conf.type = BMA400_ACCEL;

    /* Get the accelerometer configurations which are set in the sensor */
    rslt = bma400_get_sensor_conf(&conf, 1, &bma);
    if (rslt != BMA400_OK) return rslt;

    /* Modify the desired configurations as per macros
     * available in bma400_defs.h file */
    conf.param.accel.odr = BMA400_ODR_25HZ;
    conf.param.accel.range = BMA400_RANGE_4G;
    conf.param.accel.data_src = BMA400_DATA_SRC_ACCEL_FILT_1;

    /* Set the desired configurations to the sensor */
    rslt = bma400_set_sensor_conf(&conf, 1, &bma);
    if (rslt != BMA400_OK) return rslt;

    fifo_conf.type = BMA400_FIFO_CONF;

    rslt = bma400_get_device_conf(&fifo_conf, 1, &bma);
    if (rslt != BMA400_OK) return rslt;

    fifo_conf.param.fifo_conf.conf_regs = BMA400_FIFO_X_EN 
                                        | BMA400_FIFO_Y_EN 
                                        | BMA400_FIFO_Z_EN
                                        | BMA400_FIFO_AUTO_FLUSH;   // flush on power mode change
    fifo_conf.param.fifo_conf.conf_status = BMA400_ENABLE;
    fifo_conf.param.fifo_conf.fifo_watermark = N_FRAMES;
    fifo_conf.param.fifo_conf.fifo_wm_channel = BMA400_INT_CHANNEL_1;

    rslt = bma400_set_device_conf(&fifo_conf, 1, &bma);
    if (rslt != BMA400_OK) return rslt;

    rslt = bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);
    if (rslt != BMA400_OK) return rslt;

    fifo_frame.data = fifo_buff;
    fifo_frame.length = FIFO_SIZE;

    int_en.type = BMA400_FIFO_WM_INT_EN;
    int_en.conf = BMA400_ENABLE;

    rslt = bma400_enable_interrupt(&int_en, 1, &bma);
    if (rslt != BMA400_OK) return rslt;

    // init done; sleep the accelerometer
    accelerometer_sleep();

    return 0;
}

void accelerometer_wake(void) {
    bma400_set_power_mode(BMA400_MODE_NORMAL, &bma);
}

void accelerometer_sleep(void) {
    bma400_set_power_mode(BMA400_MODE_SLEEP, &bma);
}

void accelerometer_get_data(uint8_t **data_ptr, uint16_t *data_len) {

    // bma400_get_fifo_data(&fifo_frame, &bma);
    // accel_frames_req = N_FRAMES;
    // bma400_extract_accel(&fifo_frame, accel_data, &accel_frames_req, &bma);
    // *data_len = accel_frames_req;

    bma400_get_regs(BMA400_REG_FIFO_LENGTH, (uint8_t *)data_len, 2, &bma);
}


// interface function implmementations

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

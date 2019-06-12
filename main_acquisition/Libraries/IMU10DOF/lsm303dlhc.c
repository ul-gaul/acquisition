/*
 * lsm303dlhc.c
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#include "lsm303dlhc.h"


LSM303DLHC_result lsm303dlhc_init(void) {

	//SECTION ACCELEROMETER
	/* write to CTRL_REG1_A
     * set power mode to "normal/low-power (400 Hz)"
     */
    i2c_write(LSM_ACC_ADDRESS_W, LSM_CTRL_REG1_A, 0x77);

    /* write to CTRL_REG4_A
     * output registers are not updated while reading data and
     * high resolution mode is enabled
     */
    uint8_t value = 0x80;
    value |= LSM_ACC_G_GAIN << 4; // set g gain
    value |= 8; // enable High Resolution mode
    i2c_write(LSM_ACC_ADDRESS_W, LSM_CTRL_REG4_A, value);

    // temporary, read control registers, to debug
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG1_A); // 0x77
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG2_A); // 0x00
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG3_A); // 0x00
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG4_A); // 0xB8
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG5_A); // 0x00
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_CTRL_REG6_A); // 0x00

    //SECTION MAGNETOMETER
    i2c_write(LSM_MAG_ADDRESS_W, LSM_CRA_REG_M, 0x9C);
    i2c_write(LSM_MAG_ADDRESS_W, LSM_CRB_REG_M, 0x20);
    i2c_write(LSM_MAG_ADDRESS_W, LSM_MR_REG_M, 0x00);

    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_CRA_REG_M); // 0x9C
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_CRB_REG_M); // 0x20
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_MR_REG_M); // 0x00


    return LSM303DLHC_Result_Ok;
}

LSM303DLHC_result lsm303dlhc_read_acceleration(LSM303DLHC_struct* data) {
    uint8_t value;
    /* Read X axis */
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_X_L_A);
    data->acc_x_uncomp = value;
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_X_H_A);
    data->acc_x_uncomp |= value << 8;

    /* Read Y axis */
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_Y_L_A);
    data->acc_y_uncomp = value;
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_Y_H_A);
    data->acc_y_uncomp |= value << 8;

    /* Read Z axis */
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_Z_L_A);
    data->acc_z_uncomp = value;
    value = i2c_read(LSM_ACC_ADDRESS_R, LSM_OUT_Z_H_A);
    data->acc_z_uncomp |= value << 8;

    /* compensate values */
    data->acc_x = data->acc_x_uncomp * LSM_ACC_RES * LSM_ACC_BIT_RES;
    data->acc_y = data->acc_y_uncomp * LSM_ACC_RES * LSM_ACC_BIT_RES;
    data->acc_z = data->acc_z_uncomp * LSM_ACC_RES * LSM_ACC_BIT_RES;
    /* Return OK */
    return LSM303DLHC_Result_Ok;
}

LSM303DLHC_result lsm303dlhc_read_magneticfield(LSM303DLHC_struct* data) {
    uint8_t value;
    //TODO : compensate the value
    /* Read X axis */
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_X_L_M);
    data->mag_x = value;
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_X_H_M);
    data->mag_x |= value << 8;

    /* Read Y axis */
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_Y_L_M);
    data->mag_y = value;
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_Y_H_M);
    data->mag_y |= value << 8;
    
    /* Read Z axis */
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_Z_L_M);
    data->mag_z = value;
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_OUT_Z_H_M);
    data->mag_z |= value << 8;
    /* Return OK */
    return LSM303DLHC_Result_Ok;
}

LSM303DLHC_result lsm303dlhc_read_temperature(LSM303DLHC_struct* data) {
    uint8_t value;

    /* Read temperature */ // la temperature est sur 12 bits
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_TEMP_OUT_L_M);
    data->temperature = value;
    value = i2c_read(LSM_MAG_ADDRESS_R, LSM_TEMP_OUT_H_M);
    data->temperature |= value << 8;
    data->temperature = data->temperature >> 4;
    /* Return OK */
    return LSM303DLHC_Result_Ok;
}

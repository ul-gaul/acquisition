/*
 * lsm303dlhc.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef IMU10DOF_LSM303DLHC_H_
#define IMU10DOF_LSM303DLHC_H_


#define LSM_ACC_ADDRESS_R 0x33
#define LSM_ACC_ADDRESS_W 0x32

#define LSM_MAG_ADDRESS_R 0x3D
#define LSM_MAG_ADDRESS_W 0x3C

#define LSM_CTRL_REG1_A 0x20
#define LSM_CTRL_REG2_A 0x21
#define LSM_CTRL_REG3_A 0x22
#define LSM_CTRL_REG4_A 0x23
#define LSM_CTRL_REG5_A 0x24
#define LSM_CTRL_REG6_A 0x25

#define LSM_REFERENCE_A 0x26

#define LSM_STATUS_REG_A 0x27

#define LSM_OUT_X_L_A 0x28
#define LSM_OUT_X_H_A 0x29
#define LSM_OUT_Y_L_A 0x2A
#define LSM_OUT_Y_H_A 0x2B
#define LSM_OUT_Z_L_A 0x2C
#define LSM_OUT_Z_H_A 0x2D

#define LSM_FIFO_CTRL_REG_A 0x2E
#define LSM_FIFO_SRC_REG_A 0x2F

#define LSM_INT1_CFG_A 0x30
#define LSM_INT1_SRC_A 0x31
#define LSM_INT1_THS_A 0x32
#define LSM_INT1_DURATION_A 0x33
#define LSM_INT2_CFG_A 0x34
#define LSM_INT2_SRC_A 0x35
#define LSM_INT2_THS_A 0x36
#define LSM_INT2_DURATION_A 0x37
#define LSM_CLICK_CFG_A 0x38
#define LSM_CLICK_SRC_A 0x39
#define LSM_CLICK_THS_A 0x3A
#define LSM_TIME_LIMIT_A 0x3B
#define LSM_TIME_LATENCY_A 0x3C
#define LSM_TIME_WINDOW_A 0x3D

#define LSM_CRA_REG_M 0x00
#define LSM_CRB_REG_M 0x01
#define LSM_MR_REG_M 0x02
#define LSM_OUT_X_H_M 0x03
#define LSM_OUT_X_L_M 0x04
#define LSM_OUT_Z_H_M 0x05
#define LSM_OUT_Z_L_M 0x06
#define LSM_OUT_Y_H_M 0x07
#define LSM_OUT_Y_L_M 0x08
#define LSM_SR_REG_M 0x09
#define LSM_IRA_REG_M 0x0A
#define LSM_IRB_REG_M 0x0B
#define LSM_IRC_REG_M 0x0C

#define LSM_TEMP_OUT_H_M 0x31
#define LSM_TEMP_OUT_L_M 0x32

/**
 * @brief
 * 	LSM303DLHC main struct
 * 	Contains XYZ accelerations and XYZ magnetic field measures
 */
typedef struct {
    short acc_x;
    short acc_y;
    short acc_z;
    short mag_x; 
    short mag_y; 
    short mag_z; 
} LSM303DLHC_struct;

/**
 * @brief
 * 	LSM303DLHC Results enumerations
 */
typedef struct {
    LSM303DLHC_Result_Ok,
    LSM303DLHC_Result_Error
} LSM303DLHC_result;

/**
 * @brief Initializes the sensor
 */
LSM303DLHC_result lsm303dlhc_init(void);

/**
 * @brief Read the acceleration measures
 */
LSM303DLHC_result lsm303dlhc_read_acceleration(LSM303DLHC_struct* data);


/**
 * @brief Read the magnetic field measures
 */
LSM303DLHC_result lsm303dlhc_read_magneticfield(LSM303DLHC_struct* data);


#endif /* IMU10DOF_LSM303DLHC_H_ */

/*
 * l3gd20.h
 *
 *  Created on: Jul 1, 2018
 *      Author: laplace
 */

#ifndef IMU10DOF_L3GD20_H_
#define IMU10DOF_L3GD20_H_
#include "imu10dof.h"
#include "stm32f4xx.h"
#include "imu10dof.h"
#include "math.h"

#define L3GD20H_I2c_SLAVE_ADDRESS 0b1101011 // 1101010 si SDO est connecter au ground, nous sommes connecter au VCC donc 1101011

#define L3GD20_ADDRESS (0x6B) // 1101011
#define L3GD20_POLL_TIMEOUT (100) // Maximum number of read attempts
#define L3GD20_ID (0xD4)
#define L3GD20H_ID (0xD7)
// Sensitivity values from the mechanical characteristics in the datasheet.
#define GYRO_SENSITIVITY_250DPS (0.00875F)
#define GYRO_SENSITIVITY_500DPS (0.0175F)
#define GYRO_SENSITIVITY_2000DPS (0.070F)

#define GYRO_WHO_AM_I 0xD4

#define GYRO_REGISTER_WHO_AM_I 0x0F // 11010100 r
#define GYRO_REGISTER_CTRL_REG1 0x20 // 00000111 rw
#define GYRO_REGISTER_CTRL_REG2 0x21 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG3 0x22 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG4 0x23 // 00000000 rw
#define GYRO_REGISTER_CTRL_REG5 0x24 // 00000000 rw
#define GYRO_REGISTER_REFERENCE 0x25 // 00000000 rw
#define GYRO_REGISTER_OUT_TEMP 0x26 // r
#define GYRO_REGISTER_STATUS_REG 0x27 // r
#define GYRO_REGISTER_OUT_X_L 0x28 // r
#define GYRO_REGISTER_OUT_X_H 0x29 // r
#define GYRO_REGISTER_OUT_Y_L 0x2A // r
#define GYRO_REGISTER_OUT_Y_H 0x2B // r
#define GYRO_REGISTER_OUT_Z_L 0x2C // r
#define GYRO_REGISTER_OUT_Z_H 0x2D // r
#define GYRO_REGISTER_FIFO_CTRL_REG 0x2E // gyro en mode fifo 0010 1110 rw, mettre 0x0E pour le mettre en bypass mode (restart data collection)
#define GYRO_REGISTER_FIFO_SRC_REG 0x2F // r
#define GYRO_REGISTER_INT1_CFG 0x30 // 00000000 rw
#define GYRO_REGISTER_INT1_SRC 0x31 // r
#define GYRO_REGISTER_TSH_XH 0x32 // 00000000 rw
#define GYRO_REGISTER_TSH_XL 0x33 // 00000000 rw
#define GYRO_REGISTER_TSH_YH 0x34 // 00000000 rw
#define GYRO_REGISTER_TSH_YL 0x35 // 00000000 rw
#define GYRO_REGISTER_TSH_ZH 0x36 // 00000000 rw
#define GYRO_REGISTER_TSH_ZL 0x37 // 00000000 rw
#define GYRO_REGISTER_INT1_DURATION 0x38 // 00000000   rw

/**
 * @brief  L3GD20 main working structure
 */
typedef struct {
	short X; /*!< X axis rotation */
	short Y; /*!< Y axis rotation */
	short Z; /*!< Z axis rotation */
} L3GD20_struct;

/**
 * @brief  L3GD20 Result enumerations
 */
typedef enum {
	L3GD20_Result_Ok,   /*!< Everything OK */
	L3GD20_Result_Error /*!< Error occurred */
} L3GD20_result;

/**
 * @brief  Enumeration for scale select
 */
typedef enum {
	L3GD20_Scale_250, /*!< Set full scale to 250 mdps */
	L3GD20_Scale_500, /*!< Set full scale to 500 mdps */
	L3GD20_Scale_2000 /*!< Set full scale to 2000 mdps */
} L3GD20_scale;


/**
 * @brief  Initializes L3GD20 sensor
 * @param  scale: L3GD20 scale selection. This parameter can be a value of @ref TM_L3GD20_Scale_t enumeration
 * @retval Sensor status:
 *            - TM_L3GD20_Result_Ok: Sensor detected
 *            - TM_L3GD20_Result_Error: Sensor not detected
 */
L3GD20_result L3GD20_Init(L3GD20_scale scale);

/**
 * @brief  Reads rotation data from sensor
 * @param  *L3DG20_Data: Pointer to working @ef TM_L3GD20_t structure
 * @retval Read status: TM_L3GD20_Result_Ok
 */
L3GD20_result L3GD20_Read(L3GD20_struct* L3DG20_Data);

void l3gd20_init(void);

#endif /* IMU10DOF_L3GD20_H_ */

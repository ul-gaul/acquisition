/*
 * rocket_packet.h
 *
 *  Created on: Nov 1, 2018
 *      Author: laplace
 */

#include <string.h>
#include "stm32f4xx.h"


#ifndef ROCKET_PACKET_ROCKET_PACKET_H_
#define ROCKET_PACKET_ROCKET_PACKET_H_

// size of the packet including the checksum
#define ROCKET_PACKET_SIZE 92
#define ROCKET_PACKET_START 's'

typedef struct {

	// time since boot in milliseconds
	double timestamp; // 8
	// GPS values
	double latitude; // 16
	double longitude; // 24
	char NSIndicator; // 25
	char EWIndicator; // 26
	double UTCTime; // 34
	// 10DOF values
	// BMP180
	
	float altitude; // 38
	uint32_t pressure; // 42
	float temperature; // 46
	// lsm303
	// acceleration values are in milli-G
	int16_t acc_x_uncomp; // 48
	int16_t acc_y_uncomp; // 50
	int16_t acc_z_uncomp; // 52
	float acc_x; // 56
	float acc_y; // 60
	float acc_z; // 64
	// magnetic field values are in milli-gauss
	int16_t mag_x; // 66
	int16_t mag_y; // 68
	int16_t mag_z; // 70
	// l3dg20
	// angular speed values are in degrees/s
	int16_t x_gyro; // 72
	int16_t y_gyro; // 74
	int16_t z_gyro; // 76

} RocketData;

typedef struct {
	RocketData data;
	char start_char;
	uint8_t checksum;
} RocketPacket;

unsigned int serialize_rocket_packet(RocketPacket* pkt, uint8_t* s);

#endif /* ROCKET_PACKET_ROCKET_PACKET_H_ */

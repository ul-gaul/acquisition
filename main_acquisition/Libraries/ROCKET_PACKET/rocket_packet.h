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
	char NSIndicator; // 32
	char EWIndicator; // 40
	double UTCTime; // 48
	// 10DOF values
	// BMP180
	
	float altitude; // 52
	uint32_t pressure; // 56
	float temperature; // 60
	// lsm303
	// acceleration values are in milli-G
	uint16_t acc_x_uncomp; // 62
	uint16_t acc_y_uncomp; // 64
	uint16_t acc_z_uncomp; // 66
	float acc_x; // 70
	float acc_y; // 74
	float acc_z; // 78
	// magnetic field values are in milli-gauss
	uint16_t mag_x; // 80
	uint16_t mag_y; // 82
	uint16_t mag_z; // 84
	// l3dg20
	// angular speed values are in degrees/s
	int16_t x_gyro; // 86
	int16_t y_gyro; // 88
	int16_t z_gyro; // 90

} RocketData;

typedef struct {
	RocketData data;
	char start_char;
	uint8_t checksum;
} RocketPacket;

unsigned int serialize_rocket_packet(RocketPacket* pkt, uint8_t* s);

#endif /* ROCKET_PACKET_ROCKET_PACKET_H_ */

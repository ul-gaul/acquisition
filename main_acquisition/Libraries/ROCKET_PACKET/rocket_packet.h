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
#define ROCKET_PACKET_SIZE 70
#define ROCKET_PACKET_START 's'

typedef struct {
	double timestamp; // 8
	// GPS values
	double latitude; // 16
	double longitude; // 24
	// 10DOF values
	float altitude; // 28
	float temperature; // 32
	float x_accel;// 36
	float y_accel;// 40
	float z_accel; // 44
	float x_magnet; // 48
	float y_magnet; // 52
	float z_magnet; // 56
	float x_gyro; // 60
	float y_gyro; // 64
	float z_gyro; // 68
} RocketData;

typedef struct {
	RocketData data;
	char start_char;
	uint8_t checksum;
} RocketPacket;

unsigned int serialize_rocket_packet(RocketPacket* pkt, uint8_t* s);

#endif /* ROCKET_PACKET_ROCKET_PACKET_H_ */

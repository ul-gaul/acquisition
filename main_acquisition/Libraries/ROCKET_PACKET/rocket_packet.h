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
#define ROCKET_PACKET_SIZE 61

typedef struct {
	unsigned long timestamp;
	// GPS values
	float latitude;
	float longitude;
	// 10DOF values
	float altitude;
	float temperature;
	float x_accel;
	float y_accel;
	float z_accel;
	float x_magnet;
	float y_magnet;
	float z_magnet;
	float x_gyro;
	float y_gyro;
	float z_gyro;
	// motor values
	float tank_pressure;
} RocketData;

typedef struct {
	RocketData data;
	unsigned short checksum;
} RocketPacket;

void serialize_rocket_packet(RocketPacket* pkt, uint8_t* s);

#endif /* ROCKET_PACKET_ROCKET_PACKET_H_ */

/*
 * @author: Maxime Guillemette
 * @team: Gaul - Avionique
 * @date: 2018-05-31
 */

#ifndef ROCKET_PACKET_H
#define ROCKET_PACKET_H

#include "mbed.h"

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
} RocketData;

typedef struct {
	RocketData data;
	uint8_t checksum;
} RocketPacket;

#endif // ROCKET_PACKET_H
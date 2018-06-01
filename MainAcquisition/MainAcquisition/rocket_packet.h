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
	float altitude;
	float latitude;
	float longitude;
} RocketData;

typedef struct {
	RocketData data;
	uint8_t checksum;
} RocketPacket;

#endif // ROCKET_PACKET_H
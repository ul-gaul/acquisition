/*
 * rocket_packet.c
 *
 *  Created on: Nov 1, 2018
 *      Author: laplace
 */

#include "rocket_packet.h"

void serialize_rocket_packet(RocketPacket* pkt, char* dst) {
	/*
	 * copy the rocket packet struct into the destination char array
	 */
	unsigned int offset = 0;
	// timestamp
	memcpy(dst + offset, (void *) &pkt->data.timestamp, sizeof(pkt->data.timestamp));
	offset += sizeof(pkt->data.timestamp);
	// GPS values
	memcpy(dst + offset, (void *) &pkt->data.latitude, sizeof(pkt->data.latitude));
	offset += sizeof(pkt->data.latitude);
	memcpy(dst + offset, (void *) &pkt->data.longitude, sizeof(pkt->data.longitude));
	offset += sizeof(pkt->data.longitude);
	// 10DOF values
	memcpy(dst + offset, (void *) &pkt->data.altitude, sizeof(pkt->data.altitude));
	offset += sizeof(pkt->data.altitude);
	memcpy(dst + offset, (void *) &pkt->data.temperature, sizeof(pkt->data.temperature));
	offset += sizeof(pkt->data.temperature);
	memcpy(dst + offset, (void *) &pkt->data.x_accel, sizeof(pkt->data.x_accel));
	offset += sizeof(pkt->data.x_accel);
	memcpy(dst + offset, (void *) &pkt->data.y_accel, sizeof(pkt->data.y_accel));
	offset += sizeof(pkt->data.y_accel);
	memcpy(dst + offset, (void *) &pkt->data.z_accel, sizeof(pkt->data.z_accel));
	offset += sizeof(pkt->data.z_accel);
	memcpy(dst + offset, (void *) &pkt->data.x_magnet, sizeof(pkt->data.x_magnet));
	offset += sizeof(pkt->data.x_magnet);
	memcpy(dst + offset, (void *) &pkt->data.y_magnet, sizeof(pkt->data.y_magnet));
	offset += sizeof(pkt->data.y_magnet);
	memcpy(dst + offset, (void *) &pkt->data.z_magnet, sizeof(pkt->data.z_magnet));
	offset += sizeof(pkt->data.z_magnet);
	memcpy(dst + offset, (void *) &pkt->data.x_gyro, sizeof(pkt->data.x_gyro));
	offset += sizeof(pkt->data.x_gyro);
	memcpy(dst + offset, (void *) &pkt->data.y_gyro, sizeof(pkt->data.y_gyro));
	offset += sizeof(pkt->data.y_gyro);
	memcpy(dst + offset, (void *) &pkt->data.z_gyro, sizeof(pkt->data.z_gyro));
	offset += sizeof(pkt->data.z_gyro);
	// motor values
	memcpy(dst + offset, (void*) &pkt->data.tank_pressure, sizeof(pkt->data.tank_pressure));
	offset += sizeof(pkt->data.tank_pressure);
	// compute checksum
	pkt->checksum = 0;
	for(int i = 0; i < offset; i++) {
		pkt->checksum += dst[i];
	}
	pkt->checksum = ~pkt->checksum;
	memcpy(dst + offset, (void *) &pkt->checksum, sizeof(pkt->checksum));
}

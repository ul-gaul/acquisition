/*
 * rocket_packet.c
 *
 *  Created on: Nov 1, 2018
 *      Author: laplace
 */

#include "rocket_packet.h"

unsigned int serialize_rocket_packet(RocketPacket* pkt, uint8_t* dst) {
	/*
	 * copy the rocket packet struct into the destination char array
	 */
	unsigned int offset = 0;
	char start_char = ROCKET_PACKET_START;

	// start char
	memcpy(dst + offset, (void *) &start_char, sizeof(char));
	offset += sizeof(char);
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
	memcpy(dst + offset, (void *) &pkt->data.acc_x, sizeof(pkt->data.acc_x));
	offset += sizeof(pkt->data.acc_x);
	memcpy(dst + offset, (void *) &pkt->data.acc_y, sizeof(pkt->data.acc_y));
	offset += sizeof(pkt->data.acc_y);
	memcpy(dst + offset, (void *) &pkt->data.acc_z, sizeof(pkt->data.acc_z));
	offset += sizeof(pkt->data.acc_z);
	memcpy(dst + offset, (void *) &pkt->data.mag_x, sizeof(pkt->data.mag_x));
	offset += sizeof(pkt->data.mag_x);
	memcpy(dst + offset, (void *) &pkt->data.mag_y, sizeof(pkt->data.mag_y));
	offset += sizeof(pkt->data.mag_y);
	memcpy(dst + offset, (void *) &pkt->data.mag_z, sizeof(pkt->data.mag_z));
	offset += sizeof(pkt->data.mag_z);
	memcpy(dst + offset, (void *) &pkt->data.x_gyro, sizeof(pkt->data.x_gyro));
	offset += sizeof(pkt->data.x_gyro);
	memcpy(dst + offset, (void *) &pkt->data.y_gyro, sizeof(pkt->data.y_gyro));
	offset += sizeof(pkt->data.y_gyro);
	memcpy(dst + offset, (void *) &pkt->data.z_gyro, sizeof(pkt->data.z_gyro));
	offset += sizeof(pkt->data.z_gyro);

	// compute checksum
	pkt->checksum = 0;
	for(int i = 1; i < offset; i++) {
		pkt->checksum += dst[i];
	}
	pkt->checksum = ~pkt->checksum;
	memcpy(dst + offset, (void *) &pkt->checksum, sizeof(pkt->checksum));
	offset += sizeof(pkt->checksum);
	return offset;
}

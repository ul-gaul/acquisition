/*
 * @author: Maxime Guillemette
 * @team: Gaul - Avionique
 * @date: 2018-05-31
 */


#include <mbed.h>
#include "GPS.h"
#include "rocket_packet.h"

// relay command related defines
#define COMMAND_LENGTH 3

#define GET_RELAY_STATE 0xA0
#define SET_RELAY 0xB0
#define RESET_RELAY 0xC0

#define RELAY_STM32 0x00
#define RELAY_DEPLOY1 0x01
#define RELAY_DEPLOY2 0x02

// RF communication related defines
#define RF_SEND_PERIOD 0.1	// 10Hz

// GPS related defines
#define GPS_SAMPLE_PERIOD 1	// 1Hz

// relay GPIOs
DigitalOut r_set_stm32(PD_6);
DigitalOut r_reset_stm32(PD_7);
DigitalOut r_feedback_stm32(PD_12);

DigitalOut r_set_deploy1(PD_13);
DigitalOut r_reset_deploy1(PD_10);
DigitalOut r_feedback_deploy1(PD_11);

DigitalOut r_set_deploy2(PD_0);
DigitalOut r_reset_deploy2(PD_1);
DigitalOut r_feedback_deploy2(PD_2);

// serial ports
Serial ser_rfcomm(PA_9, PA_10);
Serial ser_relays(PC_10, PC_11);

// GPS object
GPS gps(PD_8, PD_9);


// Ticker (timer) object to send data to ground station
Ticker rf_ticker;
Ticker gps_ticker;

// enum for the relay command decoding
enum RelayDecodeStates {
	WAIT_COMMAND,
	WAIT_RELAY_NO,
	WAIT_CHECKSUM
};
enum RelayDecodeStates r_decode_state = WAIT_COMMAND;
uint8_t r_command[COMMAND_LENGTH];

// global rocket data and rocket packet variables
RocketData rocket_data;
RocketPacket rocket_packet;
char* rocket_packet_serialized;
unsigned int sizeof_rocket_packet;

void ser_relay_handler(void);
void decode_relay_command(uint8_t* cmd, uint8_t b);
void execute_relay_command(uint8_t* cmd);
void serialize_rocket_packet(RocketPacket pkt, char* s);
void send_packet();
void update_gps();
void update_10dof();

int main() {
	ser_relays.attach(&ser_relay_handler);
	rf_ticker.attach(&send_packet, RF_SEND_PERIOD);
	gps_ticker.attach(&update_gps, GPS_SAMPLE_PERIOD);
	// TODO attach 10DOF
	sizeof_rocket_packet = sizeof(unsigned long)
								+ 4 * sizeof(float)
								+ sizeof(uint8_t);
	rocket_packet_serialized = (char *) malloc((size_t) sizeof_rocket_packet);
	for (;;) {
	}
}

void ser_relay_handler(void) {
	uint8_t b;
	if(ser_relays.readable()) {
		b = ser_relays.getc();
		decode_relay_command(r_command, b);
	}
}

void decode_relay_command(uint8_t* cmd, uint8_t b) {
	switch(r_decode_state) {
	case WAIT_COMMAND:
		if(b == GET_RELAY_STATE || b == SET_RELAY || b == RESET_RELAY) {
			cmd[0] = b;
			r_decode_state = WAIT_RELAY_NO;
		} else {
			// send NACK (0x00)
			ser_relays.putc(0x00);
		}
		break;
	case WAIT_RELAY_NO:
		if(b == RELAY_STM32 || b == RELAY_DEPLOY1 || b == RELAY_DEPLOY2) {
			cmd[1] = b;
			r_decode_state = WAIT_CHECKSUM;
		} else {
			cmd[0] = 0x00;	// invalidate current command
			r_decode_state = WAIT_COMMAND;
			// send NACK (0x00)
			ser_relays.putc(0x00);
		}
		break;
	case WAIT_CHECKSUM:
		r_decode_state = WAIT_COMMAND;
		if(b == cmd[0] + cmd[1]) {
			// command is valid
			execute_relay_command(cmd);
		} else {
			// send NACK (0x00)
			ser_relays.putc(0x00);
		}
		break;
	}
}

void execute_relay_command(uint8_t* cmd) {
	switch(cmd[0]) {
	case GET_RELAY_STATE:
		switch(cmd[1]) {
		case RELAY_STM32:
			ser_relays.putc(r_feedback_stm32.read());
			break;
		case RELAY_DEPLOY1:
			ser_relays.putc(r_feedback_deploy1.read());
			break;
		case RELAY_DEPLOY2:
			ser_relays.putc(r_feedback_deploy2.read());
			break;
		}
		break;
	case SET_RELAY:
		switch(cmd[1]) {
		case RELAY_STM32:
			r_reset_stm32 = 0;
			r_set_stm32 = 1;
			break;
		case RELAY_DEPLOY1:
			r_reset_deploy1 = 0;
			r_set_deploy1 = 1;
			break;
		case RELAY_DEPLOY2:
			r_reset_deploy2 = 0;
			r_set_deploy2 = 1;
			break;
		}
		// send ACK (0x01)
		ser_relays.putc(0x01);
		break;
	case RESET_RELAY:
		switch(cmd[1]) {
		case RELAY_STM32:
			r_set_stm32 = 0;
			r_reset_stm32 = 1;
			break;
		case RELAY_DEPLOY1:
			r_set_deploy1 = 0;
			r_reset_deploy1 = 1;
			break;
		case RELAY_DEPLOY2:
			r_set_deploy2 = 0;
			r_reset_deploy2 = 1;
			break;
		}
		// send ACK (0x01)
		ser_relays.putc(0x01);
		break;
	}
}

void serialize_rocket_packet(RocketPacket pkt, char* s) {
	size_t offset = 0;
	// copy struct into char array, using memcpy to avoid packing related bugs,
	// sending everything in order
	memcpy(s + offset, (void *) &pkt.data.timestamp, sizeof(pkt.data.timestamp));
	offset += sizeof(pkt.data.timestamp);
	memcpy(s + offset, (void *) &pkt.data.altitude, sizeof(pkt.data.altitude));
	offset += sizeof(pkt.data.altitude);
	memcpy(s + offset, (void *) &pkt.data.latitude, sizeof(pkt.data.latitude));
	offset += sizeof(pkt.data.latitude);
	memcpy(s + offset, (void *) &pkt.data.longitude, sizeof(pkt.data.longitude));
	offset += sizeof(pkt.data.longitude);
	// compute checksum
	pkt.checksum = 0;
	for(int i = 0 ; i < offset ; i++) {
		pkt.checksum += s[i];
	}
	memcpy(s + offset, (void *) &pkt.checksum, sizeof(pkt.checksum));
}

void send_packet() {
	serialize_rocket_packet(rocket_packet, rocket_packet_serialized);
	for(unsigned int i = 0; i < sizeof_rocket_packet; i++) {
		ser_rfcomm.putc(rocket_packet_serialized[i]);
	}
}

void update_gps() {
	// if sample succeeds, update rocket data
	if(gps.sample()) {
		rocket_data.latitude = gps.latitude;
		rocket_data.longitude = gps.longitude;
	}
}

void update_10dof() {
	// TODO read 10DOF values and update rocket data
}
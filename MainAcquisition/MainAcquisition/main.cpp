/*
 * @author: Maxime Guillemette
 * @team: Gaul - Avionique
 * @date: 2018-05-31
 */


#include <mbed.h>

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

// Ticker (timer) object to send data to ground station
Ticker rf_ticker;

// enum for the relay command decoding
enum RelayDecodeStates {
	WAIT_COMMAND,
	WAIT_RELAY_NO,
	WAIT_CHECKSUM
};
enum RelayDecodeStates r_decode_state = WAIT_COMMAND;
uint8_t r_command[COMMAND_LENGTH];


void ser_relay_handler(void);
void decode_relay_command(uint8_t* cmd, uint8_t b);
void execute_relay_command(uint8_t* cmd);
void send_packet();

int main() {
	ser_relays.attach(&ser_relay_handler);
	rf_ticker.attach(&send_packet, RF_SEND_PERIOD);
	for (;;) {
		//g_LED = 1;
		//wait_ms(500);
		//g_LED = 0;
		//wait_ms(500);
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
			
			break;
		case RELAY_DEPLOY1:
			
			break;
		case RELAY_DEPLOY2:
			
			break;
		}
		// send ACK (0x01)
		ser_relays.putc(0x01);
		break;
	case RESET_RELAY:
		switch(cmd[1]) {
		case RELAY_STM32:
			
			break;
		case RELAY_DEPLOY1:
			
			break;
		case RELAY_DEPLOY2:
			
			break;
		}
		// send ACK (0x01)
		ser_relays.putc(0x01);
		break;
	}
}

void send_packet() {
	// TODO send rocket packet
}
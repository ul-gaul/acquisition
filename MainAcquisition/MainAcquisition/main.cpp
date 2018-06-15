/*
 * @author: Maxime Guillemette
 * @team: Gaul - Avionique
 * @date: 2018-05-31
 */


#include <mbed.h>
#include "GPS.h"
#include "BMP180.h"
#include "LSM303DLHC.h"
#include "L3GD20.h"
#include "SDFileSystem.h"
#include "rocket_packet.h"

// relay command related defines
#define COMMAND_LENGTH 3

#define GET_RELAY_STATE 0xA0
#define SET_RELAY 0xB0
#define RESET_RELAY 0xC0

#define RELAY_STM32 0x00
#define RELAY_DEPLOY1 0x01
#define RELAY_DEPLOY2 0x02
#define RELAY_PAYLOAD 0x03

// RF communication related defines
#define RF_SEND_PERIOD 0.1	// 10Hz

// GPS related defines
#define GPS_SAMPLE_PERIOD 1	// 1Hz

// 10DOF related defines
#define IMU10DOF_SAMPLE_PERIOD 1	// 1Hz
#define IMU10DOF_SDA PB_9
#define IMU10DOF_SCL PB_8

// relay GPIOs
DigitalOut r_set_stm32(PE_5);
DigitalOut r_reset_stm32(PE_6);
DigitalOut r_feedback_stm32(PE_4);

DigitalOut r_set_deploy1(PD_2);
DigitalOut r_reset_deploy1(PD_1);
DigitalOut r_feedback_deploy1(PD_6);

DigitalOut r_set_deploy2(PE_0);
DigitalOut r_reset_deploy2(PE_1);
DigitalOut r_feedback_deploy2(PE_2);

DigitalOut r_set_payload(PD_0);
DigitalOut r_reset_payload(PD_5);
DigitalOut r_feedback_payload(PD_7);

// LED GPIOS
DigitalOut led1(PE_10);
DigitalOut led2(PE_11);
DigitalOut led3(PE_12);
DigitalOut led4(PE_13);
DigitalOut led5(PE_14);
DigitalOut led6(PE_15);

// serial ports
Serial ser_rfcomm(PD_8, PD_9, 9600);
Serial ser_relays(PC_10, PC_11, 9600);

// GPS object
GPS gps(PA_9, PA_10);

// I2C object for the communication with the 10DOF IMU breakout
// This I2C bus will communicate with the L3DG20H (gyroscope),
// LSM303DLHC (accelerometer) and the BMP180 (altitude and temperature)
I2C i2cbus(IMU10DOF_SDA, IMU10DOF_SCL);

// create 10DOF related sensors objects
BMP180 bmp180(&i2cbus);
LSM303DLHC lsm303(IMU10DOF_SDA, IMU10DOF_SCL);
L3GD20 l3gd20(IMU10DOF_SDA, IMU10DOF_SCL);
int ground_pressure;

// SD card object
//SDFileSystem sd(PA_7, PA_6, PA_5, PA_4, "gaulfs");
//char filename[] = "/gaulfs/data.csv";
FILE* fd;

// Ticker (timer) object to send data to ground station
//Ticker rf_ticker;
//Ticker gps_ticker;
//Ticker imu190dof_ticker;

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
char rocket_packet_serialized[57];
unsigned int sizeof_rocket_packet;

// global time (in ms) variable
volatile unsigned long  _millis;
// configure systick handler for millis timer
extern "C" void SysTick_Handler(void) {
	_millis++;
}

// public functions
void ser_relay_handler(void);
void decode_relay_command(uint8_t* cmd, uint8_t b);
void execute_relay_command(uint8_t* cmd);
void serialize_rocket_packet(RocketPacket pkt, char* s);
void send_packet();
void update_gps();
void update_10dof();
float get_temperature();
int get_pressure();
float get_altitude(int groundpressure);
void save_data();
void millisStart(void);
unsigned long millis(void);

//SPI spi(PA_7, PA_6, PA_5);
//DigitalOut cs(PA_4);

int main() {
	// start millis timer
	millisStart();
	// attach various interrupts to timers and peripherals
	ser_relays.attach(&ser_relay_handler);
	//	rf_ticker.attach(&send_packet, RF_SEND_PERIOD);
	//	gps_ticker.attach(&update_gps, GPS_SAMPLE_PERIOD);
	//imu190dof_ticker.attach(&update_10dof, IMU10DOF_SAMPLE_PERIOD);
	// TODO remove the led eventually
	DigitalOut led0(PD_15);
	// initialize sensors
	int bmp180_err = bmp180.init();
	ground_pressure = get_pressure();
	update_10dof();
	ground_pressure = get_pressure();
	sizeof_rocket_packet = sizeof(unsigned long)
								+ 13 * sizeof(float)
								+ sizeof(int8_t);
	size_t size_struct = sizeof(RocketPacket);
	ser_rfcomm.putc((int8_t) 10);
	for (;;) {
//		ser_rfcomm.putc((int8_t) 0x90);
		update_gps();
		if(bmp180_err != 0) {
			// retry to init bmp180
			bmp180_err = bmp180.init();
		} else {
			update_10dof();
		}
		rocket_data.timestamp = millis();
		rocket_packet.data = rocket_data;
		send_packet();
		//save_data(); no SD card, no need to call this function
		led0 = !led0;
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
		if(b == RELAY_STM32 || b == RELAY_DEPLOY1 || b == RELAY_DEPLOY2 || b == RELAY_PAYLOAD) {
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
	int s;
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
		case RELAY_PAYLOAD:
			ser_relays.putc(r_feedback_payload.read());
			break;
		}
		return;
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
		case RELAY_PAYLOAD:
			r_reset_payload = 0;
			r_set_payload = 1;
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
		case RELAY_PAYLOAD:
			r_set_payload = 0;
			r_reset_payload = 1;
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
	// timestamp
	memcpy(s + offset, (void *) &pkt.data.timestamp, sizeof(pkt.data.timestamp));
	offset += sizeof(pkt.data.timestamp);
	// GPS values
	memcpy(s + offset, (void *) &pkt.data.latitude, sizeof(pkt.data.latitude));
	offset += sizeof(pkt.data.latitude);
	memcpy(s + offset, (void *) &pkt.data.longitude, sizeof(pkt.data.longitude));
	offset += sizeof(pkt.data.longitude);
	memcpy(s + offset, (void *) &pkt.data.altitude, sizeof(pkt.data.altitude));
	offset += sizeof(pkt.data.altitude);
	memcpy(s + offset, (void *) &pkt.data.temperature, sizeof(pkt.data.temperature));
	offset += sizeof(pkt.data.temperature);
	memcpy(s + offset, (void *) &pkt.data.x_accel, sizeof(pkt.data.x_accel));
	offset += sizeof(pkt.data.x_accel);
	memcpy(s + offset, (void *) &pkt.data.y_accel, sizeof(pkt.data.y_accel));
	offset += sizeof(pkt.data.y_accel);
	memcpy(s + offset, (void *) &pkt.data.z_accel, sizeof(pkt.data.z_accel));
	offset += sizeof(pkt.data.z_accel);
	memcpy(s + offset, (void *) &pkt.data.x_magnet, sizeof(pkt.data.x_magnet));
	offset += sizeof(pkt.data.x_magnet);
	memcpy(s + offset, (void *) &pkt.data.y_magnet, sizeof(pkt.data.y_magnet));
	offset += sizeof(pkt.data.y_magnet);
	memcpy(s + offset, (void *) &pkt.data.z_magnet, sizeof(pkt.data.z_magnet));
	offset += sizeof(pkt.data.z_magnet);
	memcpy(s + offset, (void *) &pkt.data.x_gyro, sizeof(pkt.data.x_gyro));
	offset += sizeof(pkt.data.x_gyro);
	memcpy(s + offset, (void *) &pkt.data.y_gyro, sizeof(pkt.data.y_gyro));
	offset += sizeof(pkt.data.y_gyro);
	memcpy(s + offset, (void *) &pkt.data.z_gyro, sizeof(pkt.data.z_gyro));
	offset += sizeof(pkt.data.z_gyro);
	// compute checksum
	pkt.checksum = 0;
	for(int i = 0 ; i < offset ; i++) {
		pkt.checksum += s[i];
	}
	pkt.checksum = ~pkt.checksum;
	memcpy(s + offset, (void *) &pkt.checksum, sizeof(pkt.checksum));
}

void send_packet() {
	serialize_rocket_packet(rocket_packet, rocket_packet_serialized);
	// wait for serial to be available
	while(!ser_rfcomm.writeable());
	// send start byte
	ser_rfcomm.putc((int8_t) 's');
	for(unsigned int i = 0; i < sizeof_rocket_packet; i++) {
		while(!ser_rfcomm.writeable());
		ser_rfcomm.putc((int8_t) rocket_packet_serialized[i]);
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
	// update accelerometer and magnetometer values
	lsm303.read(&rocket_data.x_accel, &rocket_data.y_accel, &rocket_data.z_accel,
				&rocket_data.x_magnet, &rocket_data.y_magnet, &rocket_data.z_magnet);
	// update temperature and altitude values
	rocket_data.temperature = get_temperature();
	rocket_data.altitude = get_altitude(ground_pressure);
	// update gyroscope values
	l3gd20.read(&rocket_data.x_gyro, &rocket_data.y_gyro, &rocket_data.z_gyro);
}

float get_temperature() {
	float temperature;
	bmp180.startTemperature();
	wait_ms(5);	// wait for conversion to complete
	if(bmp180.getTemperature(&temperature) != 0) {
		return -1.0;
	}
	return temperature;
}

int get_pressure() {
	int pressure;
	bmp180.startPressure(BMP180::ULTRA_LOW_POWER);
	wait_ms(10);
	if(bmp180.getPressure(&pressure) != 0) {
		return -1;
	}
	return pressure;
}

float get_altitude(int groundpressure) {
	int pressure;
	float a;
	pressure = get_pressure();
	a = 44330.0 * (1 - pow(((float) pressure / groundpressure), 1.0 / 5.255));
	return a;
}

void save_data() {
	fprintf(fd, "%lu, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n",
		rocket_data.timestamp, rocket_data.latitude, rocket_data.longitude,
		rocket_data.altitude, rocket_data.temperature, 
		rocket_data.x_accel, rocket_data.y_accel, rocket_data.z_accel,
		rocket_data.x_magnet, rocket_data.y_magnet, rocket_data.y_magnet,
		rocket_data.x_gyro, rocket_data.y_gyro, rocket_data.z_gyro);
}

void millisStart(void) {
	SysTick_Config(SystemCoreClock / 1000);
}
 
unsigned long millis(void) {
	return _millis;
}
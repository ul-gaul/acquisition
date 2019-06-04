/* 
 * Author: laplace
 *
 * Brief:
 * 		Abstraction layer for the SD card drivers.
 */

#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_fatfs.h"
#include "rocket_packet.h"
#include "math.h"

#define FILENAME "test_write_rocket_packet2.csv"
#define DATA_HEADER "\"timestamp\","\
					"\"latitude\","\
					"\"longitude\","\
					"\"NSIndicator\","\
					"\"EWIndicator\","\
					"\"UTCTime\","\
					"\"altitude\","\
					"\"pressure\","\
					"\"temperature\","\
					"\"acc_x_uncomp\","\
					"\"acc_y_uncomp\","\
					"\"acc_z_uncomp\","\
					"\"acc_x\","\
					"\"acc_y\","\
					"\"acc_z\","\
					"\"mag_x\","\
					"\"mag_y\","\
					"\"mag_z\","\
					"\"x_gyro\","\
					"\"y_gyro\","\
					"\"z_gyro\"\n"
#define DATA_LINE "%s,%s,%s,%s,%s,%s,%s,%lu,%s,%d,%d,%d,%s,%s,%s,%d,%d,%d,%d,%d,%d\n"
#define RD_BUFFER_SIZE 128

typedef struct {
	size_t r;
	size_t w;
	RocketData rd[RD_BUFFER_SIZE];
} RocketDataCircBuf;

/* open and set the file pointer to the end of the file */
FRESULT open_append(FIL* fp, const char* path);

/* write a float as a string to a buffer */
void float_to_string(float a, char* buf, uint8_t res);

/* mount the sd card and create a file for writing */
int sd_card_init();

/* copy rocket data to buffer */
int sd_card_add_rd(RocketDataCircBuf* rdb, RocketData* rd);

/* write all available rocket data structs to the file */
//int sd_card_write_rocket_data(RocketDataCircBuf* rd, int is_open);

/* write a rocket data to the sd card */
int sd_card_write_rocket_data(RocketData* rd);

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
					"\"z_gyro\"\n"\
#define DATA_LINE "%s,%s,%s,%s,%s,%s,%s,%lu,%s,%d,%d,%d,%s,%s,%s,%d,%d,%d,%d,%d,%d\n"


/* mount the sd card and create a file for writing */
int sd_card_init();

/* write all available rocket packets to the file */
void sd_card_write_rocket_packets();

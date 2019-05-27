#include "sd_card.h"


/* private variables */
FATFS fat_fs;
FIL fd;
uint32_t sd_total_space;
uint32_t sd_free_space;
char filename[256] = "data.csv";

void float_to_string(float a, char* buf, uint8_t res);
int w_rd_line(FIL* fd, RocketData rd);

int sd_card_init() {
	int ret;
	

	/* mount FAT filesystem */
	ret = f_mount(&fat_fs, "", 1);
	if (ret != FR_OK) {
		goto close;
	}

	/* determine filename from existing files */
	// TODO

	/* create file and write header */
	ret = f_open(&fd, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if (ret != FR_OK) {
		goto close;
	}
	ret = f_puts("2019\n", &fd);
	if (ret < 0) {
		goto close;
	}
	ret = f_puts(DATA_HEADER, &fd);
	if (ret <= 0) {
		goto close;
	}

close:
	f_close(&fd);
	f_mount(0, "", 1);
	return ret;
}

int sd_card_add_rd(RocketDataCircBuf* rdb, RocketData* rd) {
	// TODO make sure this is a deep copy,
	// otherwise all the packets in the buffer will be the same
	rdb->rd[rdb->w].timestamp = rd->timestamp;
	rdb->rd[rdb->w].latitude = rd->latitude;
	rdb->rd[rdb->w].longitude = rd->longitude;
	rdb->rd[rdb->w].NSIndicator = rd->NSIndicator;
	rdb->rd[rdb->w].EWIndicator = rd->EWIndicator;
	rdb->rd[rdb->w].altitude = rd->altitude;
	rdb->rd[rdb->w].pressure = rd->pressure;
	rdb->rd[rdb->w].temperature = rd->temperature;
	rdb->rd[rdb->w].acc_x_uncomp = rd->acc_x_uncomp;
	rdb->rd[rdb->w].acc_y_uncomp = rd->acc_y_uncomp;
	rdb->rd[rdb->w].acc_z_uncomp = rd->acc_z_uncomp;
	rdb->rd[rdb->w].acc_x = rd->acc_x;
	rdb->rd[rdb->w].acc_y = rd->acc_y;
	rdb->rd[rdb->w].acc_z = rd->acc_z;
	rdb->rd[rdb->w].mag_x = rd->mag_x;
	rdb->rd[rdb->w].mag_y = rd->mag_y;
	rdb->rd[rdb->w].mag_z = rd->mag_z;
	rdb->rd[rdb->w].x_gyro = rd->x_gyro;
	rdb->rd[rdb->w].y_gyro = rd->y_gyro;
	rdb->rd[rdb->w].z_gyro = rd->z_gyro;

	rdb->w++;
}

int sd_card_write_rocket_packets(RocketDataCircBuf* rdb, int is_open) {
	int err;

	/* mount sd card and open file where we left off */
	// TODO use f_sync(&fd); to sync cached changed to disk

	if (is_open == 0) {
		err = f_mount(&fat_fs, "", 1);
		if (err != FR_OK) {
			goto close;
		}

		/* create file and write header */
		err = f_open(&fd, filename, FA_OPEN_APPEND | FA_READ | FA_WRITE);
		if (err != FR_OK) {
			goto close;
		}
	}

	for (;;) {
		rdb.r++;
		if (rdb.r >= RD_BUFFER_SIZE)
			rdb.r = 0;
		if (rdb.r == rdb.w)
			break;
		res = w_rd_line(&fd, rdb.rd[rdb.r]);
		if (ret <= 0) {
			goto close;
		}
	}

	// write cached changes to disk to that 
	// they are saved in case of a crash
	f_sync(&fd);

	return 1;

close:
	f_close(&fd);
	f_mount(0, "", 1);
	return 0;
}

int w_rd_line(FIL* fd, RocketData rd) {
	char fstr[16];

	float_to_string(rd.timestamp, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.latitude, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.longitude, fstr, 4);
	f_printf(fd, "%s,", fstr);
	f_printf(fd, "%s,", rd.NSIndicator);
	f_printf(fd, "%s,", rd.EWIndicator);
	float_to_string(rd.altitude, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.pressure, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.temperature, fstr, 4);
	f_printf(fd, "%s,", fstr);
	f_printf(fd, "%d,", rd.acc_x_uncomp);
	f_printf(fd, "%d,", rd.acc_y_uncomp);
	f_printf(fd, "%d,", rd.acc_z_uncomp);
	float_to_string(rd.acc_x, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.acc_y, fstr, 4);
	f_printf(fd, "%s,", fstr);
	float_to_string(rd.acc_z, fstr, 4);
	f_printf(fd, "%s,", fstr);
	f_printf(fd, "%d,", rd.mag_x);
	f_printf(fd, "%d,", rd.mag_y);
	f_printf(fd, "%d,", rd.mag_z);
	f_printf(fd, "%d,", rd.x_gyro);
	f_printf(fd, "%d,", rd.y_gyro);
	f_printf(fd, "%d\n", rd.z_gyro);

	return 0;
}

void float_to_string(float a, char* buf, uint8_t res) {
	int int_part;
	int dec_part;
	int_part = (int) a;
	dec_part = (int) ((a - int_part) * pow(10, res));
	sprintf(buf, "%d.%d", int_part, dec_part);
}

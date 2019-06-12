#include "sd_card.h"


/* private variables */
FATFS fat_fs;
FIL fd;
uint32_t sd_total_space;
uint32_t sd_free_space;
int sd_card_is_open;
DIR cwd;
FILINFO cfi;
char filename[FILENAME_SIZE];

int w_rd_line(FIL* fd, RocketData rd);

FRESULT open_append(FIL* fp, const char* path) {
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

void get_filename(char* fname) {
	int fcount = 0;
	int ret;

	/* determine filename from existing files */
	f_opendir(&cwd, "/");
	for (;;) {
		ret = f_readdir(&cwd, &cfi);
		if (ret != FR_OK || cfi.fname[0] == 0) {
			// break on error or end of dir
			break;
		}
		if (!(cfi.fattrib & AM_DIR)) {
			// current file info is a file (not directory)
			fcount++;
		}
	}
	f_closedir(&cwd);
	// create filename
	memset(fname, 0, FILENAME_SIZE);
	sprintf(fname, FILENAME, fcount);
}

int sd_card_init() {
	int ret;

	/* mount FAT filesystem */
	ret = f_mount(&fat_fs, "", 1);
	if (ret != FR_OK) {
		goto close;
	}

	get_filename(filename);

	/* create file and write header */
	ret = f_open(&fd, filename, FA_WRITE | FA_OPEN_ALWAYS);
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

	// success
	sd_card_is_open = 1;
	return 0;

close:
	sd_card_is_open = 0;
	f_close(&fd);
	f_mount(0, "", 1);
	return ret;
}

int sd_card_write_rocket_data(RocketData* rd) {
	int err;

	if (sd_card_is_open != 1) {
		err = f_mount(&fat_fs, "", 1);
		if (err != FR_OK) {
			goto close;
		}

		get_filename(filename);

		err = open_append(&fd, filename);
		if (err != FR_OK) {
			goto close;
		}
		sd_card_is_open = 1;
	}

	err = w_rd_line(&fd, *rd);
	if (err != 0) {
		goto close;
	}
	// write cached changes to disk to that
	// they are saved in case of a crash
	f_sync(&fd);

	return 0;

close:
	sd_card_is_open = 0;
	f_close(&fd);
	f_mount(0, "", 1);
	return err;
}

int w_rd_line(FIL* fd, RocketData rd) {
	char fstr[16];

	memset(fstr, 0, 16);
	float_to_string(rd.timestamp, fstr, 4);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.latitude, fstr, 6);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.longitude, fstr, 6);
	f_printf(fd, "%s,", fstr);

	f_printf(fd, "%c,%c,", rd.NSIndicator, rd.EWIndicator);

	memset(fstr, 0, 16);
	float_to_string(rd.UTCTime, fstr, 6);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.altitude, fstr, 4);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.pressure, fstr, 4);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.temperature, fstr, 4);
	f_printf(fd, "%s,", fstr);

	f_printf(fd, "%d,%d,%d,", rd.acc_x_uncomp, 
			rd.acc_y_uncomp, rd.acc_z_uncomp);

	memset(fstr, 0, 16);
	float_to_string(rd.acc_x, fstr, 4);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.acc_y, fstr, 4);
	f_printf(fd, "%s,", fstr);

	memset(fstr, 0, 16);
	float_to_string(rd.acc_z, fstr, 4);
	f_printf(fd, "%s,", fstr);

	f_printf(fd, "%d,%d,%d,%d,%d,%d\n", 
			rd.mag_x, rd.mag_y, rd.mag_z, 
			rd.x_gyro, rd.y_gyro, rd.z_gyro);

	return 0;
}

void float_to_string(float a, char* buf, uint8_t res) {
	int int_part;
	int dec_part;
	int_part = (int) a;
	dec_part = abs((int) ((a - int_part) * pow(10, res)));
	sprintf(buf, "%d.%d", int_part, dec_part);
}

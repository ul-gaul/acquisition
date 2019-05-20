#include "sd_card.h"


/* private variables */
FATFS fat_fs;
FIL fd;
uint32_t sd_total_space;
uint32_t sd_free_space;
char filename[256] = "data.csv";


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
	ret = f_puts("2019\n");
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

void sd_card_write_rocket_packets(RocketDataCircBuf* rd) {
	int err;

	/* mount sd card and open file where we left off */
	// TODO open file and append to it to not overwrite 
	// previous contents
	err = f_mount(&fat_fs, "", 1);
	if (err != FR_OK) {
		goto close;
	}

	/* create file and write header */
	err = f_open(&fd, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if (err != FR_OK) {
		goto close;
	}
close:
	f_close(&fd);
	f_mount(0, "", 1);
}

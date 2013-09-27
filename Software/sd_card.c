#include "sd_card.h"

int openSdCard(){
	sdDev = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	if(sdDev != NULL && alt_up_sd_card_is_FAT16()){
		return 0;
	} else {
		return -1;
	}
}

short int openFile(char *fileName) {
	return alt_up_sd_card_fopen(fileName, 0);
}

int closeFile(short int fh) {
	return alt_up_sd_card_fclose(fh);
}

unsigned char readByte(short int fh) {
	return (unsigned char) alt_up_sd_card_read(fh);
}
short int readWord(short int fh) {
	short int byte1, byte2;

	byte1 = alt_up_sd_card_read(fh);
	byte2 = alt_up_sd_card_read(fh);

	if (byte1 == -1 || byte2 == -1)
		return -1;

	return ((unsigned short int)byte2 << 8) | (unsigned short int)byte1;
}

int readDWord(short int fh) {
	short int byte1, byte2, byte3, byte4;

	byte1 = alt_up_sd_card_read(fh);
	byte2 = alt_up_sd_card_read(fh);
	byte3 = alt_up_sd_card_read(fh);
	byte4 = alt_up_sd_card_read(fh);

	if (byte1 == -1 || byte2 == -1 || byte3 == -1 || byte4 == -1)
			return -1;

	return ((unsigned short int)byte4 << 24) | ((unsigned short int)byte3 << 16) | ((unsigned short int)byte2 << 8) | (unsigned short int)byte1;
}

unsigned int getFileLength(const char *fileName) {
	unsigned int fileLength = 0;

	short int fileHandle = openFile(fileName);
	if (fileHandle == -1) {
		printf("Error occured, unable to open file in 'getFileLength' with name: %s", fileName);
	}

	while (readByte(fileHandle) != -1) {
		fileLength++;
	}

	closeFile(fileName);
	return fileLength;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "audio.h"
#include "sd_card.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"

//#include "audio_up_hack.h"

const char *CONFIG_NAME = "/dev/audio_and_video_config_0"; // may need to change to verilog version

const char *AUDIO_NAME = "/dev/audio_0"; // may need to change to verilog version
alt_up_audio_dev *audio = NULL;

alt_up_av_config_dev *config = NULL;

int DEBUG = 0;//1;
int toneLength = 122;
unsigned int tone[122];

unsigned int *laserBuffer;
unsigned int laserBufferLength;

void setupAudio()
{
	config = (alt_up_av_config_dev *)alt_up_av_config_open_dev(CONFIG_NAME);
	if (config == NULL) {
		printf("Error: audio video config could not be opened.\n");
	}
	while (!alt_up_av_config_read_ready(config)) {
	}

	audio = (alt_up_audio_dev *)alt_up_audio_open_dev(AUDIO_NAME);
	if (config == NULL) {
		printf("Error: audio codec could not be opened.\n");
	}

	int amp = 10000000;
	//printf("Amp is %d\n", amp);
	int i;
	for (i = 0; i < toneLength; i++) {
		tone[i] = amp * sin((float)i * 3.141592 / 122.0);//* (PI / 8.0));
		//tone[i] = i % 2 == 0 ? 0x00000FFF : 0x00000000;
	}

	// Setup interrupts
	alt_up_audio_disable_read_interrupt(audio);
	alt_up_audio_enable_write_interrupt(audio);

	if (DEBUG == 1) {
		printf("Successfully setup sound.\n");
	}
}

// Need to check if we write words or bytes to the FIFOs
void playAudio(unsigned int *leftBuffer, int leftLength, unsigned int *rightBuffer, int rightLength)
{
	// check that fifos have space
	int leftWritten = alt_up_audio_write_fifo(audio, leftBuffer, leftLength, ALT_UP_AUDIO_LEFT);
	int rightWritten = alt_up_audio_write_fifo(audio, rightBuffer, rightLength, ALT_UP_AUDIO_RIGHT);

	//int leftWritten = alt_up_audio_play_l(audio, leftBuffer, leftLength);
	//int rightWritten = alt_up_audio_play_r(audio, rightBuffer, rightLength);

	if (DEBUG == 1) {
		printf("Wrote %d to left audio FIFO. with value\n", leftWritten, *leftBuffer);
		printf("Wrote %d to right audio FIFO.\n", rightWritten, *rightBuffer);
	}
}

void audioTest()
{
	setupAudio();
	printf("Starting audio test\n");
	int length = 8;
	unsigned int right [length];
	unsigned int left [length];
	int i;

	for (i = 0; i < length; i++) {
		right[i] = (i * i * i) & 0x0000FFFF;
		left[i] = right[i];
	}

	unsigned int *rightBuffer = right;
	unsigned int *leftBuffer = left;

	//sin (param*PI/180); for degrees, it uses radians

	// Should use a timer to measure how long it takes to stick a byte in and whatever
	// Is it easier (processor wise) to move in larger chunks, or is there no difference?

	//alt_up_audio_reset_audio_core(audio);
	int free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
	printf("%d words free in right FIFO\n", free);
	// printf("Entering loop");

	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		//printf("%d words free in right FIFO\n", free);
		if (free >= length) {
			//printf("Playing audio\n");
			playAudio(leftBuffer, length, rightBuffer, length);
		}
//		else {
//			if (DEBUG == 1) {
//				//printf("Skipped audio write\n");
//			}
//		}
	}
}


void testTone(void)
{
	unsigned int *toneBuffer = tone;
	//int toneLen = 360;
	int free, len;
	int wrap = 0;

	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		if (free > 1) {
			if ((int)toneBuffer + free >= toneLength) {
				// Wrap around
				len = toneLength - free;
				wrap = 1;
			} else {
				len = free;
				wrap = 0;
			}
			playAudio(toneBuffer, len, toneBuffer, len);
			toneBuffer = wrap == 1 ? tone : toneBuffer + len;
		}
	}
}

void playLaser1(void) {
	unsigned int fileWordLength = 38384;//laserii//(getWavFileLength("laseri.wav") / 2);// laser i = 26200;/*looked in hex file *///
	printf("File Length is: %x\n", fileWordLength);
	readWavFile("laserii.wav", fileWordLength, laserBuffer);

	int free, len;
	int wrap = 0;
	unsigned int *cursor = laserBuffer;
	for (;;) {
		free = alt_up_audio_write_fifo_space(audio, ALT_UP_AUDIO_RIGHT);
		if (free > 1) {
			if ((int)cursor + free >= (int)laserBuffer + (2 *fileWordLength)) {
				// Wrap around
				len = fileWordLength - free;
				wrap = 1;
			} else {
				len = free;
				wrap = 0;
			}
			playAudio(cursor, len, cursor, len);
			cursor = wrap == 1 ? laserBuffer : cursor + len;
		}
	}
}


void readWavFile(char *wavFileName, unsigned int fileWordLength, unsigned int *buffer) {
	laserBuffer = malloc(fileWordLength * 2); //words are 2 bytes // this line should be changed

	short int fileHandle = openFile(wavFileName);
	if (fileHandle == -1) {
		printf("Error opening %s\n", wavFileName);
		return;
	}

	readPastWavHeader(fileHandle);

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int word = readWord(fileHandle);
	printf("first word is %x\n", word);
	//while (word != -1) {
	while (i < fileWordLength) {
		laserBuffer[i++] = word;
		word = readWord(fileHandle);
	}
	printf("reached EOF\n");

	closeFile(fileHandle);
	return;
}


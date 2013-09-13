#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include "system.h"
#include "../timer.h"

#define MATRIX_SIZE 100

void setupMatrixA(int matrix[MATRIX_SIZE][MATRIX_SIZE])
{
	int i;
	int j;

	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j ++) {
			matrix[i][j] = i * j;
		}
	}
}

void setupMatrixB(int matrix[MATRIX_SIZE][MATRIX_SIZE])
{
	int i;
	int j;

	for (i = MATRIX_SIZE - 1; i >= 0; i--) {
		for (j = 0; j < MATRIX_SIZE; j ++) {
			matrix[i][j] = i * j;
		}
	}
}

void matrixMultiply(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE][MATRIX_SIZE])
{
	int i, j, k;
	int sum;
	int c[100][100];

	for (i = 0; i < MATRIX_SIZE; i++) {
		for (j = 0; j < MATRIX_SIZE; j++) {
			sum = 0;
			for (k = 0; k < MATRIX_SIZE; k++) {
				sum = sum + a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
	}
}

void timeMatrixMultiply(void)
{
	int a[MATRIX_SIZE][MATRIX_SIZE];
	int b[MATRIX_SIZE][MATRIX_SIZE];
	int startTime, endTime, ticks;
	float freq, time;
	setupMatrixA(a);
	setupMatrixB(b);

	startTime = (int)alt_timestamp();
	matrixMultiply(a, b);
	endTime = (int)alt_timestamp();
	ticks = endTime - startTime;
	freq = (float)alt_timestamp_freq();
	time = (float)ticks / freq;

	printf("Time taken: %d clock ticks\n", ticks);
	printf("            %f seconds\n", time);
}

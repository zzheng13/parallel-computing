/*
 ============================================================================
 Name        : partB.c
 Author      : Zimo Zheng
 Version     :
 Copyright   :
 Description : Serial version of calculate histogram program
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <pthread.h>

#define NUMBINS 1000

int *histogramVals;    /*global histogram array for all threads*/
float binSize;		   /*bin size calculated from min and max of array values*/

float maxVal = FLT_MIN;// the maximum and minimun value of all the data.
float minVal = FLT_MAX;

float rawData[1000000];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *calcHistogram (void *startnum);





int main(void) {

	clock_t begin = clock();

	histogramVals =  malloc(NUMBINS * sizeof(int));

	FILE *fileIn = fopen("numbers.bin", "rb");
	if (fileIn == NULL){
		printf("File not opened\n");
		return 0;
	}

	int cnt = 1000000;
	int resultCode;

	resultCode = fread(rawData, sizeof(rawData[0]), sizeof(rawData)/sizeof(rawData[0]), fileIn);

	fclose(fileIn);



	int cnt2;
	for(cnt2 = 0; cnt2 < cnt; cnt2++ ) {
		if ( rawData[cnt2] > maxVal ) maxVal = rawData[cnt2];
		if ( rawData[cnt2] < minVal ) minVal = rawData[cnt2];
	}


	binSize = (maxVal-minVal)/(float)NUMBINS;
	printf("minVal: %f  maxVal: %f  binSize: %f\n", minVal, maxVal, binSize);

	// create four threads in main, and each thread reads in certain part of the rawData.
	// And calculate Histogram result is in a global variable histogramVals.
	pthread_t tid1;
	int startnum1 = 0; 
	pthread_create(&tid1, NULL, calcHistogram, &startnum1);

	pthread_t tid2;
	int startnum2 = 250000;
	pthread_create(&tid2, NULL, calcHistogram, &startnum2);

	pthread_t tid3;
	int startnum3 = 500000;
	pthread_create(&tid3, NULL, calcHistogram, &startnum3);

	pthread_t tid4;
	int startnum4 = 750000;
	pthread_create(&tid4,NULL,calcHistogram,&startnum4); 

	// join all the threads.
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	pthread_join(tid4,NULL);





	int cnt1;
	for (cnt1 = 0; cnt1 < NUMBINS; cnt1++){
		printf("bin: %d  binStart: %f   binEnd:  %f  Histogram Value: %d\n", cnt1, binSize*(float)cnt1, binSize*(float)(cnt1+1), histogramVals[cnt1]);
	}

	clock_t end = clock(); 
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf(" time is %lf\n", time_spent);

	
	return EXIT_SUCCESS;
}


/*-------------------------------------------------------------------
 * Function:    calcHistogram
 * Purpose:     Calculate Histogram result is in a global variable histogramVals.
 * In args:    startnum:The start number of each thread to calculate the histogram.
 * Outputs:     Histogram of values stored in global histogramVals
				binSize in global variable.
 *
 */
void *calcHistogram (void *startnum){
	int index = *(int *)startnum;
	int endnum = index + 250000;
	printf("the endnum is %d",endnum);
	for (index; index < endnum; index++){
		int binNum = (int)((rawData[index] - minVal) / binSize);
		pthread_mutex_lock(&mutex);
		
		histogramVals[binNum]++;

		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

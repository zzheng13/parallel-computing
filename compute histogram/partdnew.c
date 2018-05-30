/*
 ============================================================================
 Name        : partD.c
 Author      : zimo zheng
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
#define threads_num 4 


float rawData[1000000];
float maxVal = FLT_MIN;
float minVal = FLT_MAX;
float binSize;	



// ***************************************************************
struct Hisgram{
	int startnum;
	int data[NUMBINS]; 
	
};

/*-------------------------------------------------------------------
 * Function:    calcHistogram
 * Purpose:     Calculate Histogram result is in a global variable histogramVals.
 * In args:     data:  values to be histogrammed.
 *              numDataPoints:  Number of data points to be histogrammed.
 *              numBins:  Number of bins in output histogram
 * Outputs:     Histogram of values stored in global histogramVals
				binSize in global variable.
 *
 */
void *calcHistogram (void *h){

	struct Hisgram *hisgram = (struct Hisgram*)h;

	int index = hisgram->startnum;

	int *histogramVals = hisgram->data; 

    
	int endnum = index + 250000;
	
	int ind;
	for (ind = index; ind < endnum; ind++){

		int binNum = (int)((rawData[ind] - minVal) / binSize);
	
		histogramVals[binNum]++;

	}
	pthread_exit(NULL);
}



int main(void){


	clock_t begin = clock(); // begin the clock 

	pthread_t thread[4]; 
	struct Hisgram cal_his[4];
	int finalarray[NUMBINS];
	int array1[NUMBINS];
	int array2[NUMBINS];
	int index; 


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



	// Initialize array from cal_his 
	int thnum,numb;
	for(thnum = 0; thnum < 4; thnum++){
		for(numb = 0; numb < NUMBINS; numb++){
			cal_his[thnum].data[numb] = 0;
		}
	}


	// create pthread 
	int i,j;
	for(i = 0; i < threads_num; i++){

		// printf("index = %d\n", in );
		if(i %2 == 0){
			for(j = i; j < i + 2; j++){
				cal_his[j].startnum = j * 250000;
				pthread_create(&thread[j],NULL,calcHistogram, (void*) &cal_his[j]);
			}

		}

	}
	int t;
	for(t = 0; t < threads_num; t++){
	
		pthread_join(thread[t],NULL);
	}



	int binn;
	
	for(binn = 0; binn < NUMBINS; binn++){
		array1[binn]=cal_his[0].data[binn] + cal_his[1].data[binn];
		array2[binn]=cal_his[2].data[binn] + cal_his[3].data[binn];
		//finalarray[binn]= cal_his[0].data[binn] + cal_his[1].data[binn] + cal_his[2].data[binn] + cal_his[3].data[binn];
	}
	
	for (binn = 0; binn < NUMBINS; binn++){
		finalarray[binn] = array1[binn]+array2[binn];
	}
	

	int cnt1;
	for (cnt1 = 0; cnt1 < NUMBINS; cnt1++){
		printf("bin: %d  binStart: %f   binEnd:  %f  Histogram Value: %d\n", cnt1, binSize*(float)cnt1, binSize*(float)(cnt1+1), finalarray[cnt1]);
	}


	clock_t end = clock(); 
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf(" time is %lf\n", time_spent);

	
	return EXIT_SUCCESS;


}























































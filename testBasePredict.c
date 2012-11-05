/**********************************************************************
* Header files
***********************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

/**********************************************************************
* Golbal Variables
***********************************************************************/
#define TRUE  1
#define FALSE 0

#define numParam 4

#define numUsers 458293
#define numMovies 17770
//this is the number of points in all
//#define numDataPts 102416306
//this is the number of points in trainingBin.dta
//#define numDataPts 99666408
#define numDataPts 100

#define numIterations 1


float lrate = 0.1; 
float K = 0.02;
 
struct dataPt
{
	int user;
	short movie;
	short rating;
};  

static struct dataPt training[numDataPts];

static float *averageRating;
static float *averageOffset;




/*
r	Open the file for reading.
w	Open the file for overwriting. Creates a new file if it doesn't already exist.
a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
b	Opens the file as a binary file (the default without the "b" is a text file).
+	Open the file for reading and writing.
*/

/*
void main(){


	//load the pointer to the training points in binary
	FILE *baseOffBin = fopen("binary_movie_avgs.dta", "rb");
	FILE *baseOff = fopen("regularized_base_movie_avgs.dta", "r");

	float value;
	float data;
	char str[50];
	
	int i;
	for(i = 0; i < numDataPts; i++){
	
		//read in the line
		fscanf(baseOff, "%s", str);
		//convert each value to int and put it in data[]
		sscanf(str, "%f\n", &data);
		
		
		fwrite (&data, 4, 1, baseOffBin);
		
		
		//fread (&data, 4, 1, baseOffBin);
		printf ("%f\n", data);
		
		
		
		
		fread (&data, 4, 1, baseOffBin);
		printf( "%f\n", data);
		
	}
	
	fclose(baseOff);
	fclose(baseOffBin);
	
}
*/

static inline void load_baseline(){

	//load the avergae movie scores and average user offsets
	averageRating = (float *)malloc(numMovies);
	averageOffset = (float *)malloc(numUsers);	
	
	//load the pointer to the training points in binary
	FILE *avgRatingf = fopen("movieAvgBin.dta", "rb");
	FILE *avgOffsetf = fopen("userOffsetBin.dta", "rb");
	
	long i;
	for(i = 0; i < numMovies; i++){
			
		fread (averageRating, 4, 1, avgRatingf);
		fread (averageOffset, 4, 1, avgOffsetf);

	}
	for (i = numMovies; i < numUsers; i++){
		fread (averageOffset, 4, 1, avgOffsetf);
	}

	fclose(avgRatingf);
	fclose(avgOffsetf);
	
} 

void main(){
	//load the pointer to the training points in binary
	FILE *allPoints = fopen("trainingBin.dta", "rb");
	FILE *baseRat = fopen("baseRateBin.dta", "wb");

	long i;
	for(i = 0; i < numDataPts; i++){
			
			fread (&training[i].user, 4, 1, allPoints);
			fread (&training[i].movie, 2, 1, allPoints);
			fread (&training[i].rating, 2, 1, allPoints);
			
			if (i%100000 ==0){
				printf("%d\n\n",i);
			}
			
	}

	fclose(allPoints);
	
	printf("done loading!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");

	load_baseline();
	


	float rating;
	
	for ( i =0; i < numDataPts; i++){
	
		printf("%d\n\n", i);	
		rating = averageRating[training[i].movie - 1] + averageOffset[training[i].user - 1];
		fwrite (&rating, 4, 1, baseRat);
	}
	
	
}
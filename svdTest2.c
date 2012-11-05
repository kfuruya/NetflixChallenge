/**********************************************************************
* Header files
***********************************************************************/
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <math.h>

/**********************************************************************
* Golbal Variables
***********************************************************************/
#define TRUE  1
#define FALSE 0

#define numUsers 458293
#define numMovies 17770
//this is the number of points in all
//#define numDataPts 102416306
//this is the number of points in trainingBin.dta
#define numDataPts 99666408

#define numParam 3
#define numIterations 100


/*******************************************************
 * 		 Other Declarations
 ******************************************************/
 
float lrate = 0.001; 
float K = 0.02;
 
struct dataPt
{
	int user;
	short movie;
	short rating;
};  

static struct dataPt training[numDataPts];

static float userParams[numUsers][numParam];
static float movieParams[numMovies][numParam];

static float *averageRating;
static float *averageOffset;


/*******************************************************
 * 		 Function Declarations
 ******************************************************/

/* 
r	Open the file for reading.
w	Open the file for overwriting. Creates a new file if it doesn't already exist.
a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
b	Opens the file as a binary file (the default without the "b" is a text file).
+	Open the file for reading and writing.
*/



//predicts the rating based on the current value of the user and movie parameters
float predictRating(int us, short mov)
{

	float rat = 0;
	
	//take the dot product of the movie and user paramteres
	int i;
	for (i = 0; i < numParam; i++){
		rat += movieParams[mov - 1][i] * userParams[us- 1][i];
		if (rat < 1){
			rat = 1;
		} else if (rat > 5) {
			rat = 5;
		}
	}
	

	
	
	return rat;
 
}


//trains a specific parameter for one user and movie for a specific data point in all.dta, where mov and us are 0 indexed
static inline void train	(int us, short mov, short rat, short param, float error)
{

	float temp;
	
	temp  = userParams[us - 1][param];
	//userParams[us - 1][param] += lrate * (error * movieParams[mov - 1][param] - K * temp);
	//movieParams[mov - 1][param] += lrate * (error * temp - K * movieParams[mov - 1][param]);
	userParams[us - 1][param] += error * movieParams[mov - 1][param];
	movieParams[mov - 1][param] += error * temp;

}

static inline void load_training(){

	//load the pointer to the training points in binary
	FILE *allPoints = fopen("trainingBin.dta", "rb");

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
	
	
	
	
}

void saveParams(){

	//open the files which contain the user and movie parameters
	//	we open it as a binary file where we can read and write to it
	FILE *userp = fopen("userpref.dta", "wb");
	FILE *moviep = fopen("moviepref.dta", "wb");
	
	fwrite ( &userParams, 4 * numParam, numUsers, userp );
	fwrite ( &movieParams, 4 * numParam, numMovies, moviep );
	
	//close the files
	fclose(userp);
	fclose(moviep);
		
}
 
static inline void load_params(){
	
	//float paramSeed = sqrt(3.6/numParam);
	
	int index1, index2;
	for(index1 = 0; index1 < numUsers; index1++){
		for(index2 = 0; index2 < numParam; index2++){
			userParams[index1][index2] = ((float)(rand()%1001))/10000.0;
		}		
	}
	
	for(index1 = 0; index1 < numMovies; index1++){
		for(index2 = 0; index2 < numParam; index2++){
			movieParams[index1][index2] = ((float)(rand()%1001))/10000.0;
		}		
	}
}
 
//predicts the rating based on the current value of the user and movie parameters
float base_predictRating(int us, short mov)
{
	
	return averageRating[mov - 1] + averageOffset[us - 1];
 
}

static inline void load_baseline(){

	//load the avergae movie scores and average user offsets
	averageRating = (float *)malloc(numMovies * 4);
	averageOffset = (float *)malloc(numUsers * 4);	
	
	//load the pointer to the training points in binary
	FILE *avgRatingf = fopen("movieAvgBin.dta", "rb");
	FILE *avgOffsetf = fopen("userOffsetBin.dta", "rb");
	
	long i;
	for(i = 0; i < numMovies; i++){
			
		fread (&averageRating[i], 4, 1, avgRatingf);
		fread (&averageOffset[i], 4, 1, avgOffsetf);

	}
	for (i = numMovies; i < numUsers; i++){
		fread (&averageOffset[i], 4, 1, avgOffsetf);
	}

	fclose(avgRatingf);
	fclose(avgOffsetf);
	
} 
 
//trains a specific parameter for one user and movie for a specific data point in all.dta
static inline void initial_train	(int us, short mov, short rat, short param, float error){

	float temp;
	
	temp  = userParams[us - 1][param];
		
	userParams[us - 1][param] += error * movieParams[mov - 1][param];
	movieParams[mov - 1][param] += error * temp;
		
}

 
 
 
 
int main() 
{ 

	//load the paramter values initially
	load_params();
	
	//load the training set into memory
	load_training();
	
	//load baseline values
	load_baseline();
		
	int i;
	long j;
	short k;
	
	float err;

	//sdo the first training run with the predict rating with baseline
	for(j = 0; j < numDataPts; j++){
	
		err = lrate * (training[j].rating - predictRating(training[j].user, training[j].movie));
		
		for(k = 0; k < numParam; k++){
			initial_train(training[j].user, training[j].movie, training[j].rating, k, err);
		}
			
		//print shit so we know somethings happeneing, the ldigits will be the number of the iteration
		if (j%500000 == 0){
			printf("%d\n", j);
		}
	}
	
	free(averageRating);
	free(averageRating);
	
	
	//iterate through the data set this mamny times
	for(i = 1; i < numIterations; i++){
	
		//iterate through the training points
		for(j = 0; j < numDataPts; j++){
		
			err = lrate * (training[j].rating - predictRating(training[j].user, training[j].movie));
		
			for(k = 0; k < numParam; k++){
				train (training[j].user, training[j].movie, training[j].rating, k, err);
			}
			
			//print shit so we know somethings happeneing, the ldigits will be the number of the iteration
			if (j%500000 == 0){
				printf("%d\n", j + i);
			}
			
		}
		
		printf("\n");
		
		//save the paramters in the files "moviepref.dta" and "userpref.dta" after each iteration
		if (i%5 == 0 || i == numIterations - 1){
			saveParams();
		}
		
		if (i == 40){
			lrate = 0.0001;
		}
	}
	
	return 0;
}
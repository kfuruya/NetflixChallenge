/**********************************************************************
* Header files
***********************************************************************/

#include <stdio.h> 
#include <string.h>

/**********************************************************************
* Golbal Variables
***********************************************************************/
#define TRUE  1
#define FALSE 0

#define numParam 5

#define numUsers 458293
#define numMovies 17770
//this is the number of points in all
//#define numDataPts 102416306
//this is the number of points in trainingBin.dta
//#define numDataPts 99666408

#define numIterations 1


//this is for test purposes
#define numDataPts 20



/*
r	Open the file for reading.
w	Open the file for overwriting. Creates a new file if it doesn't already exist.
a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
b	Opens the file as a binary file (the default without the "b" is a text file).
+	Open the file for reading and writing.
*/


int main() 
{ 

	float movieParams[numParam];
	FILE *moviep = fopen("userpref.dta", "rb");
	double specificMovieOffsetAddr;
	
	
	int i, j;
	for(i = 0; i < 10; i++){
	
		//find the offset number of bytes of where the specific user or movie's parameters start being listed
		//	with respect to the base address of the beginning of the file
		specificMovieOffsetAddr = (i) * (4 * numParam);
	
		//change the stream pointer to the spot where the parameters for that user/movie starts
		fseek ( moviep , specificMovieOffsetAddr, SEEK_SET );
	
		

		fread(movieParams, 4, numParam, moviep);
		
		for (j = 0; j < numParam; j++){
			printf("%f\n", movieParams[j]);
		}
		
		printf("\n");
		

	}
	
	fclose(moviep);

	
	return 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*
void main(){

	//load the pointer to the training points in binary
	FILE *allPointsBin = fopen("trainingBin.dta", "rb");
	FILE *allPoints = fopen("all.dta", "r");

	int user;
	short movie;
	short rating;
	
	int j;
	
	char str[20];
	int data[4];
	
	int i;
	for(i = 0; i < 10; i++){
	
		for(j = 0; j < 4; j++){
				//read in the line
				fscanf(allPoints, "%s", str);
				//convert each value to int and put it in data[]
				data[j] = atoi(str);
				

				
			}
			
			if (data[3] != 0){
				printf("poop%d , ", data[0]);
				printf("%d , ", data[1]);
				printf("%d\n", data[3]);
			}
			
			//fseek(allPoints, 0, SEEK_SET);
			fread (&user, 4, 1, allPointsBin);
			fread (&movie, 2, 1, allPointsBin);
			fread (&rating, 2, 1, allPointsBin);
			
			
			printf("%d , ", user);
			printf("%d , ", movie);
			printf("%d\n\n", rating);
			
			
		}
	
	//printf("%d\n\n",startIndex);
	
	fclose(allPoints);
	fclose(allPointsBin);
	
}

*/
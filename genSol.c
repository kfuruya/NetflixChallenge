/**********************************************
* 
*		THIS FILE GENERATES THE SOLUTION FILE
*
***********************************************/
/* READ MEEEEEEEEEEE!!!!!!!!!!!!!!!!
* MAKE SURE TO CHANGE numParam to whatever the number of params is
*
* basically takes the dot product of the paramters for the passed movie and user
* and turns it into an ascii string and prints it to a rating.dta
*
* the paramters are in the files "userpref.dta" and "moviepref.dta" and are stored as 
* binary! (as 32 bit floats)
*
*/


#include <stdio.h> 
#include <string.h>

#define numParam 3
#define numPointsQual 2749898
/* 
r	Open the file for reading.
w	Open the file for overwriting. Creates a new file if it doesn't already exist.
a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
b	Opens the file as a binary file (the default without the "b" is a text file).
+	Open the file for reading and writing.
*/


int main() 
{ 

	
	//open the qual file
	FILE *qual = fopen("qual.dta", "r");
	//open the user amd movie parameter files
	FILE *userp = fopen("userpref.dta", "rb");
	FILE *moviep = fopen("moviepref.dta", "rb");
	//open the file to write the ratings into
	FILE *ratingf = fopen("ratings.dta", "w");

	
	long specificUserOffsetAddr;
	long specificMovieOffsetAddr;
	float movieParams[numParam];
	float userParams[numParam];
	
	//creates an array to put the data into
	int data[3];
	char str[20];
	
	int i, j, k;
	int temp = 0;

	
	char ratingStr[9];
	
	//go through qual
	for(i = 0; i < numPointsQual; i++){
		
		//go through each line in qual
		for(j = 0; j < 3; j++){
			fscanf(qual, "%s", str);
			
			//puts the user, movie, date, and rating value into the array as ints
			data[j] = atoi(str);
		}
		
		//find the offset number of bytes of where the specific user or movie's parameters start being listed
		//	with respect to the base address of the beginning of the file
		//specificUserOffsetAddr = (i) * (8 * numParam);
		specificUserOffsetAddr = (data[0] - 1) * (4 * numParam);
		specificMovieOffsetAddr = (data[1] - 1) * (4 * numParam);
		//specificMovieOffsetAddr = (i) * (8 * numParam);
	
		//change the stream pointer to the spot where the parameters for that user/movie starts
		fseek ( userp , specificUserOffsetAddr , SEEK_SET );
		fseek ( moviep , specificMovieOffsetAddr , SEEK_SET );
	
		//read the value of the paramaters
		fread (userParams, 4, numParam, userp);
		fread (movieParams, 4, numParam, moviep);
		
	
		float rating = 0;
	
		//take the dot product of the movie and user paramteres
		for (k = 0; k < numParam; k++){
			rating += movieParams[k] * userParams[k];
			if (rating > 5){
				rating = 5;
			}
			if (rating < 1){
				rating =1;
			}
			
		}
	
		//turn the rating into a string
		sprintf(ratingStr, "%f\n", rating);
		//printf("%f\n", rating);
		fputs (ratingStr,ratingf);
		
		if(i%50000 == 0){
			printf("%d\n",i);
		}
		
		
	}

	//close the qual file
	fclose(qual);
	//close the user amd movie parameter files
	fclose(userp);
	fclose(moviep);
	//close the file to write the ratings into
	fclose(ratingf);

	
	return 0;
}
/* 
r	Open the file for reading.
w	Open the file for overwriting. Creates a new file if it doesn't already exist.
a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
b	Opens the file as a binary file (the default without the "b" is a text file).
+	Open the file for reading and writing.
*/

#include <stdio.h> 
#include <string.h>

//REMEBER TO CHANGE THIS IN svd.c!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define numParam 3
#define numUsers 458293
#define numMovies 17770


int main() 
{ 
	//open files in which to write the paramters of the movie and users
	FILE *moviep = fopen("moviepref.dta", "wb");
	FILE *userp = fopen("userpref.dta", "wb");
	
	//fill an array with floats of whatever value of the number of paramters
	float userParams[numParam] = { [0 ... (numParam - 1)] = 1.09 };;
	
	//write tha array for each user
	int i;
	for (i = 0; i < numUsers; i++){
		fwrite (userParams, 4, numParam, userp);
	}
	
	//write that array for each movie
	for (i = 0; i < numMovies ; i++){
		fwrite (userParams, 4, numParam, moviep);
	}
	
	//close the files
	fclose(userp);
	fclose(moviep);
	
	return 0;
}
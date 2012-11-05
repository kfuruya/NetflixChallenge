/**********************************************************************
* CONVERTS all.dta or any other ascii encoded file of that type into
* a binary encoded file
* 
* formatting:
* first 32 bits are the user number
* next 16 bits are movie number
* last 16 bits are rating
* repeat for each data point
* 
* CS 156b
* Irene Yang
*
***********************************************************************/


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

#define numParam 3
#define lrate 0.001

//SAVE USER AS INT - 4 bytes
//SAVE MOVIE AS SHORT - 2 bytes
//SAVE RATING AS SHORT - 2 bytes


int main() 
{ 

	//the number of fata points in all.dta
	int numPointsGoThrough = 102416306;
	//int numPointsGoThrough = 20000;
	
	//in the end this will tell u how many of each type of the point ur selecting out for there are
	long count = 0;
	
	//open the all data pts file (encoded in ASCII)
	FILE *all = fopen("all.dta", "r");
	//create or open the file where the converted data will go
	FILE *conv = fopen("trainingBin.dta", "wb");
	
	//create the string to put the parsed data into
	char str[20];
	//creates an int array to put the str->int data into
	int data[4];
	//creates an array that will be written to the converted data file
	int writeDat[2];
	
	//define indexes cause my compiler sux
	int j;
	long i;
	
	//iterate through each point in the all.dta set
	for(i = 0; i < numPointsGoThrough; i++){
		//iterate through each value (user movie date rating) in each data point 
		for(j = 0; j < 4; j++){
			//read in the line
			fscanf(all, "%s", str);
			//convert each value to int and put it in data[]
			data[j] = atoi(str);
		}
			
			//puts the user value into an int
			writeDat[0] = data[0];
			//puts the last 16 bits of the movie (which should not cut out any relevant data) into the 32 bit int element of writeDat[2]
			writeDat[1] = data[1];
			
			//shift the (less than 16 bit) value of rating into the most sig bits of of data[3]
			data[3] = data[3]<<16;
			//ors it into the top 32 bits of writeDat
			writeDat[1] |= data[3];
			
			//if the rating wasnt 0, then write the data point into the converted file
			if( data[3] != 0 ){
				fwrite(writeDat, 4, 2, conv);
				count++;
			}
			
			//print something out so we know shit is happening
			if(i%100000 == 0){
				printf("%d\n", i);
			}
		}
		
		//prints how many data points we filtered out of all.dta
		printf("count: %d\n", count);
	
	
	//close the files
	fclose(all);
	fclose(conv);
	
	return 0;
}
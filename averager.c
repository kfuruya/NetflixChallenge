#include <stdio.h>
#include <string.h>

#define REGULARIZATION_CONSTANT 25

/* 
 * r	Open the file for reading.
 * w	Open the file for overwriting. Creates a new file if it doesn't already exist.
 * a	Open the file for appending (i.e. new data is added at the end). Creates a new file if it doesn't already exist.
 * b	Opens the file as a binary file (the default without the "b" is a text file).
 * +	Open the file for reading and writing.
 */

void parse_um_line(char line[], int *user_number, int *movie_number, int *date, int *rating)
{
	*user_number = atoi(strtok(line, " "));
	*movie_number = atoi(strtok(NULL, " "));
	*date = atoi(strtok(NULL," "));
	*rating = atoi(strtok(NULL, ""));
}

float get_global_movie_average(const char *infile)
{
    FILE *file = fopen(infile, "r");
    char line[128];
    int user;
    int this_movie;
    double total_rating = 0.0;
    double movie_count = 0.0;
    double movie_average;
    int date;
    int rating;

    while(fgets(line, sizeof(line), file) != NULL)
    {
        parse_um_line(line, &user, &this_movie, &date, &rating);
        total_rating += rating;
        movie_count ++;
    }
    movie_average = total_rating / movie_count;
    fclose(file);
    return movie_average;
}

void get_movie_averages(const char *infile, const char *outfile)
{
    FILE *file = fopen(infile, "r");
    FILE *ofile = fopen(outfile, "w");
    char line[128];
    int user;
    int this_movie;
    int last_movie = 1;
    double last_movie_total= 0.0;
    double last_movie_count = 0.0;
    double last_movie_average;
    int date;
    int rating;
    double K;
    double global_movie_average;

    K = REGULARIZATION_CONSTANT;

    global_movie_average = get_global_movie_average(infile);

    while(fgets(line, sizeof(line), file) != NULL)
    {
        parse_um_line(line, &user, &this_movie, &date, &rating);
        if(this_movie == last_movie)
        {
            last_movie_count ++;
            last_movie_total += rating;
        }
        else
        {
            last_movie_average = (K * global_movie_average + last_movie_total)
                                 / (K + last_movie_count);
            fprintf(ofile, "%f\n", last_movie_average);
            last_movie = this_movie;
            last_movie_count = 1.0;
            last_movie_total = rating;
        }
    }
    fclose(file);
    fclose(ofile);
}

int main(int argc, char **argv)
{
    char *infile;
    char *outfile;
    if(argc != 3)
    {
        printf("Wrong number of inputs! :(\n");
        return -1;
    }
    infile = argv[1];
    outfile = argv[2];
    get_movie_averages(infile, outfile);
    return 0;
}
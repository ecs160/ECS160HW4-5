
/*
Homework 4
Abigail Lee - 913790818 abllee@ucdavis.edu
Luc Nglankong -

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// define :
// max line 1024 characters
// max length = 20,000 lines

const char* getfield(char* row, int num)
{
    const char* tok;
    for (tok = strtok(row, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main() {
// goal - find and print top 10 tweeters ordered in decreasing order

  // accept csv file
  // https://stackoverflow.com/questions/12911299/read-csv-file-in-c
    printf("Start"); 

    FILE* csvFileStream = fopen("./Test CSV/ECS160TestCSV - simplecsv.csv", "r");

    char row[1024];
    while (fgets(row, 1024, csvFileStream))
    {
        char* tmp = strdup(row);
        printf("Field in column 1: %s\n", getfield(tmp, 1));
        free(tmp);
    }
  printf("Hello world!");
  return 0;
    // check if invalid file type
    // check for valid "header" - could be anywhere in file
    // check for valid "name" - could be anywhere in file
    // items may or may not be in quotes

    // Store tweeters/tweets from csv into object - HashMap?


  // loop/stream? through tweets and track number of tweets for every tweeter
  // sort tweeters by count
  // print top 10


}

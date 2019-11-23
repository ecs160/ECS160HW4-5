
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
    for (tok = strtok(row, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int findNameCol(char* row) // send first row
{
    const char* tok;
    int index = 0;
    for (tok = strtok(row, ",");
    		tok && *tok; 
		tok = strtok(NULL, ",\n")) 
    {
	//printf("\nindex %d", index); 
	//index++;
        if (strcmp(tok,"name")){ // add additional checks 
         // return index;
 	}
        
    }


    for (int i = 0; i < strlen(tok); i++) {
    	index++; 
	printf("\nindex %d", index);
	if (strcmp(tok[i], "name") {
		return index;
	}
    }
    return 0; // did not find name column
}

int main() {
// goal - find and print top 10 tweeters ordered in decreasing order

  // accept csv file
  // https://stackoverflow.com/questions/12911299/read-csv-file-in-c
    printf("Start\n");
    const char* filePath = "./Test CSV/csv2.csv";
    FILE* csvFileStream = fopen(filePath, "r");

    // check if invalid file type
    if (!csvFileStream) {
      // could not find or open file
      printf("Invalid Input Format");
      return 0;
    }

    // check if number of rows exceed 20,000
    // check if length of row exceeds 1024 characters
    // check for valid "header" - could be anywhere in file
    // check for valid "name" - could be anywhere in file
    // items may or may not be in quotes
    // else - Invalid Input Format

    // try/catch?


    char row[1024];
    int lineCount = 0;

    printf("Find name column: \n");
    char *tmp;
    tmp = strdup(row);
    int indexName = findNameCol(tmp);
    if (indexName == 0){
      printf("Invalid Input Format\n"); // name column not found
    } else {
      printf("Name column found at: %d\n", indexName);
    }
    printf("end test");


    while (fgets(row, 1024, csvFileStream) && lineCount <= 20000)
    {
        char* tmp = strdup(row);
        printf("Field in column 1: %s\n", getfield(tmp, 1));
        free(tmp);
        lineCount++;
    }
  printf("Hello world!");
  return 0;

    // Store tweeters/tweets from csv into object - HashMap?


  // loop/stream? through tweets and track number of tweets for every tweeter
  // sort tweeters by count
  // print top 10


}

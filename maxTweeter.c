
/*
Homework 4
Abigail Lee - 913790818 abllee@ucdavis.edu
Luc Nglankong -

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_LENGTH 20000

struct tweeter {
  char* name; 
  int tweets;
};

// from stack overflow... need to delete 
char* getfield(char* row, int num) {
    char* tok;
    for (tok = strtok(row, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}


// Need to handle multiple name columns case 
int findNameCol(char* row) {
  char* word = strtok(row, ","); 
  int indexNameCol = -1;
  int tmpIndex = 1; 
  int countNames= 0;

  while(word != NULL) {
    if (strstr(word, "name")) {
      countNames++; 
      indexNameCol = tmpIndex;
    }
    word = strtok(NULL, ",");
    tmpIndex++; 
  }

  if (countNames == 1) {
    return indexNameCol;
  } 
    return -1;

}

// other needed helper functions
// search dynamic struct of tweeters for existing tweeter
// add new tweeter to dynamic struct
// sort dynamic struct of tweeters by num tweets or find top 10 highest tweeter function 


int main() {
// goal - find and print top 10 tweeters ordered in decreasing order

  // is there a way to do this dynamically? waste of space?
  struct tweeter* tweeters = malloc(MAX_LENGTH*sizeof(int)*sizeof(char*));

  // struct tweeter* tweeter_ptr = tweeters; 
  // accept csv file
  // https://stackoverflow.com/questions/12911299/read-csv-file-in-c
    printf("Start\n");
    const char* filePath = "./Test CSV/csv2.csv";
    FILE* csvFileStream = fopen(filePath, "r");

    // check if invalid file type
    if (!csvFileStream) {
      printf("Invalid Input Format");
      return 0;
    }

    // check if number of rows exceed 20,000
    // check if length of row exceeds 1024 characters
    // check for valid "header" - could be anywhere in file
    // check for valid "name" - could be anywhere in file
    // items may or may not be in quotes
    // else - Invalid Input Format


    // FIND NAME COLUMN INDEX 
    char row[1024];
    int lineCount = 0;
    char *tmp;
    tmp = fgets(row, 1024,csvFileStream);
    int indexName = findNameCol(tmp);
   
    if (indexName == -1){
      printf("Invalid Input Format\n"); // name column not found
    } else {
      printf("Name column found at: %d\n", indexName);
    }

    
    //tweeter *newTweeter = malloc(sizeof(tweeter));
   
    // NOT WORKING-- 
    int numTweetersTotal = 0;
    char * str;
    // PRINT NAMES IN COLUMN
    while (fgets(row, MAX_LINE, csvFileStream) && lineCount <= MAX_LENGTH)
    {
        char* tmp = strdup(row);
        str = getfield(tmp, indexName);
        printf("Field in name column: %s\n", str );
       // need to add a search function for finding existing tweeters 
        tweeters[numTweetersTotal].name = str;
        tweeters[numTweetersTotal].tweets++; 

        free(tmp);
        lineCount++;
        numTweetersTotal++; 
    }
    printf("Tweeters Stored:");

    for (int i = 0; i < numTweetersTotal; i++) {
      printf("Tweeter: %s", tweeters[i].name);
    }

    // -- 
    
  printf("\nDone");
  return 0;

    // Store tweeters/tweets from csv into object - HashMap?


  // loop/stream? through tweets and track number of tweets for every tweeter
  // sort tweeters by count
  // print top 10


}

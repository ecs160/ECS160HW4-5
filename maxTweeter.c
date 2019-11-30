
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
#define MAX 10

struct tweeter {
  char* name; 
  int tweets;
};

// function prototypes 
char* getfield(char*, int); 
int findNameCol(char*);
struct tweeter *getTweeter(struct tweeter*, char*, int);
void printTweeters(struct tweeter*, int);
struct tweeter * findTopTenTweeters(struct tweeter*, int);


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
    const char* filePath = "./Test CSV/csv3.csv";
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

        //search for existing tweeter
        struct tweeter * currentTweeter = getTweeter(tweeters, str, numTweetersTotal);

        //increment tweet if tweeter exists, otherwise create new tweeter
        if(currentTweeter != NULL){
          currentTweeter->tweets++;
        }else{
          tweeters[numTweetersTotal].name = str;
          tweeters[numTweetersTotal].tweets++; 
          numTweetersTotal++; 
        }

        free(tmp);
        lineCount++;
    }
    printf("Tweeters Stored:\n");
    printTweeters(tweeters,numTweetersTotal);

    // FIND TOP 10 TWEETERS 
    // finds tweeters with most tweets first
    struct tweeter *topFound = findTopTenTweeters(tweeters, 20);

    // Print new structure made just for top tweeters
    printf("\nTop tweeters: ");
    printTweeters(topFound, MAX);

    // for (int i = 0; i < numTweetersTotal; i++) {
    //   printf("\nTweeter: %s Tweeets %d", tweeters[i].name, tweeters[i].tweets);
    // }

    
  printf("\nDone\n");
  return 0;

    // Store tweeters/tweets from csv into object - HashMap?


  // loop/stream? through tweets and track number of tweets for every tweeter
  // sort tweeters by count
  // print top 10


}


// returns a cell from a row
char* getfield(char* row, int requiredCellIndex){

  //go to index of cell in row
  int currCellIndex = 1;
  int currCharIndex = 0;
  for(; currCellIndex != requiredCellIndex;){
    if(row[currCharIndex] == ','){
      currCellIndex++;
    }
    currCharIndex++;
  }

  //get size of cell
  size_t cellSize = 0;
  for(int i=currCharIndex; (row[i] != '\0') && (row[i] != '\n') 
      && (row[i] != ',') && (i<strlen(row)); i++){
    cellSize++;
  }

  //get substring cell from row
  char* substr = malloc(cellSize+1);
  strncpy(substr, &row[currCharIndex], cellSize);
  substr[cellSize] = '\0';

  return substr;
}


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


struct tweeter * getTweeter(struct tweeter* tweeters, char* name, int numTweeters){

  printf("Entered function!\n");
  for(int i=0; i<numTweeters; i++){
    printf("COMPARING %s with %s\n", tweeters[i].name, name);
    if (strcmp(tweeters[i].name, name) == 0){
      return &tweeters[i];
    }
  }

  return NULL;
}


// Finds Top three tweeters by iteration 
struct tweeter * findTopTenTweeters( struct tweeter* tweeters, int numTweeters){
  int foundTen = 0;
  char * maxTweeterName = ""; 
  int maxTweeterCount = 0; 
  int maxTweeterIndex = 0; 

  struct tweeter * topTweeters = malloc(MAX*sizeof(char*)*sizeof(int));
  
  while (foundTen < MAX) {

   // loop through tweeters
    for (int k = 0; k < numTweeters; k++){ 

      // if num tweets is greater than current max
      // and tweeter hasn't already been added
      if (tweeters[k].tweets >= maxTweeterCount) {
        maxTweeterName = tweeters[k].name;
        maxTweeterCount = tweeters[k].tweets;
        maxTweeterIndex = k;
      }

    }
        // max tweeter won't be added again to topTweeter structure since num tweets is never -1 during initialization
        tweeters[maxTweeterIndex].tweets = -1;
        
        topTweeters[foundTen].name = maxTweeterName;
        topTweeters[foundTen].tweets = maxTweeterCount;
        printf("\n added top tweeter %d", maxTweeterCount);
        foundTen++;
        maxTweeterName = "";
        maxTweeterCount = 0;

  }

  return topTweeters;


}


void printTweeters(struct tweeter* tweeters, int numTweeters) {
  // PRINT TWEETERS STORED 
  for (int j = 0; j < numTweeters; j++) {
    printf("\n tweeter  %s has %d  tweets.", tweeters[j].name, tweeters[j].tweets);
  }
}

/*
Homework 4
Abigail Lee - 913790818 abllee@ucdavis.edu
Luc Nglankong -

*/


// TODO  double check following:

    // check if number of rows exceed 20,000
    // check if length of row exceeds 1024 characters
    // check for valid "header" - could be anywhere in file
    // check for valid "name" - could be anywhere in file
    // items may or may not be in quotes
    // else - Invalid Input Format


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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
int validateName(int indexName, char* row);

int main() {

  struct tweeter* tweeters = malloc(MAX_LENGTH*sizeof(int)*sizeof(char*));

  // struct tweeter* tweeter_ptr = tweeters; 
  // accept csv file
  // https://stackoverflow.com/questions/12911299/read-csv-file-in-c
    const char* filePath = "./Test CSV/csv3.csv";
    FILE* csvFileStream = fopen(filePath, "r");

    // check if invalid file type
    if (!csvFileStream) {
      printf("Invalid Input Format\n");
      return 0;
    }

    // FIND NAME COLUMN INDEX 
    char row[1024];
    int lineCount = 0;
    char *tmp;
    tmp = fgets(row, 1024,csvFileStream);
    char* validationRow = strdup(row); //save copy of row for validation
    int indexName = findNameCol(tmp);
    int validateNameResult = validateName(indexName-1, validationRow); //validate name header
    if(validateNameResult < 1){
      printf("Invalid header format\n"); // name header is invalid
      return -1;
    }
   
    if (indexName == -1){
      printf("Invalid Input Format\n"); // name column not found
    } 

    
    //tweeter *newTweeter = malloc(sizeof(tweeter));
   
    int numTweetersTotal = 0;
    char * str;
    // PRINT NAMES IN COLUMN
    while (fgets(row, MAX_LINE, csvFileStream) && lineCount <= MAX_LENGTH)
    {
        char* tmp = strdup(row);
        str = getfield(tmp, indexName);

        //search for existing tweeter
        struct tweeter * currentTweeter = getTweeter(tweeters, str, numTweetersTotal);

        //increment tweet if tweeter exists, otherwise create new tweeter
        if(currentTweeter != NULL){
          currentTweeter->tweets++;
        }else{
          tweeters[numTweetersTotal].name = str;
          tweeters[numTweetersTotal].tweets = 1; 
          numTweetersTotal++; 
        }

        free(tmp);
        lineCount++;
    }

    // FIND TOP 10 TWEETERS 
    // finds tweeters with most tweets first
    struct tweeter *topFound = findTopTenTweeters(tweeters, 20);

    // Print new structure made just for top tweeters
    printTweeters(topFound, MAX);
    
  return 0;


}

int validateName(int indexName, char* row){
  int cellIndex = 0;
  int nameCharIndex = 0;

  for(int i=0; i<strlen(row) && nameCharIndex < 1; i++){
    // increment cell index when a comma is encountered
    if(row[i] == ','){
      cellIndex++;
    }

    // when the current cell matches the 
    // given "name" header cell, save the char index
    if(cellIndex == indexName){
      nameCharIndex = i;
    }
  }

  //validate for whitespace
  for(int i=nameCharIndex+1; (row[i] != '\0') && (row[i] != '\n') && 
    (row[i] != ',') && (row[i] != '\r'); i++){

      // check if there is a whitespace in name header
      if(isspace(row[i])){
        // return invalid
        return 0;
      }
  }
  // return valid
  return 1;
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
      && (row[i] != ',') && (row[i] != '\r') && (i<strlen(row)); i++){
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

  for(int i=0; i<numTweeters; i++){
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
        foundTen++;
        maxTweeterName = "";
        maxTweeterCount = 0;

  }

  return topTweeters;


}


void printTweeters(struct tweeter* tweeters, int numTweeters) {
  for (int j = 0; j < numTweeters; j++) {
    printf("%s: %d\n", tweeters[j].name, tweeters[j].tweets);
  }
}
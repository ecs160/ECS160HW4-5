
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
    // Done - items may or may not be in quotes
    // else - Invalid Input Format


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


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
int validateHeader(int indexName, char* row, bool isName);
bool quoteScanner(char* phrase);
int getNumColumns(char* row);
bool getQuoteIndex(char* header, int columnIndex);
bool checkColumnQuotes(char* row, bool quoteIndex[], int numColumns);

int main() {

  struct tweeter* tweeters = malloc(MAX_LENGTH*sizeof(int)*sizeof(char*));

  // struct tweeter* tweeter_ptr = tweeters; 
  // accept csv file
  // https://stackoverflow.com/questions/12911299/read-csv-file-in-c
    const char* filePath = "./Test CSV/csv2.csv";
    FILE* csvFileStream = fopen(filePath, "r");

    // check if invalid file type
    if (!csvFileStream) {
      printf("Invalid Input Format\n");
      return -1;
    }

    // FIND NAME COLUMN INDEX 
    char row[1024];
    int lineCount = 0;
    char *tmp;
    tmp = fgets(row, 1024,csvFileStream);

    char* validationRow = strdup(row); //save copy of row for validation
    char* numColumnsRow = strdup(row); //save copy of row for finding # of columns
    char* quoteIndexRow = strdup(row); //save copy of row for getting column quote index

    int indexName = findNameCol(tmp);
    int numColumns = getNumColumns(validationRow); //get the number of columns

    int validateNameResult = validateHeader(indexName, validationRow, true); //validate name header
    
    // get which columns should be wrapped in quotes
    // quoteIndex[i] == true - should be wrapped in quotes
    // quoteIndex[i] == false - should not be wrapped in quotes
    bool quoteIndex[numColumns];
    for(int i=1; i<=numColumns; i++){
      quoteIndex[i] = getQuoteIndex(quoteIndexRow, i);
    }

    free(validationRow);
    free(numColumnsRow);
    free(quoteIndexRow);
    if(validateNameResult < 1){
      return -1;
    }
   
    if (indexName == -1){
      printf("Invalid Input Format\n"); // name column not found
      return -1;
    } 
    
    //tweeter *newTweeter = malloc(sizeof(tweeter));
   
    int numTweetersTotal = 0;
    char * str;
    // PRINT NAMES IN COLUMN
    while (fgets(row, MAX_LINE, csvFileStream) && lineCount <= MAX_LENGTH)
    {
        char* tmp = strdup(row);
        str = getfield(tmp, indexName);

        //validate that current row has the correct # of columns
        if(getNumColumns(tmp) != numColumns){
          printf("Invalid csv: a row contains an incorrect # of columns.\n");
          return -1;
        }

        //validate that the columns that should be wrapped in quotes are
        //correctly wrapped in quotes
        if(!checkColumnQuotes(tmp, quoteIndex, numColumns)){
          return -1;
        }

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

bool checkColumnQuotes(char* row, bool *quoteIndex, int numColumns){
  
  //iterate through each cell in row
  for(int i=1; i<=numColumns; i++){

    //goto current cell
    int currCellIndex = 1;
    int currCharIndex = 0;
    for(; currCellIndex != i;){
      if(row[currCharIndex] == ','){
        currCellIndex++;
      }
      currCharIndex++;
    }

    //get cell size
    size_t cellSize = 0;
    for(int j=currCharIndex; (row[j] != '\0') && (row[j] != '\n') 
        && (row[j] != ',') && (row[j] != '\r') && (j<strlen(row)); j++){
      cellSize++;
    }

    //get substring cell from row
    char* substr = malloc(cellSize+1);
    strncpy(substr, &row[currCharIndex], cellSize);
    substr[cellSize] = '\0';

    //check for consistent quotes on cell entry
    if(!quoteScanner(substr)){
      printf("Invalid csv: a row does not contain consistent quotes.\n");
      return false;
    }

    //if the cell's header has quotes but the cell doesn't
    if((substr[0] == '"' && !quoteIndex[i])){
      printf("Invalid csv: a column that must not contain quotes contains an entry with quotes.\n");
      return false;
    }

    //if the cell's header doesn't have quotes but the cell does, return invalid
    if(substr[0] != '"' && quoteIndex[i]){
      printf("Invalid csv: a column that must contain quotes contains an entry without quotes.\n");
      return false;
    }
  }

  return true;
}


// return whether or not a column should be wrapped in quotes
bool getQuoteIndex(char* header, int columnIndex){
  
  // navgiate to column index
  int currCellIndex = 1;
  int currCharIndex = 0;
  for(; currCellIndex != columnIndex;){
    if(header[currCharIndex] == ','){
      currCellIndex++;
    }
    currCharIndex++;
  }

  // if the first character of the column is a quote, then 
  // we know that this entire column should be wrapped in quotes.
  // We don't need to check if the last character is a quote because
  // we already validated that the header is properly wrapped in 
  // quoteScanner()
  // if a "," is found, then the column header is empty and entries shouldn't
  // be wrapped in quotes.
  if(header[currCharIndex] == '"' && header[currCharIndex] != ','){
    return true;
  }

  // if the first character was not a quote or the column is
  // empty, then this column should not be wrapped in quotes
  return false;
}

// given the csv header, find the number of columns
int getNumColumns(char* header){
  int numColumns=0;
  for(int i=0; i<strlen(header); i++){
    if(header[i] == ','){
      numColumns++;
    }
  }
  numColumns++;

  return numColumns;
}

bool quoteScanner(char*phrase) {
  bool startQuote = false;
  bool endQuote = false;

  for (int i = 0; i < strlen(phrase); i++ ){
    if (phrase[i] == '"') {
        //  printf("\nfound start quote\n");
        startQuote = true;
    }
    i++;
    if ((startQuote) && (phrase[i] =='"')){
        // printf("\nfound end quote\n");
      endQuote = true;
    }
  }
  if((startQuote && endQuote) || (!startQuote && !endQuote)){
     return true;
  }

  return false;
}

// validate header
int validateHeader(int indexName, char* row, bool isName){

  //go to index of cell in row
  int currCellIndex = 1;
  int currCharIndex = 0;
  for(; currCellIndex != indexName;){
    if(row[currCharIndex] == ','){
      currCellIndex++;
    }
    currCharIndex++;
  }

  //get size of cell
  size_t cellSize = 0;
  for(int i=currCharIndex; (row[i] != '\0') && (row[i] != '\n') 
      && (row[i] != ',') && (row[i] != '\r') && (i<strlen(row)); i++){

    // invalid if there is a space
    if(isspace(row[i]) && isName){
      printf("Invalid header: name column must not contain whitespaces.\n");
      return -1;
    }
    cellSize++;
  }

  //get substring cell from row
  char* substr = malloc(cellSize+1);
  strncpy(substr, &row[currCharIndex], cellSize);
  substr[cellSize] = '\0';

  if(!quoteScanner(substr)){
    printf("Invalid header: inconsistent quotes.\n");
    return -1;
  }
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
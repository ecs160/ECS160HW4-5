
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

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
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

  #include <stdio.h>
#include <stdlib.h>
#include <string.h>



    FILE* stream = fopen("ECS160TestCSV - simplecsv.csv", "r");

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        printf("Field 3 would be %s\n", getfield(tmp, 3));
        // NOTE strtok clobbers tmp
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

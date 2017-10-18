#include "stackHw.h"

//initializing globals 
int *numbers = NULL;
int numSize = 0;


void GetNumbers(char fileName[])
{
  FILE* fp = fopen(fileName, "rb");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  const char delim[2] = ",";
  int lineCount = 0;
  char * token;
  int numbersCount = 0;
  char lineStr[10000];
  if(fp == NULL)
  {
    printf("\nError reading file");
    exit(-1);
  }
  while ((read = getline(&line, &len, fp)) != -1) 
  {
    sprintf(lineStr, "%s", line);
    if(lineCount == 0)
    {
      numSize = atoi(line);
      numbers = malloc(numSize * sizeof(int));
    }
    else if(lineCount>1)
    {
      break;
    }
    else
    {
    
      token = strtok(lineStr, delim);
  
      while( token != NULL ) 
      {
        numbers[numbersCount] = atoi(token);
        //printf( " %s\n", token );
        numbersCount++;
        token = strtok(NULL, delim);
      }
      //printf("Retrieved line of length %zu :\n", read);
      //printf("%s\n", line);
    }
    lineCount++;
  }

  fclose(fp);
  if (line!= NULL)
  {
    free(line);
  }

}
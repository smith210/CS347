#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "arg_parse.h"
/*  argCount
*   counts number of arguements
*
*/

static unsigned int argCount(char* line){
  unsigned int counter = 0;
  while (*line != '\0'){
    if(*line == ' ' || *line == '\t'){
      line++;
    }else{
      counter++;
      while(*line != ' ' &&  *line != '\0' && *line != '\t'){
        line++;
      }
    }
  }
  return counter;
}
/* Arg Parse
* returns a set of pointers that points at the beginning of each
* arguement, setting the end of each arguement with a null space.
*/
char** arg_parse(char* line, int* argcp){
  *argcp = argCount(line);
  char** args = malloc ((argCount(line) + 1) * sizeof(char*));
  unsigned int curr = 0;
  while(*line != '\0' && argCount(line) > 0){
    while(*line == ' '){
      line++;
    }
    args[curr] = line;
    curr++;
    while(*line != ' ' && *line != '\0' && *line != '\t'){
      line++;
    }
    if(*line == ' ' || *line == '\t'){
      *line = '\0';
      line++;
    }
  }

  return args;
}

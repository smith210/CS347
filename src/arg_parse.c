#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "arg_parse.h"
#include <ctype.h>
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
  unsigned int num_args = argCount(line);
  *argcp = num_args;

  char** args = malloc ((num_args + 1) * sizeof(char*));
  unsigned int curr = 0;
  int begin_arg = 1;//1 = false, 0 = true

  while(*line != '\0' && num_args > 0){
    if (isspace((int)(*line))){
      if (begin_arg == 0){
        begin_arg = 1;
        *line = '\0';
      }
    }else{
      if (begin_arg == 1){
        begin_arg = 0;
        args[curr] = line;
        curr++;
      }
    }
      line++;
  }

  return args;
}

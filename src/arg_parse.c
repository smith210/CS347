#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "arg_parse.h"
#include <ctype.h>
/*  argCount
*   counts number of arguements
*
*/

static unsigned int argCount(char* line){
  unsigned int counter = 0;
  int seen_char = 0;
  while (*line != '\0'){
    if(isspace((int)*line)){
        if(seen_char == 1)
          seen_char = 0;
    }else{
      if (seen_char == 0){
        counter++;
        seen_char = 1;
      }
    }
    line++;
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
  char** args = (char**)malloc(num_args * sizeof(char*)+1);
  char* new_line = strdup(line);
  unsigned int curr = 0;
  int begin_arg = 1;//1 = false, 0 = true
  if(num_args <= 0){
	   return NULL;
  }else{
   while(*new_line != '\0'){

    if (isspace((int)(*new_line))){
      if (begin_arg == 0){
        begin_arg = 1;
      }
      *new_line = '\0';
    }else{
      if(*new_line == '='){
        *new_line = '\0';
      }
      if (begin_arg == 1){
        begin_arg = 0;
        args[curr]=new_line;
        curr++;
      }
    }
      new_line++;
  }
  }
  //free(new_line);
  return args;
}

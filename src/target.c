#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "arg_parse.h"
#include "target.h"

/*
*is_colon
* 1 = colon in line
* 0 = no colon in line
*/
static unsigned int count(char* line){
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


unsigned int is_colon(char* line){
  unsigned int exist_colon = -1; //-1 = false, 0 = true;
  unsigned int exist_other = -1; //-1 = false, 0 = true;

  while (*line != '\0'){
    if(!(isspace((int)*line))){
      exist_other = 0;
    }
    if(*line == ':'){
      if(exist_other != -1){
        exist_colon = 0;
        break;
      }
    }
    line++;
  }
  return exist_colon;
}

/*
*find_colon
* 1 = no colon exists as arguement
* 0 = colon exists as arguement
*/
unsigned int find_colon(char** line){
  unsigned int exist_colon = 1;
  unsigned int curr = 0;
  while (line[curr] != '\0'){
    if(is_colon(&*line[curr]) == 0){

      exist_colon = 0;
      break;
    }
    curr++;
    *line++;
  }

  return exist_colon;
}

/*
*add_rules - appends rules to pre-existing rules
*/

char*** target_parse(char* line, int* argcp, int* argtp, int* argdp){
  unsigned int num_args = count(line);

  char*** all_args = malloc(2*sizeof(char**));
  char** args = malloc ((num_args + 1) * sizeof(char*));
  unsigned int curr = 0;
  int begin_arg = 1;//1 = false, 0 = true

  while(*line != ':'){
    //printf("adding targets...\n");
    if (isspace((int)(*line))){
      if (begin_arg == 0){
        begin_arg = 1;
        *line = '\0';
      }
    }else{

      if (begin_arg == 1){
        //printf("%s\n", line);
        begin_arg = 0;
        args[curr] = line;
        curr++;
      }
    }
      line++;
  }
  *line = '\0';
  line++;
  while(isspace((int) *line)){
    line++;
  }
  //printf("%s\n", line);
  //*line = '\0';
  all_args[0] = args;
  //printf("arg0\n");
  int dep_num;
  all_args[1] = arg_parse(line, &dep_num);
  //printf("arg1\n");
  if(num_args - curr - dep_num != 0){
    num_args--;
  }
  *argcp = num_args;
  *argtp = dep_num;
  *argdp = curr;
  //printf("%d\n", *argcp);
  return all_args;
}


/*
*add_rules - appends rules to pre-existing rules
*/
char*** add_rules(char** new_rule, char*** guideline, int num_arg){
  guideline[num_arg] = new_rule;
  num_arg++;
  return guideline;
}
/*

int is_tgt_alphabet(char* line){
  if (*line == '\0'){
    return -1;
  }
  if (*line == '\t'){
    return -1;
  }
  if (isspace((int)*line)){
    return 0;
  }
  if (*line == ':'){
    return 1;
  }else{
    return 2;
  }
}

*state cases

int is_tgt_q0(char* line){
  switch(is_tgt_alphabet(line)) {
    case -1: {
      return 0;
    }
    case 0: {
      return is_tgt_q0(line++);
    }
    case 1: {
      return is_tgt_q1(line);
    }
    case 2: {
      return is_tgt_q1(line);
    }
  }
}

int is_tgt_q1(char* line){
  switch(is_tgt_alphabet(line)) {
    case -1: {
      return 0;
    }
    case 0: {
      return is_tgt_q0(line++);
    }
    case 1: {
      return 1;
    }
    case 2: {
      return is_tgt_q2(line);
    }
  }
}

int is_tgt_q2(char* line){
  switch(is_tgt_alphabet(line)) {
    case -1: {
      return 0;
    }
    case 0: {
      return is_tgt_q2(line++);
    }
    case 1: {
      return 1;
    }
    case 2: {
      return is_tgt_q2(line);
    }
  }
}
*/

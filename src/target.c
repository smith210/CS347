#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "arg_parse.h"
#include "target.h"
/*
*Note: I tried parsing using a state machine but I didn't really understand how
* to implement it, and thus therefore I couldn't get it working with my
* original code, so I left it out and stayed with my original code.
*Also I have several misunderstandings about linked lists that I didn't
*realize I had (especially when it came to pointers and setting them up) until
*too late, so the code will not exactly work up to par (for now).
*
*/



/*
*count
* needed it in target since the function in arg_parse was static.
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

/*
*is_colon
* 1 = colon in line
* 0 = no colon in line
*/
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
    //*line++;
  }

  return exist_colon;
}

/*
*target_parse- creates list of rules
*/

char*** target_parse(char* line){
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

  //printf("%d\n", *argcp);
  return all_args;
}

/*compares strings, returns 0 if true, else returns -1
*
*/
int comparison(char* arg, target* listing){
  //printf("int comparison\n");
  int truth_teller = -1;
  while(listing != NULL){
    //printf("listing -> next does a thing\n");
    char** curr_target = listing->one_tgt;
    if (curr_target != NULL){
    //  printf("Yeah\n");
      int check = strcmp(arg, *curr_target);
      //printf("%d\n", check);
      if (check == 0){
        truth_teller = 0;
        break;
      }
      curr_target++;
    }
    listing = listing->next;
  }
  return truth_teller;
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

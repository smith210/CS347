#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "target.h"
#include "arg_parse.h"
#include <ctype.h>
#include <string.h>

target collect_tgts(){
  FILE* makefile = fopen("./uMakefile", "r");
  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);
  target curr;
  int curr_arg;

  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(is_colon(&line) == 0){
      curr->one_target = line;
      int col_place = find_colon(line);
      curr->two_dependent = arg_parse(line[col_place], &curr_arg);
    }else{
//      curr->three_rule = ;
printf("DERP");
    }


    linelen = getline(&line, &bufsize, makefile);
  }

  free(line);
  return curr;
}



int target_search(target* t, char* name){
  return strcmp(t->one_target, name);
}

static unsigned int find_colon(char* line){
  unsigned int exist_colon = 0;
  while (*line != '\0'){
    if(*line == ':'){
      break;
    }
    exist_colon++;
    line++;
  }
  return exist_colon;
}

static unsigned int is_colon(char* line){
  unsigned int exist_colon = 1; //1 = false, 0 = true;
  while (*line != '\0'){
    if(*line == ':'){
      exist_colon = 0;
    }
    line++;
  }
  return exist_colon;
}

char*** add_rules(char** new_rule, char*** guideline, int num_arg){
  guideline[num_arg] = new_rule;
  return guideline;
}

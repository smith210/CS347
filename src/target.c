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

  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(line[0] == '\t')
      processline(&line[1]);


    linelen = getline(&line, &bufsize, makefile);
  }

  free(line);
  return curr;
}



int target_search(target* t, char* name){
  return strcmp(t->one_target, name);
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

char* add_target(char* name){
  return name;
}
char** add_dependents(char* new_dependent, char** dependent_list, int num_arg){
  dependent_list[num_arg] = new_dependent;
  return dependent_list;
}
char*** add_rules(char*** rule_list, char** new_rule, int num_arg){
  rule_list[num_arg] = new_rule;
  return rule_list;
}

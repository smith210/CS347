#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "arg_parse.h"
#include "target.h"

char** retrieve_dpt(char* line, int* sz){
    while(*line != ':'){
      line++;
    }

    *line = '\0';
    line++;

    char** faux_dependents = arg_parse(line, sz);
    int sz_a =*sz;
    if(sz_a == 0){
      return NULL;
    }
    char** dependents = malloc(sz_a * sizeof(char*));
    int curr = 0;
    //char** act_dpt = malloc ((sz) * sizeof(char*));
    while(curr != sz_a){
      dependents[curr] = strdup(*faux_dependents);
      //printf("string: %s\n", dependents[curr]);
      curr++;
      faux_dependents++;

    }
    dependents[curr] = NULL;
    faux_dependents = dependents;
    //printf("%s is my first dependent\n", *faux_dependents);
    //printf("did escape?\n");
    return faux_dependents;

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
        //*line = '\0';
        exist_colon = 0;
        break;
      }
    }
    line++;
  }

  return exist_colon;
}

void printdpt(char** lines, int sz){
  int curr = 0;
  while (curr != sz){
    printf("curr->%d\n", curr);
    printf("dpt: %s\n", *lines);
    lines++;
    curr++;
  }
}

void add_target_dsc(char* line, targets** target_rule, char*** dpts, int dpt_sz){
  //printf("target -> %s\n", line);
  //printf("num dependent -> %d\n", dpt_sz);
//printdpt(*dpts, dpt_sz);
  targets* new_target = (targets*) malloc(sizeof(targets));
  targets* end_target_list = *target_rule;

  new_target->one_tgt = strdup(line);
  new_target->two_dpndt = *dpts;
  new_target->next = NULL;



  if((*target_rule)->one_tgt == NULL){
    *target_rule = new_target;
    return;
  }

  while(end_target_list->next != NULL){
      end_target_list = end_target_list->next;
  }

  end_target_list->next = new_target;


  return;

}


void add_rules(char* line,  int type_target, targets** holder){
  rules* new_rule = (rules*) malloc(sizeof(rules));

  targets* end_target_list = *holder;

  //printf("Position of target Num: %d\n", type_target);
  //printf("Rule input: %s\n", line);
  int i = 1;

  while(i != type_target){
    //printf("CURR TARGET: %s\n", end_target_list->one_tgt);
      end_target_list = end_target_list->next;
      //printf("next\n");
      i++;
  }
  //printf("target: %s\n", end_target_list->one_tgt);
  //printf("rule being added: %s\n", line);
  new_rule->rule_detail = strdup(line);
  new_rule->rule_next = NULL;

  rules* list_rule = end_target_list->rules;

  if(list_rule == NULL){
    list_rule = new_rule;
    end_target_list->rules = list_rule;
    return;
  }
  rules* end_rule_list = end_target_list->rules;
  while(end_rule_list->rule_next != NULL){
      end_rule_list = end_rule_list->rule_next;
  }
  end_rule_list->rule_next = new_rule;
  return;

}

int is_alphabet(char* line){
  if (is_colon(line) == -1 && *line != '\t'){
    if(has_equal(line) == 0){
      return 2;
    }else{
    return -1;
    }
  }
  if (*line == '\t'){
    return 0;
  }
  return 1;
}

int has_equal(char* line){
  int is_equal = 1;
  while(*line != '\0'){
    if(*line == '='){
      is_equal = 0;
    }
    line++;
  }
  return is_equal;
}

int is_tgt_alphabet(char* line){
  if (*line == '\0'){
    return -1;
  }
  if (isspace((int) *line)){
    return 0;
  }

  return 1;

}

void add_tgt_dpt0(char* line, targets** tracker, char*** dpts, int dpt_sz){
  while(isspace((int) *line)){

    line++;
  }
  //printdpt(*dpts, dpt_sz);
  add_target_dsc(line, tracker, dpts, dpt_sz);
  return;

}
/*
void add_tgt_dpt1(char* line, struct target_st** tracker){
  switch(is_tgt_alphabet(line)) {
    case -1: {
      return;
    }
    case 0: {
      *line = '\0';
      //move to adding dependencies
      return;
    }
    case 1: {
      return add_tgt_dpt1(line++, tracker);
    }
  }
}

void add_tgt_dpt2(char* line, struct target_st** tracker){
  switch(is_tgt_alphabet(line)) {
    case -1: {
      return;
    }
    case 0: {
      *line = '\0';
      //line++;
      //real_add_dpt();
      return;
    }
    case 1: {
      return add_tgt_dpt2(line, tracker);
    }
  }
}
*/

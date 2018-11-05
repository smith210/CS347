#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "arg_parse.h"
#include "target.h"

unsigned int is_colon(char* line){
  unsigned int exist_colon = -1; //-1 = false, 0 = true;
  unsigned int exist_other = -1; //-1 = false, 0 = true;

  while (*line != '\0'){
    if(!(isspace((int)*line))){
      exist_other = 0;
    }
    if(*line == ':'){
      if(exist_other != -1){
        *line = '\0';
        exist_colon = 0;
        break;
      }
    }
    line++;
  }

  return exist_colon;
}


void add_target_dsc(char* line, targets** target_rule){

  targets* new_target = (targets*) malloc(sizeof(targets));
  targets* end_target_list = *target_rule;

  new_target->one_tgt = strdup(line);
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
    return -1;
  }
  if (*line == '\t'){
    return 0;
  }
  return 1;
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

void add_tgt_dpt0(char* line, targets** tracker){
  while(isspace((int) *line)){

    line++;
  }
  add_target_dsc(line, tracker);
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

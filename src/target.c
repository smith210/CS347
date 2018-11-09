#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "arg_parse.h"
#include "target.h"
/* retrieve_dpt
*
* interprets the target line, takes all the dependents, assigns and returns
* them as pointers.
*/
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
/* is_colon
*
* returns -1 if false
* returns 0 if true
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
        //*line = '\0';
        exist_colon = 0;
        break;
      }
    }
    line++;
  }

  return exist_colon;
}

/* add_target_dsc
*
* creates a new target containing specified target and dependencies, which
* will then be placed in the overall target structure.
*
*/
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

/* add_target_dsc
*
* creates a new rule containing passed rule, which
* will then be placed in the overall target structure after (possibly)
* looping through the target structure.
*
*/
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
/* is_alphabet
*
* function used for alphabet to determine how the line from the file
* is read and parsed in the umake files accordingly.
*
* -1 : Empty line
* 1  : Everything Else
* 0  : Rule
* 2  : Enviornment - needs to be set
*/
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

/* has_equal
*
* returns 0 if '=' is in the file line
* returns 1 if '=' is not in the file line
*/
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
   int valid_env(char* line);


/* enviornment expansions
*
*/

/*expand - expands enviornmental variables.
 *env_holder - buffer that can contain the enviornment desired
 *
 *validity - flag, marks when the char is '$'
 *is_valid - flag, determines whether the line has valid syntax (no missing {})
 *is_cbrace - flag, checks if '{' appears after '$'
 *bracket_valid - flag, checks for valid brackets
 *env_valid - flag, checks for spaces within brackets
 *
 *val - value of where to place values in temporary buffer env_holder
 *buff_val - value of where to place values in buffer new
 *
 *if flags = 1, then false
 *if flags = 0, then true
*/

int expand(char* orig, char* new, int newsize){
     //printf("expand\n");
      char env_holder[newsize];
      int validity = 1;
      int is_cbrace = 1;
      int bracket_valid = bracket_check(orig);
      int env_valid = valid_env(orig);
      int val = 0;
      int buff_val = 0;

      if(bracket_valid == 0){
        if(env_valid == 0){
          return 0;
        }
        while (*orig != '\0'){

          if (validity == 0){
            //printf("validity true\n");
            if(*orig == '{'){
              //printf("is_cbrace\n");
              is_cbrace = 0;
            }

            if(is_cbrace == 0 && *orig != '{' && *orig != '}'){
              env_holder[val] = *orig;
              val++;
            }

            if(is_cbrace == 1){
              new[buff_val] = '$';
              buff_val++;
              new[buff_val] = *orig;
              buff_val++;
              validity = 1;
            }
          }

          if(*orig == '$'){
            validity = 0;
            val = 0;
            is_cbrace = 1;
            //nxt_rc = 0;
            memset(env_holder, 0, newsize);
          }

          if(*orig == '}'){
            validity = 1;
            char* b = getenv(env_holder);

            if (b != NULL){
              while(*b != '\0'){
                new[buff_val] = *b;
                buff_val++;
                b++;
              }

            }
          }
          if(validity == 1 && *orig != '}' && *orig != '\\'){
            new[buff_val] = *orig;
            buff_val++;
          }
          orig++;
      }
      }else{
        return 0;
      }

      new[buff_val] = '\0';
      return 1;
  }

/*bracket_check - helper flag function for expand
*returns 1 if valid
*returns 0 if invalid
* checks for number of left/right braces, and if a right brace appears
* before a left.
*/
int bracket_check(char* line){
 int right_brace = 0;
 int left_brace = 0;

 while(*line != '\0'){
   if(right_brace > left_brace){
     break;
   }
   if(*line == '}'){
     right_brace++;
   }
   if(*line == '{'){
     left_brace++;
   }
   line++;
 }

 if (left_brace == right_brace){
   return 0;
 }else{
   return 1;
 }
}

/*valid_env - helper flag function for expand
*returns 1 if valid
*returns 0 if invalid
* checks if the eniovnment is empty
*/
int valid_env(char* line){
 int in_env = 1;

 while(*line != '\0'){
   if(in_env == 0 && (isspace((int)(*line)))){
     return 0;
     break;
   }
   if(*line == '}'){
     in_env = 1;
   }
   if(*line == '{'){
     in_env = 0;
   }
   line++;
 }

   return 1;
}
  /* size_line
   * returns total number of chars in line.
   */
  int size_line (char* line){
    int sz = 0;
    while(*line != '\0'){
      line++;
      sz++;
    }
    return sz;
  }


/* add_tgt_dpt0
*
* parses through spaces before parses the
* file line into the file structure.
*/
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

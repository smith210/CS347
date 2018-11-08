#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include "target.h"
#include "arg_parse.h"


/* CSCI 347 micro-make
 *
 * 05 Oct 2018, Martin Smith
 */


/* CONSTANTS */
//unsigned int argCount(char* line);

/* PROTOTYPES */

/* Process Line
 * line   The command line to execute.
 * This function interprets line as a command line.  It creates a new child
 * process to execute the line and waits for that process to complete.
 */
 void processline(char* line);
 void hlp_process(rules** instructions);
 int expand(char* orig, char* new, int newsize);
 char* dup_env(char* line);
 int expand_0(char* line);
 int expand_alphabet(char* line);
 int bracket_check(char* line);
  void execute_dpt(char** dpt, targets** roster);
   void execute_dpt_hlp(char* dpt, targets** roster);


 /* Arg Parse
 *  line   Interprets the command line by counting the number of arguements,
 *  and creating pointers and null spaces per each arguement if there are
 *  multiple.
 */


 /* Main entry point.
  * argc    A count of command-line arguments
  * argv    The command-line argument valus
  *
  * Micro-make (umake) reads from the uMakefile in the current working
  * directory.  The file is read one line at a time.  Lines with a leading tab
  * character ('\t') are interpreted as a command and passed to processline minus
  * the leading tab.
  */
 int main(int argc, const char* argv[]) {

   FILE* makefile = fopen("./uMakefile", "r");

   size_t  bufsize = 0;
   char*   line    = NULL;
   ssize_t linelen = getline(&line, &bufsize, makefile);

   targets* historian = (targets*) malloc(sizeof(targets));
   historian->one_tgt = NULL;
   historian->rules = NULL;

   int target_acc = 0;

   while(-1 != linelen) {

     if(line[linelen-1]=='\n') {
       linelen -= 1;
       line[linelen] = '\0';
     }
 /*
     if(line[0] == '\t')
       processline(&line[1]);
 */

     //if(historian->rules != NULL){
     //  printf("%s\n", historian->rules->rule_detail);
     //}


     switch(is_alphabet(&line[0])){
 //not a target/dependent or rule
     case -1: {
       break;
     }

     case 0: {
       add_rules(&line[1], target_acc, &historian);
       break;
     }

     case 1: {
       //printf("No?\n");
       int dpt_sz;

       char** dpts = retrieve_dpt(&line[0], &dpt_sz);
       //printdpt(dpts, dpt_sz);
       add_tgt_dpt0(&line[0], &historian, &dpts, dpt_sz);
       target_acc++;
       //printf("we done?\n");
       break;
     }

    case 2:{
      int garbage;
      char** env_contain = arg_parse(&line[0], &garbage);
      char* name = strdup(env_contain[0]);
      //printf("%s\n", name);
      char* value = strdup(env_contain[2]);
      int did_setenv = setenv(name, value, 1);

      if(did_setenv != 0){
        perror("setenv");
        exit(EXIT_FAILURE);
        break;
      }
     }
     }

     linelen = getline(&line, &bufsize, makefile);
   }

   if(argc > 1){

     for (int i = 1; i < argc; i++){
       //printf("WHATS FOR LOOPING in %d?\n", i);

       targets* end_target_list = *(&historian);
       while(end_target_list != NULL){

         if (strcmp(end_target_list->one_tgt, argv[i]) == 0){
           //check for dependencies
           char** dependents = end_target_list->two_dpndt;

           if (dependents != NULL){
             targets* dictionary = *(&historian);
             execute_dpt(end_target_list->two_dpndt, &dictionary);
             //printf("YEAH THERE's dependents whoo\n");
           }
             hlp_process(&(end_target_list->rules));

         }

           end_target_list = end_target_list->next;

       }
     }
 }

   free(line);
   return EXIT_SUCCESS;
 }

 void execute_dpt(char** dpt, targets** roster){

     while(*dpt != NULL){
       execute_dpt_hlp(*dpt, roster);
       dpt++;
     }


 }

 void execute_dpt_hlp(char* dpt, targets** roster){
   targets* end_target_list = *roster;
   while(end_target_list != NULL){

     if (strcmp(end_target_list->one_tgt, dpt) == 0){
       //check for dependencies
       char** dependents = end_target_list->two_dpndt;

       if (dependents != NULL){
         targets* dictionary = *roster;
         execute_dpt(end_target_list->two_dpndt, &dictionary);
         //printf("YEAH THERE's dependents whoo\n");
       }
         hlp_process(&(end_target_list->rules));

     }
       end_target_list = end_target_list->next;

   }


 }

 void hlp_process(rules** instructions){

   rules *end_rule_list = *instructions;
   //printf("In hlp_process\n");
   //printf("hlp_process: %s\n", end_rule_list->rule_detail);
   while (end_rule_list != NULL){
     //printf("I'm not null YEEEEEEET\n");
     processline(end_rule_list->rule_detail);
     end_rule_list = end_rule_list->rule_next;
   }
   //printf("Guess we're out of rules\n\n");

 }

 /* enviornment expansions
 *
 */

 /*expand - expands enviornmental variables.
  *env_holder - buffer that can contain the enviornment desired
  *
  *validity - flag, marks when the char is '$'
  *is_valid - flag, determines whether the line has valid syntax (no missing {})
  *is_cbrace - flag, checks if '{' appears after '$'
  *bracket_valid - flag, checks for matching/mismatching brackets
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
       //int is_valid = expand_0(orig);
       int val = 0;
       int buff_val = 0;
       //int nxt_rc = 1;
       //printf("is_valid: %d\n", is_valid);
       //if(bracket_valid == 1){
         //return 0;
       //}

       if(bracket_valid == 0){
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
             //printf("curr: %s\n", env_holder);
             }
             if(is_cbrace == 1){
               new[buff_val] = '$';
               buff_val++;
               new[buff_val] = *orig;
               buff_val++;
               //printf("not is_cbrace\n");
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
             //printf("%s is the enviornment...\n", b);
             if (b != NULL){
               while(*b != '\0'){
                 new[buff_val] = *b;
                 buff_val++;
                 b++;
               }
               //printf("What to return: %s\n", new);
             }
             //puts(getenv("USER"));
           }
           if(validity == 1 && *orig != '}' && *orig != '\\'){
             new[buff_val] = *orig;
             buff_val++;
             //printf("What to return: %s\n", new);
           }
           //printf("Moving to next value...\n\n");
           orig++;
       }
       }else{
         //printf("Yeah can't do that lol\n");
         return 0;
       }
       //buff_val++;
       new[buff_val] = '\0';
       //printf("What to return: %s\n", new);
       //memset(new, buff_val, newsize);
       return 1;
   }

/*bracket_check - helper flag function for expand
*returns 1 if valid
*returns 0 if invalid
*note - this only checks for same number of brackets,
*       the expand function will do syntax checks.
*/
int bracket_check(char* line){
  int right_brace = 0;
  int left_brace = 0;

  while(*line != '\0'){
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


 /* Process Line
  *
  */
 void processline (char* line) {
   //printf("processline\n");
   //int buf_count;
   //char* wild_west = ;
   //int expansion = expand(line, wild_west, buf_count);

   int is_arg;
   int buf_size = size_line(line) - 1;
   //printf("%d is the size of the array\n", buf_size);
  char new_rule[buf_size];
  int is_expand = expand(line, new_rule ,buf_size);
  //printf("is_expand: %d\n", is_expand);
  if(is_expand == 0){
    //line = new_rule;
    printf("ERROR: expand\n");
    exit(1);
  }
    line = new_rule;
   char** lne = arg_parse(line, &is_arg);


   const pid_t cpid = fork();
   switch(cpid) {

   case -1: {
     perror("fork");
     break;
   }

   case 0: {
     if(is_arg > 0){
    //printf("string: %s\n", lne[0]);
     execvp(lne[0], lne);

     perror("execvp");
     exit(EXIT_FAILURE);
     break;
   }
   }

   default: {
     int   status;
     const pid_t pid = wait(&status);
     if(-1 == pid) {
       perror("wait");
     }
     else if (pid != cpid) {
       fprintf(stderr, "wait: expected process %d, but waited for process %d",
               cpid, pid);
     }
     break;
   }
   }
 }

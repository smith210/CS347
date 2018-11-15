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
  void execute_dpt(char** dpt, targets** roster);
   void execute_dpt_hlp(char* dpt, targets** roster);



 /* Arg Parse
 *  line   Interprets the command line by counting the number of arguements,
 *  and creating pointers and null spaces per each arguement if there are
 *  multiple.
 */


 /* Main entry point.
  * argc    A count of command-line arguments
  * argv    The command-line argument values
  *
  * Micro-make (umake) reads from the uMakefile in the current working
  * directory.  The file is read one line at a time.  Lines with a leading tab
  * character ('\t') are interpreted as a command and passed to processline minus
  * the leading tab.
  */
 int main(int argc, char* argv[]) {

   FILE* makefile = fopen("./uMakefile", "r");

   if (makefile == NULL){
     perror("fopen");
     exit(EXIT_FAILURE);
   }else{

   size_t  bufsize = 0;
   char*   line    = NULL;
   ssize_t linelen = getline(&line, &bufsize, makefile);

   targets* historian = (targets*) malloc(sizeof(targets));
   historian->one_tgt = NULL;
   historian->two_dpndt = NULL;
   historian->rules = NULL;
   historian->next = NULL;

   int target_acc = 0;

   while(-1 != linelen) {

     if(line[linelen-1]=='\n') {
       linelen -= 1;
       line[linelen] = '\0';
     }


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
      line = NULL;

     }

     }

     linelen = getline(&line, &bufsize, makefile);
   }

   if(argc > 1){

     for (int i = 1; i < argc; i++){
       //printf("WHATS FOR LOOPING in %d?\n", i);

       targets* end_target_list = *(&historian);
       char* goal = argv[i];
       execute_dpt_hlp(goal, &end_target_list);

     }
 }
   free(line);
   return EXIT_SUCCESS;
 }
 }
 /*execute_dpt
  *recursive calling dependencies for a target.
  *
 */
 void execute_dpt(char** dpt, targets** roster){

     while(*dpt != NULL){
       execute_dpt_hlp(*dpt, roster);
       dpt++;
     }

 }

 /*execute_dpt_hlp
  *recursive calling dependencies for a target.
  *
 */

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


 /*hlp_process
  *passes all rules for a target into processline
 */
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
    fprintf(stderr, "expand: invalid rule\n");
    exit(-1);
  }
   char** lne = arg_parse(new_rule, &is_arg);


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

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
   //printf("ASFJ");
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
       //ignore

       break;
     }

     case 0: {
 //is rule

       add_rules(&line[1], target_acc, &historian);
       break;

     }
 //is target/dependent
     case 1: {

       add_tgt_dpt0(&line[0], &historian);
       target_acc++;
       break;
     }

 //    default: {
 //      perror("is_alphabet");
 //      exit(EXIT_FAILURE);
 //    }
     }
     linelen = getline(&line, &bufsize, makefile);
   }

   if(argc > 1){

     for (int i = 1; i < argc; i++){
       //printf("WHATS FOR LOOPING in %d?\n", i);

       targets* end_target_list = *(&historian);
       while(end_target_list != NULL){
         //printf("TOTALLY NOT NULL LOL\n");

       //printf("argv[i] = %s\n\n", argv[i]);

     //printf("end_target_list->one_tgt = %s\n", end_target_list->one_tgt);
       //printf("RULES: %s\n", end_target_list->rules->rule_detail);
         if (strcmp(end_target_list->one_tgt, argv[i]) == 0){
           //printf("OMG WE THE SAME YEET\n\n");
         //printf("comparison = yes\n");
           hlp_process(&(end_target_list->rules));
         }

           end_target_list = end_target_list->next;

       }
     }
 }

   free(line);
   return EXIT_SUCCESS;
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
 /* Process Line
  *
  */
 void processline (char* line) {
   //int buf_count;
   //char* wild_west = ;
   //int expansion = expand(line, wild_west, buf_count);

   int is_arg;
   char** lne = arg_parse(line, &is_arg);

   const pid_t cpid = fork();
   switch(cpid) {

   case -1: {
     perror("fork");
     break;
   }

   case 0: {
     if(is_arg > 0){
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

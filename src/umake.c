/* CSCI 347 micro-make
 *
 * 05 Oct 2018, Martin Smith
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "arg_parse.h"
#include "target.h"

/* PROTOTYPES */

/* Process Line
 * line   The command line to execute.
 * This function interprets line as a command line.  It creates a new child
 * process to execute the line and waits for that process to complete.
 */
void processline(char* line);

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
 * the leading tab.rg_
 */
int main(int argc, const char* argv[]) {
  FILE* makefile = fopen("./uMakefile", "r");

  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);

  int arg_nums = 0;
  //int rule_num = 0;
  int first_tgt_flag = 0;
  char*** tgt_dpt = malloc((arg_nums + 1) * sizeof(char*));
  char*** rulebook = malloc((arg_nums + 1) * sizeof(char**));
  target* working_tgt = malloc(sizeof(target));;
  target* head = working_tgt;

  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(line[0] != '\t'){
      //processline(&line[1]);
      if(is_colon(&line[0]) == 0){

        if (first_tgt_flag == 0){
          first_tgt_flag++;
          //printf("first-target\n");
        }else{
          //rule_num = 0;
          //printf("going to next-target...");
          working_tgt = working_tgt->next;
          working_tgt = malloc(sizeof(target));
          //printf("done\n");
          //printf("clean rulebook & others...");
          rulebook = malloc((arg_nums + 1) * sizeof(char*));
          tgt_dpt = malloc((arg_nums + 1) * sizeof(char**));
          //printf("done\n");
        }
        //printf("I have a colon\n");
        int args_total = 0;
        int target_total = 0;
        int dep_total = 0;
        tgt_dpt = target_parse(&line[0], &args_total, &target_total, &dep_total);

        //printf("ACTUALLY adding targets...\n");
        //printf("%s\n", *tgt_dpt[0]);
        //printf("%d\n", args_total);
        //printf("YES?\n");
        //working_tgt->one_tgt = tgt_dpt[0];
        int total_targets = args_total - target_total;
        int total_depends = args_total - dep_total;
        //printf("De %d\n", total_depends);
        //printf("Ta %d\n", total_targets);

        //if(total_targets == 1){
          //printf("onetarget\n");
          //working_tgt->one_tgt = *tgt_dpt[0];
        //}else{
          //printf("onetarget\n");
          working_tgt->one_tgt = tgt_dpt[0];
        //}
        //if(total_depends != 0){
          //printf("depends\n");
          working_tgt->two_dpndt = tgt_dpt[1];
        //}
        //printf("done\n");
        //printf("%d\n", rule_count);
        //printf("%d\n", args_total);


      /*  if(rule_count != 0 ){
          printf("asd\n");
          rule_count++;
          int garbage;
          printf("%s\n", &line[rule_count]);
          if(&line[rule_count] == ":"){
            printf("PLEASEPEA\n");
          }
          dependents = arg_parse(&line[rule_count], &garbage);
          printf("adding dependents...");
          working_tgt->two_dpndt = dependents;
          printf("done\n");
        }*/

      }

    }else{
      //processline(&line[1]);
      //printf("No colon - must be a rule\n");
      char** list_rule = arg_parse(&line[1], &arg_nums);
      int rule_num = 0;
        if(arg_nums > 0){
          //printf("NUM ARGS for Rules: %d\n", arg_nums);
          rulebook = add_rules(list_rule, rulebook, rule_num);
          rule_num++;
        }
        //printf("Adding to rulebook... num_rules: %d\n", rule_num);
        working_tgt->three_rule = rulebook;
    }

    linelen = getline(&line, &bufsize, makefile);
  }
  working_tgt->next = NULL;
  if(argc > 1){
    for (int i = 1; i < argc; i++){
        if(comparison(argv[i], working_tgt)){
          processline();
        }
    }
  }

  free(line);
  return EXIT_SUCCESS;
}


/* Process Line
 *
 */
void processline (char* line) {
  int num_arg;
  char** lne = arg_parse(line, &num_arg);
  //printf("num_args: %d\n", num_arg);

  const pid_t cpid = fork();
  switch(cpid) {

  case -1: {
    perror("fork");
    break;
  }

  case 0: {
    if (num_arg > 0){
    execvp(lne[0], lne);

    perror("execvp");
    exit(EXIT_FAILURE);
    }
    break;
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

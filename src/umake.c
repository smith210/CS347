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

  int arg_nums;
  int rule_num = 0;
  int first_tgt_flag = 0;
  char** targets = malloc((rule_num + 1) * sizeof(char*));
  char** dependents = malloc((rule_num + 1) * sizeof(char*));
  char*** rulebook = malloc((rule_num + 1) * sizeof(char**));
  target* working_tgt;

  while(-1 != linelen) {
    working_tgt = malloc(sizeof(target));
    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(line[0] != '\t'){
      //processline(&line[1]);
      if(is_colon(&line[0]) == 0){

        if (first_tgt_flag == 0){
          first_tgt_flag++;
          printf("first-target\n");
        }else{
          rule_num = 0;
          printf("going to next-target...");
          working_tgt = working_tgt->next;
          printf("done\n");
          printf("clean rulebook & others...");
          rulebook = malloc((rule_num + 1) * sizeof(char*));
          dependents = malloc((rule_num + 1) * sizeof(char*));
          targets = malloc((rule_num + 1) * sizeof(char**));
          printf("done\n");
        }
        printf("I have a colon\n");
        int rule_count = 0;
        targets = target_parse(&line[0]);

        /*while (rule_count != arg_nums){
          printf("target: %s\n", tgt_dpt[rule_count]);
          if(find_colon(&tgt_dpt[rule_count]) == 0){
            printf("TARGET ADDING\n");
            targets = add_tgt(tgt_dpt[rule_count], targets, rule_count);
            printf("Should have colon here: %s\n", tgt_dpt[rule_count]);
            break;
          }
          rule_count++;
        }*/
        printf("ACTUALLY adding targets...");
        working_tgt->one_tgt = targets;
        printf("done\n");
        printf("%d\n", rule_count);
        printf("%d\n", arg_nums);

        if(rule_count != arg_nums){
          rule_count++;
          int garbage;
          dependents = arg_parse(&line[rule_count], &garbage);
          printf("adding dependents...");
          working_tgt->two_dpndt = dependents;
          printf("done\n");
        }

      }

    }else{
      //processline(&line[1]);
      printf("No colon - must be a rule\n");
      char** list_rule = arg_parse(&line[1], &arg_nums);
        if(arg_nums > 0){
          printf("NUM ARGS for Rules: %d\n", arg_nums);
          rulebook = add_rules(list_rule, rulebook, rule_num);
          rule_num++;
        }
        printf("Adding to rulebook... num_rules: %d\n", rule_num);
        working_tgt->three_rule = rulebook;
    }

    linelen = getline(&line, &bufsize, makefile);
  }
  working_tgt->next = NULL;
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

/* CSCI 347 micro-make
 *
 * 05 Oct 2018, Martin Smith
*(I mentioned this in my target.c file, but it seemed necessary to include in
*here as well). I didn't realize until recently that I have several
*misunderstandings about linked lists
*(especially when it came to pointers and setting them up) until
*too late, so the code will not exactly work up to par for linking to lists
*(for now).
*
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
void processline(char** line);
void hlp_process(char*** rule_set);

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
int main(int argc, char* argv[]) {
  FILE* makefile = fopen("./uMakefile", "r");

  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);

  int arg_nums = 0;
  int first_tgt_flag = 0;
  char*** tgt_dpt = malloc((arg_nums + 1) * sizeof(char*));
  char*** rulebook = malloc((arg_nums + 1) * sizeof(char**));
  target* working_tgt = malloc(sizeof(target));;


  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(line[0] != '\t'){
      if(is_colon(&line[0]) == 0){
        if (first_tgt_flag == 0){
          first_tgt_flag++;
        }else{
          //reset values
          working_tgt = working_tgt->next;
          working_tgt = malloc(sizeof(target));
          rulebook = malloc((arg_nums + 1) * sizeof(char*));
          tgt_dpt = malloc((arg_nums + 1) * sizeof(char**));
        }

        tgt_dpt = target_parse(&line[0]);

        working_tgt->one_tgt = tgt_dpt[0];
        working_tgt->two_dpndt = tgt_dpt[1];

      }

    }else{

      char** list_rule = arg_parse(&line[1], &arg_nums);
      int rule_num = 0;
        if(arg_nums > 0){
          rulebook = add_rules(list_rule, rulebook, rule_num);
          rule_num++;
        }

        working_tgt->three_rule = rulebook;
    }

    linelen = getline(&line, &bufsize, makefile);
  }

  //printf("out of the wood\n");
  char** tempo = working_tgt->one_tgt;
  //printf("%s\n", *tempo);
  working_tgt->next = NULL;
  tempo = working_tgt->one_tgt;
  //printf("%s\n", *tempo);


  if(argc > 1){
    //printf("argc >1\n");
    target* head_place = working_tgt;
    for (int i = 1; i < argc; i++){
      //printf("%d\n", i);
      //printf("%s\n", argv[i]);
      while(head_place != NULL){
        //printf("head_place->next != NULL\n");
        if (comparison(argv[i], head_place) == 0){
          //printf("comparison = yes\n");
          hlp_process(head_place->three_rule);
        }else{
          head_place = head_place->next;
        }
      }
    }

  }

  free(line);
  return EXIT_SUCCESS;
}

/* help_process
 * takes parsed args into processline
 */
void hlp_process(char*** rule_set){
  //printf("hlp_process\n");
  int count = 0;
  while (rule_set[count] != NULL){
    processline(rule_set[count]);
  }

}


/* Process Line
 *
 */
void processline (char** line) {
  //int num_arg;
  //char** lne = arg_parse(line, &num_arg);
  //printf("num_args: %d\n", num_arg);

  const pid_t cpid = fork();
  switch(cpid) {

  case -1: {
    perror("fork");
    break;
  }

  case 0: {
    //if (num_arg > 0){
    execvp(line[0], line);

    perror("execvp");
    exit(EXIT_FAILURE);
    //}
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

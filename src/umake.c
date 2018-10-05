/* CSCI 347 micro-make
 *
 * 05 Oct 2018, Martin Smith
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* CONSTANTS */
unsigned int argCount(char* line);

/* PROTOTYPES */

/* Process Line
 * line   The command line to execute.
 * This function interprets line as a command line.  It creates a new child
 * process to execute the line and waits for that process to complete.
 */
void processline(char* line);

/* Arg Parse
*  line
*
*
*/
char** arg_parse(char*line);

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
  return EXIT_SUCCESS;
}


/* Process Line
 *
 */
void processline (char* line) {
  char** unkownVar = arg_parse(line);
  const pid_t cpid = fork();
  switch(cpid) {

  case -1: {
    perror("fork");
    break;
  }

  case 0: {
    execlp(line, line, (char*)(0));
    perror("execlp");
    exit(EXIT_FAILURE);
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
/*  argCount
*
*
*/

unsigned int argCount(char* line){
    unsigned int count = 0;

    while (*line != '\0'){
        if(*line != ' '){
            count++;
            while (*line != '\0' || *line != ' '){
              line++;
            }
        }
        if(*line != '\0'){
            line++;
        }
    }
    return count;
}
/* Arg Parse
*
*/
char** arg_parse(char* line){
    char** args = malloc ((argCount(line) + 1) * sizeof(char*));

    unsigned int argCurr = 0;
    while(*line != '\0'){
        while(*line == ' '){
            line++;
        }
        *args[argCurr] = *line;
        argCurr++;
        while(*line != ' ' || *line != '\0'){
            line++;
        }
        if(*line == ' '){
            *line = '\0';
            line++;
        }
    }
    *args[argCurr] = *line;

    return args;
}

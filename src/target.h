#ifndef HEADER_TARGET
#define HEADER_TARGET

typedef struct rule_st{
  char* rule_detail;
  struct rule_st* rule_next;
}rules;

typedef struct target_st{
    char* one_tgt;
    char** two_dpndt;
    rules* rules;
    struct target_st* next;
}targets;

void add_target_dsc(char* line, targets** target_rule, char*** dpts, int dpt_sz);
void add_tgt_dpt0(char* line, targets** list_tgt, char*** dpts, int dpt_sz);

void add_rules(char* line, int type_target, targets** holder);
int is_alphabet(char* line);
int is_tgt_alphabet(char* line);
int check_arg_0(char* line);
unsigned int is_colon(char* line);
int has_equal(char* line);
char** retrieve_dpt(char* line, int* sz);
void printdpt(char** lines, int sz);

int expand(char* orig, char* new, int newsize);
char* dup_env(char* line);
int expand_alphabet(char* line);
int bracket_check(char* line);
 void execute_dpt(char** dpt, targets** roster);
  int valid_env(char* line);
int size_line (char* line);

#endif

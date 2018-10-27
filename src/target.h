#ifndef HEADER_TARGET
#define HEADER_TARGET

unsigned int is_colon(char* line);
unsigned int find_colon(char** line);
int state_one(char* line);
char** add_tgt(char* new_rule, char** guideline, int num_arg);
char*** add_rules(char** new_rule, char*** guideline, int num_arg);
int is_tgt_alphabet(char* line);
char** target_parse(char* line);
int is_tgt_q0(char* line);
int is_tgt_q1(char* line);
int is_tgt_q2(char* line);

typedef struct target_st{
    char** one_tgt;
    char** two_dpndt;
    char*** three_rule;
    struct target_st* next;
}target;

target collect_tgts(char* line);
target assign_tgts(char* line, target collector, int* curr_arg, int* rule_tracker);
int target_search(target* t, char* name);

#endif

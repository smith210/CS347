#ifndef HEADER_TARGET
#define HEADER_TARGET

typedef
struct target_st{
    char* one_target;
    char** two_dependent;
    char*** three_rule;
    struct target_st* next;
}target;

#endif

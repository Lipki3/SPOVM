#ifndef PARENT_HEADER_H
#define PARENT_HEADER_H
void print_envp(char* envp[]);
char** create_child_env(char* fenvp);

char* search_child_path(char** str_arr);

#endif //PARENT_HEADER_H

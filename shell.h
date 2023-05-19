#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *command);
char *get_location(const char *command);
void execute_commands_from_file(const char *filename);
void display_prompt(void);
void remove_newline(char *str);
void execute_command(char *command);
void handle_comments(char *command);
void execute_unsetenv(int num_args, char **args);
void execute_setenv(int num_args, char **args);
void execute_cd(int num_args, char **args);
int parse_command(char *command, char **args);
void replace_variables(char *command, int status);
char *replace_string(char *str, const char *old_str, const char *new_str);
int execute_external(char *command, int status);
void print_aliases(void);
void execute_alias(int num_args, char *args[]);

#endif /* SHELL_H */

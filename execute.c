#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "shell.h"

#define MAX_ARGS 100
#define MAX_COMMAND_LENGTH 100
/**
 * replace_variable - replace a variable
 * @command: pointer to command passed
 * Return: void
 */
void replace_variables(char *command, int status)
{
	char exit_status_str[4];  /* Maximum 3 digits for exit status */
	char pid_str[10];         /* Maximum 9 digits for process ID */
	char *variable;
	char *replacement;

	/* Replace $? with the exit status of the previous command */
	snprintf(exit_status_str, sizeof(exit_status_str), "%d", status);
	variable = "$?";
	replacement = exit_status_str;

	while ((command = strstr(command, variable)) != NULL)
	{
		strncpy(command, replacement, strlen(replacement));
		command += strlen(replacement);
	}

	/* Replace $$ with the process ID of the shell */
	snprintf(pid_str, sizeof(pid_str), "%d", getpid());
	variable = "$$";
	replacement = pid_str;

	while ((command = strstr(command, variable)) != NULL)
	{
		strncpy(command, replacement, strlen(replacement));
		command += strlen(replacement);
	}
}
/**
 * replace_string - replaces a given string
 * @str: pointer to the string
 * @old_str: pointer to the old str
 * @new_str: pointer to the new str
 * Return: result
 */
char *replace_string(char *str, const char *old_str, const char *new_str)
{
	char *result;
	int i = 0, count = 0;
	size_t new_len = strlen(new_str);
	size_t old_len = strlen(old_str);

	/* Count the number of occurrences of old_str in str */
	for (i = 0; str[i] != '\0'; i++)
	{
		if (strstr(&str[i], old_str) == &str[i])
		{
			count++;
			i += old_len - 1;
		}
	}

	/* Allocate memory for the new string */
	result = (char *)malloc(i + count * (new_len - old_len) + 1);

	while (*str)
	{
		/* Compare the substring with the result */
		if (strstr(str, old_str) == str)
		{
			strcpy(&result[i], new_str);
			i += new_len;
			str += old_len;
		}
		else
		{
			result[i++] = *str++;
		}
	}

	result[i] = '\0';
	return (result);
}

/**
 * execute_external - executes an external command
 * @command: pointer to input command
 * @status: status of the previous command
 * Return: exit status
 */
int execute_external(char *command, int status)
{
	int logical_operator = 0;  /* 0: No operator, 1: &&, 2: || */
	char *args[MAX_ARGS];
	char *token;
	int arg_count = 0;
	int i;
	int exit_status;
	pid_t pid;

	replace_variables(command, status);

	/* Tokenize the command into arguments */
	token = strtok(command, " ");

	while (token != NULL)
	{
		args[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;

	/* Check for logical operators (&& and ||) */
	for (i = 0; i < arg_count; i++)
	{
		if (strcmp(args[i], "&&") == 0)
		{
			args[i] = NULL;
			logical_operator = 1;
		}
		else if (strcmp(args[i], "||") == 0)
		{
			args[i] = NULL;
			logical_operator = 2;
		}
	}

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		/* Check the exit status of the command */
		exit_status = WEXITSTATUS(status);

		if (logical_operator == 1 && exit_status != 0)
		{
			/* The command failed, and it was followed by && */
			fprintf(stderr, "Command failed\n");
		}
		else if (logical_operator == 2 && exit_status == 0)
		{
			/* The command succeeded, and it was followed by || */
			fprintf(stderr, "Command succeeded\n");
		}
	}

	return (exit_status);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "shell.h"
#define MAX_ARGS 50
#define MAX_PATH_LENGTH 100
/**
 * execute_command - executes commands passed in the shell
 * @command: pointer to the command passed
 * Return: void
 */
void execute_command(char *command)
{
	char *args[MAX_ARGS];
	int num_args = parse_command(command, args), exit_status, status = 0;

	if (num_args > 0)
	{
		if (strcmp(args[0], "cd") == 0)
		{
			/* Handle cd command */
			execute_cd(num_args, args);
		}
		else if (strcmp(args[0], "setenv") == 0)
		{
			/* Handle setenv command */
			execute_setenv(num_args, args);
		}
		else if (strcmp(args[0], "unsetenv") == 0)
		{
			/* Handle unsetenv command */
			execute_unsetenv(num_args, args);
		}
		else if (strcmp(args[0], "alias") == 0)
		{
			/* Handle alias command */
			execute_alias(num_args, args);
		}
		else if (strcmp(args[0], "exit") == 0)
		{
			/* Handle the exit command */
			if (args[1] != NULL)
			{
				exit_status = atoi(args[1]);
				exit(exit_status);
			}
		}
		else
		{
			/* Execute external command */
			execute_external(command, status);
		}
	}
}

/**
 * parse_command - parses the command string into arguments
 * @command: the command string
 * @args: an array to store the arguments
 * Return: the number of arguments
 */
int parse_command(char *command, char **args)
{
	int num_args = 0;
	char *token;

	token = strtok(command, " \t\n");
	while (token != NULL && num_args < MAX_ARGS - 1)
	{
		args[num_args] = token;
		num_args++;
		token = strtok(NULL, " \t\n");
	}

	args[num_args] = NULL;

	return (num_args);
}

/**
 * execute_cd - handles the cd command
 * @num_args: number of arguments
 * @args: array of arguments
 */
void execute_cd(int num_args, char **args)
{
	char *dir = args[1];
	char cwd[MAX_PATH_LENGTH];

	if (num_args > 2)
	{
		fprintf(stderr, "cd: Too many arguments\n");
		return;
	}
	else
	{
		if (num_args == 1 || strcmp(dir, "~") == 0)
		{
			/* Change to home directory */
			dir = getenv("HOME");

			if (dir == NULL)
			{
				fprintf(stderr, "cd: No home directory\n");
				return;
			}
		}
		else if (strcmp(dir, "-") == 0)
		{
			/* Change to previous directory */
			dir = getenv("OLDPWD");

			if (dir == NULL)
			{
				fprintf(stderr, "cd: No previous directory\n");
				return;
			}
			printf("%s\n", dir);
		}

		if (chdir(dir) == 0)
		{
			getcwd(cwd, sizeof(cwd));
			setenv("OLDPWD", getenv("PWD"), 1);
			setenv("PWD", cwd, 1);
		}
		else
		{
			fprintf(stderr, "cd: No such file or directory: %s\n", dir);
		}
	}
}

/**
 * execute_setenv - handles the setenv command
 * @num_args: number of arguments
 * @args: array of arguments
 */
void execute_setenv(int num_args, char **args)
{
	const char *name = args[1];
	const char *value;

	if (num_args < 2)
	{
		fprintf(stderr, "setenv: Too few arguments\n");
	}
	else if (num_args > 3)
	{
		fprintf(stderr, "setenv: Too many arguments\n");
	}
	else
	{
		value = (num_args == 3) ? args[2] : "";

		if (setenv(name, value, 1) != 0)
		{
			fprintf(stderr, "setenv: Failed to set environment variable\n");
		}
	}
}

/**
 * execute_unsetenv - handles the unsetenv command
 * @num_args: number of arguments
 * @args: array of arguments
 */
void execute_unsetenv(int num_args, char **args)
{
	const char *name = args[1];

	if (num_args < 2)
	{
		fprintf(stderr, "unsetenv: Too few arguments\n");
	}
	else if (num_args > 2)
	{
		fprintf(stderr, "unsetenv: Too many arguments\n");
	}
	else
	{
		if (unsetenv(name) != 0)
		{
			fprintf(stderr, "unsetenv: Failed to unset environment variable\n");
		}
	}
}

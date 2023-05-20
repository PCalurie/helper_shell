#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define MAX_INPUT_LENGTH 80

/**
 * display_prompt - displays prompt on shell
 */
void display_prompt(void)
{
	printf("simple_shell> ");
}

/**
 * remove_newline - removes new lines
 * @str: pointer to the string
 */
void remove_newline(char *str)
{
	str[strcspn(str, "\n")] = '\0';
}

/**
 * main - main function
 * @argc: argument count
 * @argv: number of arguments
 * Return: 0 for success
 */
int main(int argc, char *argv[])
{
	char input[MAX_INPUT_LENGTH];
	char *token;
	char *args[10]; /* Assuming a maximum of 10 arguments */
	int num_args;

	if (argc > 1)
	{
		/* File mode - read commands from a file */
		FILE *file = fopen(argv[1], "r");
		char *token;

		if (file == NULL)
		{
			fprintf(stderr, "Error opening file: %s\n", argv[1]);
			return (EXIT_FAILURE);
		}

		while (fgets(input, sizeof(input), file))
		{
			token = strtok(input, "");
			if (strcmp(token, "exit\n") == 0) {
				break;
			}
			remove_newline(input);
			execute_command(input);
		}

	fclose(file);
	}
	else
	{
		/* Interactive mode - read commands from the user */
		while (1)
		{
			display_prompt();

			if (fgets(input, sizeof(input), stdin) == NULL)
			break;

			handle_comments(input);

			remove_newline(input);

		/* Split the input by semicolon and execute each command */
			token = strtok(input, ";");

			while (token != NULL)
			{
				remove_newline(input);
				num_args = parse_command(token, args);
				
/* Call the execute_alias function with the correct arguments */
				execute_alias(num_args, args);
				execute_command(token);
				token = strtok(NULL, ";");
			}
		}
	}

	return (EXIT_SUCCESS);
}

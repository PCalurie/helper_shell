#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
/**
 * execute_commands_from_file - execute commands
 * @filename: name of the file
 * Return: void
 */
void execute_commands_from_file(const char *filename)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	FILE *file = fopen(filename, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
		return;
	}

	while ((read = getline(&line, &len, file)) != -1)
	{
		/* Remove newline character from the end of the line */
		if (line[read - 1] == '\n')
		{
			line[read - 1] = '\0';
		}

/* Execute the command */
		execute_command(line);
	}

	free(line);
	fclose(file);
}

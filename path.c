#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "shell.h"
/**
 * get_location - gets the path
 * @command: command input
 * Return: path copy
 */
char *get_location(const char *command)
{
	struct stat st;
	char *path = getenv("PATH");
	char *path_copy, *directory, *file_path;

	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	directory = strtok(path_copy, ":");

	while (directory != NULL)
	{
		file_path = malloc(strlen(directory) + strlen(command) + 2);

		strcpy(file_path, directory);
		strcat(file_path, "/");
		strcat(file_path, command);

		if (stat(file_path, &st) == 0)
		{
			free(path_copy);
			return (file_path);
		}

		free(file_path);
		directory = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

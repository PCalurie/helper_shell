#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

#define MAX_ALIAS_COUNT 100
#define MAX_ALIAS_LENGTH 50
typedef struct
{
	char name[MAX_ALIAS_LENGTH];
	char value[MAX_ALIAS_LENGTH];
} Alias;
/**
 * print_aliases - handles the alias command
 */
void print_aliases(void)
{
	int i, alias_count = 0;
	Alias aliases[MAX_ALIAS_COUNT];

	for (i = 0; i < alias_count; i++)
	{
		printf("%s='%s'\n", aliases[i].name, aliases[i].value);
	}
}
/**
 * execute_alias - handles the alias command
 * @num_args: number of arguments
 * @args: array of arguments
 */
void execute_alias(int num_args, char *args[])
{
	char *arg = args[0];
	char *name = arg;
	char *equal_sign;
	char *value;
	int existing_index, j, i, found;
	Alias aliases[MAX_ALIAS_COUNT];
	int alias_count = 0;

	if (num_args == 1)
	{
		/* Print all aliases */
		print_aliases();
	}
	else
	{
		for (i = 1; i < num_args; i++)
		{
			equal_sign = strchr(arg, '=');

			if (equal_sign != NULL)
			{
				/* Define an alias with value */
				equal_sign = '\0';
				value = equal_sign + 1;
				/* Check if the alias already exists */
				existing_index = -1;

				for (j = 0; j < alias_count; j++)
				{
					if (strcmp(aliases[j].name, name) == 0)
					{
						existing_index = j;
						break;
					}
				}

				if (existing_index != -1)
				{
					strcpy(aliases[existing_index].value, value);
				}
				else
				{
					/* Add a new alias */
					if (alias_count < MAX_ALIAS_COUNT)
					{
						strcpy(aliases[alias_count].name, name);
						strcpy(aliases[alias_count].value, value);
						alias_count++;
					}
					else
					{
						fprintf(stderr, "alias: Maximum alias count reached\n");
					}
				}
			}
			else
			{
				/* Print the value of an alias */
				found = 0;
				for (j = 0; j < alias_count; j++)
				{
					if (strcmp(aliases[j].name, arg) == 0)
					{
						printf("%s='%s'\n", aliases[j].name, aliases[j].value);
						found = 1;
						break;
					}
				}
				if (!found)
				{
					fprintf(stderr, "alias: Unknown alias: %s\n", arg);
				}
			}
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
/**
* handle_comments - handles comments on shell
* @command: input on shell
* Return: void
*/
void handle_comments(char *command)
{
	char *comment_start = strchr(command, '#');
	if (comment_start != NULL)
	{
		*comment_start = '\0';
	}
}

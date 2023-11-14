#include "shell.h"

/**
 * error_file - handle error when file cannot be found or
 * if the file name is too long or the user haven't the permission to
 * open the file
 *
 * @shellData: Structure with all data of the shell
 * @nerr: The number of the error occured:
 * 0 for file not found
 * 1 for File name too long
 * 2 for Permission denied
 *
 * Return: EKEYEXPIRED
 */
int error_file(shellData *shellData, int nerr)
{
	char *merr[3] = {	": not found\n",
						": File name too long\n",
						": Permission denied\n"};

	fprintf(stderr, "%s: %d: %s%s", shellData->argv[0], shellData->loopCount,
			 shellData->args[0], merr[nerr]);

	return (-EKEYEXPIRED);
}

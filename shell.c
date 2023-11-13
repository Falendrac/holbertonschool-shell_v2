#include "shell.h"

/**
 * _chooseExecProcess - Test the commands passes by the user to execute
 * the correct command
 *
 * @datas: The pointer with all datas of the shell
 * @size_test: Test if the size of the arguments passes is not over 255 char
*/
void _chooseExecProcess(shellData *datas, int size_test)
{
	int path_exec = 1, env_exec = 1;
	struct stat st;

	if (datas->buffer != NULL && datas->args != NULL && size_test == 0)
	{
		if ((datas->args[0][0] == '.' && datas->args[0][1] != '\0')
			|| datas->args[0][0] != '.')
		{
			if ((_strcmp(datas->args[0], "env") == 0 ||
				_strcmp(datas->args[0], "printenv") == 0))
				env_exec = _printenv(datas->env, datas->args);

			if (datas->args[0][0] != '.' && env_exec != 0)
				path_exec = test_with_path(datas);

			if (_strcmp(datas->args[0], "exit") == 0)
				exit_procedure(datas);

			if (datas->buffer != NULL && path_exec == 1
				&& stat(datas->args[0], &st) == 0)
				datas->status = _execute(datas->args[0], datas);

			else if (datas->buffer != NULL && path_exec == 1 && env_exec == 1)
				datas->status = error_file(datas, 0);
		}
	}
}

/**
 * loop_asking - Infinite loop with invoking some functions
 * for asking to user a command, and execute the command if is
 * possible
 *
 * @datas: The pointer of the data structure of the shell
 */
void loop_asking(shellData *datas)
{
	int size_test = 0;

	do {
		datas->loopCount++;

		_prompt();
		signal(SIGINT, sigint_handle);
		datas->buffer = _getline(datas);

		datas->args = separate_av(datas->buffer, " \t\n\v\r\f");
		if (datas->args != NULL && _strlen(datas->args[0]) > 255)
		{
			datas->status = error_file(datas, 1);
			size_test = 1;
		}

		_chooseExecProcess(datas, size_test);

		if (datas->args != NULL)
			free_separate_av(datas->args);

		free(datas->buffer);
		size_test = 0;
	} while (1);
}

/**
 * _execute - Execute the command passes
 *
 * @cmd: The command passes by the user
 * @datas: The data structure with all data of the shell
 *
 * Return: 1 if an error is occurs, 0 if is a success
 */
int _execute(char *cmd, shellData *datas)
{
	pid_t child_pid;
	int status;

	if ((cmd[0] == '.' && cmd[1] == '.' && cmd[3] == '\0') || access(cmd, X_OK))
		return (error_file(datas, 2));

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return (2);
	}
	if (child_pid == 0)
	{
		if (execve(cmd, datas->args, environ) == -1)
		{
			return (2);
		}
	}
	else
	{
		wait(&status);
	}

	if (status > 0)
		status = 2;

	return (status);
}

shellData *_shellDataInitialisation(char *argv[])
{
	shellData *datas = NULL;

	datas = malloc(sizeof(shellData));
	if (!datas)
		return (NULL);

	datas->env = create_env_variable();
	datas->path = create_path_variable(datas->env);
	datas->loopCount = 0;
	datas->argv = argv;
	datas->buffer = "";
	datas->status = 0;

	return (datas);
}

/**
 * main - The main function for simple shell
 *
 * @argc: Count of the arguments passes
 * @argv: Arguments value with the name of the executable
 *
 * Return: 0 if is a success
 */
int main(__attribute__((unused)) int argc, char *argv[])
{
	shellData *datas = NULL;

	datas = _shellDataInitialisation(argv);
	if (!datas)
		return (1);

	loop_asking(datas);

	return (0);
}

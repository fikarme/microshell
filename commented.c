#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

// Every Child Sees Exec Magic
void	err(char *str);
int		cd(char **av, int i);
void	set_pipe(int has_pipe, int *fd, int end);
int		exec(char **av, int i, char **envp);
// int	main(int ac, char **av, char **envp)

// /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

void err(char *str)
{
    while (*str)
        write(2, str++, 1); // 2==stderr
}

int cd(char **av, int i)
{
    if (i != 2) // Check for exactly 2 arguments
        return err("error: cd: bad arguments\n"), 1;

    if (chdir(av[1]) == -1) // Change directory
        return err("error: cd: cannot change directory to "), err(av[1]), err("\n"), 1;
    return 0;
}

// configure I/O for command chaining
// end == 1 sets stdout to act as WRITE end of our pipe
// end == 0 sets stdin to act as READ end of our pipe
void set_pipe(int has_pipe, int *fd, int end)
{
	if (has_pipe)
		if (dup2(fd[end], end) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
			err("error: fatal\n"), exit(1);
}

int exec(char **av, int i, char **envp)
{
    int has_pipe, fd[2], pid, status;

    // Check if the command includes a pipe
    has_pipe = av[i] && !strcmp(av[i], "|");

    // If the command is 'cd', execute it
    if (!has_pipe && !strcmp(*av, "cd"))
        return cd(av, i);

    // If the command includes a pipe and creating the pipe fails, print error and exit
    if (has_pipe && pipe(fd) == -1)
        err("error: fatal\n"), exit(1);

    // Fork the process and if the fork fails, print error and exit
    if ((pid = fork()) == -1)
		err("error: fatal\n"), exit(1);

    if (!pid) // Child process
    {
        av[i] = 0; // Null-terminate the arguments at the pipe

        set_pipe(has_pipe, fd, 1); // Set up pipe for writing

        if (!strcmp(*av, "cd"))
            exit(cd(av, i));

        execve(*av, av, envp); // Execute the command

        err("error: cannot execute "), err(*av), err("\n"), exit(1);
    }

	// Parent process
    waitpid(pid, &status, 0); // Wait for the child process

    set_pipe(has_pipe, fd, 0); // Set up pipe for reading in the next command

    return WIFEXITED(status) && WEXITSTATUS(status);
}

// parse commands, separate, execute
int main(int ac, char **av, char **envp)
{
	(void)ac;
    int    i = 0, status = 0;

    // Loop through each following argument
    while (av[i])
    {
        // Move pointer to the next command
    	av += i + 1;
    	i = 0;

		// Find next pipe or semicolon
    	while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
			i++;

		// If there are arguments, execute them
    	if (i)
			status = exec(av, i, envp);
    }
    return status;
}
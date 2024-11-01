// /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// every child sees exec magic
void	err(char *str);
// CDE check directory exists
int		cd(char **av, int i); 
// PRE pipe right end
void	set_pipe(int has_pipe, int *fd, int end);
// EPC execute pipes children
int		exec(char **av, int i, char **envp); 
//int	main(int ac, char **av, char **envp)


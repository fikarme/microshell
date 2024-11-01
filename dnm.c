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

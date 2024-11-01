#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void err(char *s) { while (*s) write(2, s++, 1); }

int main(int ac, char **av, char **envp)
{
	(void)ac;
	int i = 0;
	int status = 0;
}
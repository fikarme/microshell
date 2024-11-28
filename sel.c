#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void err(char *s){
    while(*s)
        write(2, s++, 1);
}

int cd(char **av, int i){
    if (i != 2)
        return err("arg"), 1;
    if (chdir(av[1]) == -1)
        return err("dir"), err(av[1]), err("\n"), 1;
    return 0;
}

void set_pipe(int has_pipe, int *fd, int end){
    if (has_pipe)
        if (dup2(fd[end], end) == -1
        || close(fd[0]) == -1
        || close(fd[1]) == -1)
            err("dup2"), exit(1);
}
//clang -Wall -Wextra -Werror -o microshell sel.c

// ./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
// microshell
// i love my microshell

// ./microshell /bin/echo WOOT "; /bin/echo NOPE;" "; ;" ";" /bin/echo YEAH
// WOOT ; /bin/echo NOPE; ; ;
// YEAH
int exec(char **av, int i, char **envp){
    int has_pipe = 0;
    int pid = 0;
    int status = 0;
    int fd[2];
    has_pipe = av[i] && !strcmp(av[i], "|");
    if (!has_pipe && !strcmp(*av, "cd"))
        return cd(av, i);
    if (has_pipe && pipe(fd) == -1)
        err("pipe"), exit(1);
    if ((pid = fork()) == -1)
        err("fork"), exit(1);
    if (!pid){
        av[i] = 0;
        set_pipe(has_pipe, fd, 1);
        if (!strcmp(*av, "cd"))
            exit(cd(av, i));
        execve(*av, av, envp);
        err("execve"), err(*av), err("\n"), exit(1);
    }
    waitpid(pid, &status, 0);
    set_pipe(has_pipe, fd, 0);
    return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **envp){
    (void)ac;
    int i = 0;
    int status = 0;
    while (av[i]){
        av += i + 1;
        i = 0;
        while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (i)
            status = exec(av, i, envp);
    }
    return status;
}
int exec(char **av, int i, char **envp){
    int has_pipe = 0;
    int status = 0;
    int pid = 0;
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

void set_pipe(int has_pipe, int *fd, int end){
    if (has_pipe)
        if (dup2(fd[end], end) == -1
        || close(fd[0]) == -1
        || close(fd[1]) == -1)
            err("dup2") ,exit(1);
}

if(!pid)
av[i] = 0;
set 1
if !s *av cd
    exit cd av i
execve *av av envp
err 
#ifndef IGRIS_CLONNER_H
#define IGRIS_CLONNER_H

#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <igris/datastruct/argvc.h>
#include <igris/dprint.h>

#include <memory>
#include <sys/wait.h>

namespace igris
{
    class subprocess
    {
        int pid;

      public:
        subprocess() = default;

        void set_pid(int pid)
        {
            this->pid = pid;
        }

        void terminate()
        {
            kill(pid, SIGTERM);
        }

        void wait()
        {
            int status;
            waitpid(pid, &status, 0);
        }
    };

    class clonner
    {
        int clonner_pid;
        std::set<std::shared_ptr<subprocess>> _childs;

        int pipefd[2];
        int oposite_pipefd[2];

      public:
        clonner(bool begin = true)
        {
            if (begin)
                init();
        }

        void starter()
        {
            while (1)
            {
                char cmd[128];
                char pidstr[16];
                int cmdlen = read(pipefd[0], cmd, 128);

                if (cmdlen == 0)
                    return;

                cmd[cmdlen] = 0;
                int pid = fork();

                if (pid == 0)
                {
                    char *argv[10];
                    int argc = argvc_internal_split(cmd, argv, 10);
                    argv[argc] = 0;

                    int sts = execve(argv[0], argv, NULL);
                }

                sprintf(pidstr, "%d", pid);
                strcpy(cmd, "pid:");
                strcat(cmd, pidstr);
                write(oposite_pipefd[1], cmd, strlen(cmd));
            }
        }

        void init()
        {
            pipe(pipefd);
            pipe(oposite_pipefd);
            clonner_pid = fork();

            if (clonner_pid == 0)
            {
                close(pipefd[1]);
                close(oposite_pipefd[0]);
                starter();
                write(STDERR_FILENO, "Warn: output from start\r\n", 25);
                exit(0);
            }

            close(pipefd[0]);
            close(oposite_pipefd[1]);
        }

        std::shared_ptr<subprocess> start_subprocess(const char *cmd)
        {
            char buf[128];

            write(pipefd[1], cmd, strlen(cmd));
            auto proc = std::make_shared<subprocess>();

            int len = read(oposite_pipefd[0], buf, 128);
            buf[len] = 0;

            if (memcmp(buf, "pid:", 4) == 0)
            {
                int pid = atoi(&buf[4]);
                proc->set_pid(pid);
            }

            _childs.insert(proc);
            return proc;
        }

        void terminate_starter()
        {
            if (clonner_pid)
            {
                kill(clonner_pid, 9);
            }

            clonner_pid = 0;
        }

        void terminate_childs()
        {
            for (auto child : _childs)
                child->terminate();

            _childs.clear();
        }

        const std::set<std::shared_ptr<subprocess>> &childs()
        {
            return _childs;
        }

        ~clonner()
        {
            terminate_starter();
        }
    };
}

#endif
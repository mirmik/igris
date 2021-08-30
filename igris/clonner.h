#ifndef IGRIS_CLONNER_H
#define IGRIS_CLONNER_H

#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <igris/dprint.h>

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
    };

    class clonner
    {
        int clonner_pid;
        std::set<subprocess> childs;

        int pipefd[2];
        int oposite_pipefd[2];

      public:
        void starter()
        {
            write(STDERR_FILENO, "STARTER", 7);

            while (1)
            {
                char buf[128];
                char buf2[128];
                int len = read(pipefd[0], buf, 128);

                sprintf(buf2, "%d", len);

                write(STDERR_FILENO, buf2, 3);
                write(STDERR_FILENO, buf, len);

                if (len == 0)
                {
                    write(STDERR_FILENO, "FINISH1\r\n", 9);
                    return;
                }

                write(oposite_pipefd[1], buf, len);
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

        subprocess start_subprocess(const char *cmd)
        {
            char buf[128];

            write(pipefd[1], cmd, strlen(cmd));
            auto proc = subprocess();

            read(oposite_pipefd[0], buf, 128);

            printf("%s", buf);

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

        ~clonner()
        {
            terminate_starter();
        }
    };
}

#endif
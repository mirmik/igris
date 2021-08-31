#ifndef IGRIS_CLONNER_H
#define IGRIS_CLONNER_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <memory>
#include <set>

#include <igris/datastruct/argvc.h>
#include <nos/print.h>

namespace igris
{
    class subprocess
    {
        int pid;
        int ipipe;
        int opipe;

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
            int ret = waitpid(pid, &status, WCONTINUED);

            PRINT(pid);
            PRINT(status);
            PRINT(ret);
        }

        void set_pipe_fds(int ipipe, int opipe)
        {
            this->ipipe = ipipe;
            this->opipe = opipe;
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

                int pipes_host_in_child_out[2];
                int pipes_child_out_host_in[2];

                pipe(pipes_host_in_child_out);
                pipe(pipes_child_out_host_in);

                if (pid == 0)
                {
                    char *argv[10];
                    int argc = argvc_internal_split(cmd, argv, 10);
                    argv[argc] = 0;

                    close(STDIN_FILENO);
                    close(STDOUT_FILENO);
                    dup2(pipes_host_in_child_out[1], STDOUT_FILENO);
                    dup2(pipes_child_out_host_in[0], STDIN_FILENO);
                    close(pipes_host_in_child_out[1]);
                    close(pipes_child_out_host_in[0]);

                    int sts = execve(argv[0], argv, NULL);
                }

                sprintf(pidstr, "%d", pid);
                strcpy(cmd, "pid:");
                strcat(cmd, pidstr);
                write(oposite_pipefd[1], cmd, strlen(cmd));

                send_fd(oposite_pipefd[1], &pipes_host_in_child_out[0], 1);
                send_fd(oposite_pipefd[1], &pipes_child_out_host_in[1], 1);
            }
        }

        void init()
        {
            socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
            socketpair(AF_UNIX, SOCK_STREAM, 0, oposite_pipefd);

            clonner_pid = fork();

            if (clonner_pid == 0)
            {
                close(pipefd[1]);
                close(oposite_pipefd[0]);
                starter();
                while (1)
                    ;
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

            int opipe;
            int ipipe;
            recv_fd(oposite_pipefd[0], &opipe, 1);
            recv_fd(oposite_pipefd[0], &ipipe, 1);

            proc->set_pipe_fds(ipipe, opipe);

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

        static void send_fd(int socket, int *fds, int n) // send fd by socket
        {
            struct msghdr msg = {0};
            struct cmsghdr *cmsg;
            char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
            memset(buf, '\0', sizeof(buf));
            struct iovec io = {.iov_base = &dup, .iov_len = sizeof(dup)};

            msg.msg_iov = &io;
            msg.msg_iovlen = 1;
            msg.msg_control = buf;
            msg.msg_controllen = sizeof(buf);

            cmsg = CMSG_FIRSTHDR(&msg);
            cmsg->cmsg_level = SOL_SOCKET;
            cmsg->cmsg_type = SCM_RIGHTS;
            cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

            memcpy((int *)CMSG_DATA(cmsg), fds, n * sizeof(int));

            if (sendmsg(socket, &msg, 0) < 0)
                nos::println("Failed to send message");
        }

        static void recv_fd(int socket, int *fds, int n)
        {
            struct msghdr msg = {0};
            struct cmsghdr *cmsg;
            char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
            memset(buf, '\0', sizeof(buf));
            struct iovec io = {.iov_base = &dup, .iov_len = sizeof(dup)};

            msg.msg_iov = &io;
            msg.msg_iovlen = 1;
            msg.msg_control = buf;
            msg.msg_controllen = sizeof(buf);

            if (recvmsg(socket, &msg, 0) < 0)
                nos::println("Failed to receive message");

            cmsg = CMSG_FIRSTHDR(&msg);

            memcpy(fds, (int *)CMSG_DATA(cmsg), n * sizeof(int));
        }
    };
}

#endif
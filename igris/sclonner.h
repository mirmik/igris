#ifndef IGRIS_CLONNER_H
#define IGRIS_CLONNER_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <memory>
#include <set>
#include <igris/datastruct/argvc.h>
#include <igris/event/delegate.h>

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

namespace igris
{
    class subprocess
    {
    public:
        int pid = 0;
        int ipipe = 0;
        int opipe = 0;

        igris::delegate<void> on_close = {};

    public:
        subprocess() = default;

        void sigchld() { on_close.invoke(); }

        void set_pid(int pid) { this->pid = pid; }

        void terminate() { kill(pid, SIGTERM); }

        void wait()
        {
            int status;
            waitpid(pid, &status, WCONTINUED);
        }

        void set_pipe_fds(int ipipe, int opipe)
        {
            this->ipipe = ipipe;
            this->opipe = opipe;
        }

        int input_fd() { return ipipe; }
        int output_fd() { return opipe; }
    };

    class sclonner
    {
        static std::set<std::shared_ptr<subprocess>> _childs;

    public:
        static std::shared_ptr<subprocess> start_subprocess(const char *ccmd)
        {
            int pid = fork();

            int pipes_host_in_child_out[2];
            int pipes_host_out_child_in[2];
            pipe(pipes_host_in_child_out);
            pipe(pipes_host_out_child_in);

            if (pid == 0)
            {
                close(pipes_host_in_child_out[0]);
                close(pipes_host_out_child_in[1]);

                char *cmd = strdup(ccmd);

                char *argv[10];
                int argc = argvc_internal_split(cmd, argv, 10);
                argv[argc] = 0;

                // close(STDIN_FILENO);
                // close(STDOUT_FILENO);
                //                int a = dup2(pipes_host_in_child_out[1],
                //                STDOUT_FILENO); int b =
                //                dup2(pipes_host_out_child_in[0],
                //                STDIN_FILENO); nos::println_to(nos::cerr, a);
                //                nos::println_to(nos::cerr, b);

                fflush(stdout);

                // close(pipes_host_in_child_out[1]);
                // close(pipes_host_out_child_in[0]);

                execve(argv[0], argv, NULL);
            }

            close(pipes_host_out_child_in[0]);
            close(pipes_host_in_child_out[1]);

            auto child = std::make_shared<subprocess>();
            child->set_pid(pid);
            child->set_pipe_fds(pipes_host_in_child_out[0],
                                pipes_host_out_child_in[1]);

            _childs.insert(child);
            return child;
        }

        static void terminate_childs()
        {
            for (auto child : _childs)
                child->terminate();

            _childs.clear();
        }

        static void sigchild_trap(int)
        {
            pid_t pid;
            int status;
            while ((pid = waitpid(-1, &status, WNOHANG)) != -1)
            {
                auto it =
                    std::find_if(_childs.begin(), _childs.end(),
                                 [pid](auto &a) { return a->pid == pid; });

                if (it != _childs.end())
                    (*it)->sigchld();
            }
        }

        static void install_sigchild_trap() { signal(SIGCHLD, sigchild_trap); }

        static const std::set<std::shared_ptr<subprocess>> &childs()
        {
            return _childs;
        }

        ~sclonner() { terminate_childs(); }
    };
}

#endif

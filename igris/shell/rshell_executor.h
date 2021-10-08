#ifndef IGRIS_RSHELL_EXECUTOR_H
#define IGRIS_RSHELL_EXECUTOR_H

#include <igris/shell/rshell.h>
#include <stdio.h>

#include <igris/dprint.h>

namespace igris
{
    class rshell_executor
    {
      public:
        virtual void execute(char *cmdbuf, int cmdlen, char *ansbuf,
                             int ansmax) = 0;
    };

    class rshell_executor_onelevel : public rshell_executor
    {
        const struct rshell_command *_cmdtable;

      public:
        rshell_executor_onelevel(const struct rshell_command *cmdtable)
            : _cmdtable(cmdtable)
        {
        }

        void execute(char *cmdbuf, int cmdlen, char *ansbuf,
                     int ansmax) override
        {
            if (cmdlen == 0)
                return;

            int ret;
            int sts =
                rshell_execute(cmdbuf, _cmdtable, &ret, 0, ansbuf, ansmax);

            if (sts)
                snprintf(ansbuf, ansmax, "Undefined command\r\n");
        }
    };

    class rshell_executor_twolevel : public rshell_executor
    {
        const rshell_command_table *_cmdtable;

      public:
        rshell_executor_twolevel(const rshell_command_table *cmdtable)
            : _cmdtable(cmdtable)
        {
        }

        void execute(char *cmdbuf, int cmdlen, char *ansbuf,
                     int ansmax) override
        {
            if (cmdlen == 0)
                return;

            int ret = 0;
            int sts =
                rshell_tables_execute(cmdbuf, _cmdtable, &ret, ansbuf, ansmax);

            if (sts)
                snprintf(ansbuf, ansmax, "Undefined command\r\n");
        }
    };
}

#endif

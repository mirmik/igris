#ifndef IGRIS_CLI_H
#define IGRIS_CLI_H

#include <igris/shell/conscmd.h>

namespace igris 
{
	using cli_handler = int(*)(int,char**);
	using cli_handler_s = int(*)(int,char**,char*,int);
}

#endif
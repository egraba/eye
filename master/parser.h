#ifndef _PARSER
#define _PARSER

#include "info.h"
#include "usage.h"

int parse_info(machine *info, char *data);
int parse_usage(cpu_usage *cpu,
		memory_usage *mem,
		swap_usage *swap,
		char *data);

#endif

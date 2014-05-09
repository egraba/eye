#include "jsonParser.h"

int
parse(const char *jsonString)
{
	yajl_val node;
	char fileBuffer[BUFSIZ];
	char errBuffer[BUFSIZ];

	memset(fileBuffer, 0, sizeof(fileBuffer));
	strncpy(fileBuffer, "{\"osName\":\"FreeBSD\"}", 20);
	node = yajl_tree_parse((const char *) fileBuffer, errBuffer, sizeof(errBuffer));
	if (node == NULL) {
		perror(errBuffer);
		return -1;
	}

	const char *info[] = {"osName"};
	yajl_val v = yajl_tree_get(node, info, yajl_t_string);
	printf("%s\n", YAJL_GET_STRING(v));
	return 0;
}

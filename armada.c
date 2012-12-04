/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
	#error Assertions cannot be disabled. Undefine NDEBUG and try again.
#endif

int main(int argc, char **argv) {
	char name[L_tmpnam + 2];
	assert(tmpnam(name));
	strcat(name, ".c");
	FILE *file = fopen(name, "a+");
	assert(file);
	assert(fputs("#include <stdio.h>\nint main(int argc, char **argv) { ", file) != EOF);
	char text[1024];
	while(fgets(text, sizeof(text), stdin)) fputs(text, file);
	fputs("}\n", file);
	rewind(file);
	pid_t pid = fork();
	assert(pid != -1);
	if(!pid) assert(execlp("c99", "c99", name, 0) != -1);
	int code;
	assert(wait(&code) == pid && !code);
	assert(remove(name) != -1);
	pid = fork();
	assert(pid != -1);
	if(!pid) assert(execvp("./a.out", argv) != -1);
	assert(wait(&code) == pid);
	assert(remove("./a.out") != -1);
	return code;
}

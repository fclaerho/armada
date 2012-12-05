/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void test(_Bool b) {
	if(b) return;
	perror("fatal");
	abort();
}

int main(int argc, char **argv) {
	char name[L_tmpnam + 2];
	test(tmpnam(name));
	(void)strcat(name, ".c");
	FILE *file = fopen(name, "a+");
	test(file);
	test(fputs("#include <stdlib.h>\n#include <stdio.h>\nint main(int argc, char **argv) { ", file) != EOF);
	char text[1024];
	while(fgets(text, sizeof(text), stdin)) test(fputs(text, file) != EOF);
	test(fputs("}\n", file) != EOF);
	rewind(file);
	pid_t pid = fork();
	test(pid != -1);
	if(!pid) test(execlp("c99", "c99", "-O3", name, 0) != -1);
	int stat;
	test(wait(&stat) != -1);
	test(WIFEXITED(stat) && !WEXITSTATUS(stat));
	test(remove(name) != -1);
	pid = fork();
	test(pid != -1);
	if(!pid) test(execvp("./a.out", argv) != -1);
	test(wait(&stat) != -1);
	test(remove("./a.out") != -1);
	return WIFEXITED(stat)? WEXITSTATUS(stat): EXIT_FAILURE;
}

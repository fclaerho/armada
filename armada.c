/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
	#error Assertions cannot be disabled, please undefine NDEBUG.
#endif

char srcname[L_tmpnam + 2], objname[L_tmpnam];

void rmsrc(void) { (void)remove(srcname); }

void rmobj(void) { (void)remove(objname); }

int main(int argc, char **argv) {
	assert(tmpnam(srcname));
	(void)strcat(srcname, ".c");
	FILE *srcfile = fopen(srcname, "a+");
	assert(srcfile);
	atexit(rmsrc);
	assert(setenv("ARMADA_INCLUDE", "#include <stdlib.h>\n#include <stdio.h>\n", 0) != -1);
	assert(fputs(getenv("ARMADA_INCLUDE"), srcfile) != EOF);
	assert(fputs("int main(int argc, char **argv) { ", srcfile) != EOF);
	char text[1024];
	while(fgets(text, sizeof(text), stdin)) assert(fputs(text, srcfile) != EOF);
	assert(fputs("}\n", srcfile) != EOF);
	rewind(srcfile);
	assert(tmpnam(objname));
	pid_t pid = fork();
	assert(pid != -1);
	if(!pid) assert(execlp("c99", "c99", "-o", objname, "-O3", srcname, 0) != -1);
	int stat;
	assert(wait(&stat) != -1);
	assert(WIFEXITED(stat) && !WEXITSTATUS(stat));
	atexit(rmobj);
	pid = fork();
	assert(pid != -1);
	if(!pid) assert(execvp(objname, argv) != -1);
	assert(wait(&stat) != -1);
	if(WIFSIGNALED(stat)) raise(WTERMSIG(stat));
	assert(WIFEXITED(stat));
	return WEXITSTATUS(stat);
}

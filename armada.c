/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char name[L_tmpnam + 2];

void rmsrc(void) { (void)remove(name); }

void rmobj(void) { (void)remove("a.out"); }

const char builtins[] =
	"#include <assert.h>\n"
	"#include <string.h>\n"
	"#include <stdarg.h>\n"
	"#include <stdlib.h>\n"
	"#include <stdio.h>\n"
	"#include <dirent.h>\n"
	"#include <libgen.h>\n"
	"typedef const struct ite {"
	"	struct ite *next;"
	"	char *val;"
	"} ite;"
	"struct ite* push(struct ite *i, const char *s) {"
	"	struct ite *j;"
	"	for(j = i; j && j->next; j = j->next);"
	"	struct ite *k = malloc(sizeof(ite));"
	"	assert(k);"
	"	if(j) j->next = k;"
	"	k->val = strdup(s);"
	"	assert(k->val);"
	"	return i? i: k;"
	"}"
	"ite* mkite(char **argv) {\n"
	"	struct ite *i = 0;\n"
	"	for(; *argv; ++argv) i = push(i, *argv);\n"
	"	return i;\n"
	"}\n"
	"ite* mkitev(char *s, ...) {"
	"	struct ite *i = 0;"
	"	va_list list;"
	"	va_start(list, s);"
	"	for(; s; s = va_arg(list, char*)) i = push(i, s);"
	"	va_end(list);"
	"	return i;"
	"}\n"
	"ite* shift(ite *i) {"
	"	ite *j = i? i->next: 0;"
	"	if(i) free((void*)i);"
	"	return j;"
	"}"
	"ite* split(const char *s, const char *sep) {"
	"	char *cpy = strdup(s);"
	"	assert(cpy);"
	"	char *cur = cpy;"
	"	struct ite *i = 0;"
	"	while(cur) i = push(i, strsep(&cur, sep));"
	"	free(cpy);"
	"	return i;"
	"}"
	"char* join(const char *sep, ite *i) {"
	"	char *buf = 0;"
	"	for(; i; i = i->next) {"
	"		buf =  realloc(buf, (buf? strlen(buf): 0) + strlen(i->val) + strlen(sep) + 1);"
	"		strcat(buf, i->val);"
	"		if(i->next) strcat(buf, sep);"
	"	}"
	"	return buf;"
	"}"
	"size_t count(ite *i) {"
	"	size_t c = 0;"
	"	for(; i; i = i->next) ++c;"
	"	return c;"
	"}"
	"_Bool match(char *s, char *t) { /* IOCCC 2001 schweikh */"
	"	return *t-'*'?*s?'?'==*t|*s==*t&&match(s+1,t+1):!*t:match(s,t+1)||*s&&match(s+1,t);"
	"}\n"
	"ite* glob(char *pattern) {\n"
	"	DIR *dir = opendir(dirname(pattern));"
	"	struct ite *i = 0;"
	"	for(struct dirent *e = readdir(dir); e; e = readdir(dir)) {"
	"		if(strcmp(e->d_name, \".\")"
	"		&& strcmp(e->d_name, \"..\")"
	"		&& match(e->d_name, basename(pattern))) i = push(i, e->d_name);"
	"	}"
	"	return i;"
	"}\n"
	"ite* slice(ite *i, size_t min, size_t max) {\n"
	"	struct ite *j = 0;\n"
	"	for(size_t cur = 0; i; i = i->next) if(min <= cur && cur<=max) j = push(j, i->val);\n"
	"	return j;\n"
	"}\n"
	"void echo(ite *i) {\n"
	"	fputs(\"(\", stdout);\n"
	"	for(; i; i = i->next) fprintf(stdout, \"%s%s\", i->val, i->next? \", \": \"\");\n"
	"	fputs(\")\\n\", stdout);\n"
	"}\n";

int main(int argc, char **argv) {
	assert(tmpnam(name));
	(void)strcat(name, ".c");
	FILE *file = fopen(name, "a+");
	assert(file);
	atexit(rmsrc);
	assert(fputs(builtins, file) != EOF);
	assert(fputs("int main(int argc, char **argv) { ", file) != EOF);
	char text[1024];
	while(fgets(text, sizeof(text), stdin)) assert(fputs(text, file) != EOF);
	assert(fputs("}\n", file) != EOF);
	rewind(file);
	pid_t pid = fork();
	assert(pid != -1);
	if(!pid) assert(execlp("c99", "c99", "-O3", name, 0) != -1);
	int stat;
	assert(wait(&stat) != -1);
	assert(WIFEXITED(stat) && !WEXITSTATUS(stat));
	atexit(rmobj);
	pid = fork();
	assert(pid != -1);
	if(!pid) assert(execvp("./a.out", argv) != -1);
	assert(wait(&stat) != -1);
	if(WIFSIGNALED(stat)) raise(WTERMSIG(stat));
	assert(WIFEXITED(stat));
	return WEXITSTATUS(stat);
}

/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void test(_Bool b) {
	if(b) {
		errno = 0;
		return;
	}
	perror("fatal");
	exit(EXIT_FAILURE);
}

char name[L_tmpnam + 2];

void rmsrc(void) { (void)remove(name); }

void rmobj(void) { (void)remove("a.out"); }

const char builtins[] =
	"#include <string.h>\n"
	"#include <stdarg.h>\n"
	"#include <stdlib.h>\n"
	"#include <stdio.h>\n"
	"#include <dirent.h>\n"
	"#include <libgen.h>\n"
	"void test(_Bool b) {"
	"	if(b) return;"
	"	perror(0);"
	"	exit(EXIT_FAILURE);"
	"}"
	"char* str(const char *fmt, ...) {"
	"	va_list lst, cpy;"
	"	va_start(lst, fmt);"
	"	va_copy(cpy, lst);"
	"	size_t len = vsnprintf(0, 0, fmt, lst);"
	"	va_end(lst);"
	"	char *buf = malloc(len + 1);"
	"	(void)vsnprintf(buf, len + 1, fmt, cpy);"
	"	va_end(cpy);"
	"	return buf;"
	"}"
	"typedef const struct ite {"
	"	struct ite *next;"
	"	char *val;"
	"} ite;"
	"struct ite* push(struct ite *i, const char *s) {"
	"	struct ite *j;"
	"	for(j = i; j && j->next; j = j->next);"
	"	struct ite *k = malloc(sizeof(ite));"
	"	test(k);"
	"	if(j) j->next = k;"
	"	k->val = strdup(s);"
	"	test(k->val);"
	"	strcpy(k->val, s);"
	"	return i? i: k;"
	"}"
	"ite* shift(ite *i) {"
	"	ite *j = i? i->next: 0;"
	"	if(i) free((void*)i);"
	"	return j;"
	"}"
	"ite* split(const char *s, const char *sep) {"
	"	char *cpy = strdup(s), *cur = cpy;"
	"	struct ite *i = 0;"
	"	while(cur) i = push(i, strsep(&cur, sep));"
	"	free(cpy);"
	"	return i;"
	"}"
	"char* join(const char *sep, ite *i) {"
	"	char *buf = strdup(\"\");"
	"	for(; i; i = i->next) {"
	"		char *tmp = str(\"%s%s%s\", buf, i->val, i->next? sep: \"\");"
	"		free(buf);"
	"		buf = tmp;"
	"	}"
	"	return buf;"
	"}"
	"size_t count(ite *i) {"
	"	size_t c = 0;"
	"	for(; i; i = i->next) ++c;"
	"	return c;"
	"}"
	"_Bool match(char *s, char *pattern) {\n"
	"	return 1;\n"
	"}\n"
	"ite* glob(char *pattern) {\n"
	"	DIR *dir = opendir(dirname(pattern));\n"
	"	struct ite *i = 0;\n"
	"	for(struct dirent *e = readdir(dir); e; e = readdir(dir)) {\n"
	"		if(strcmp(e->d_name, \".\")\n"
	"		&& strcmp(e->d_name, \"..\")\n"
	"		&& match(e->d_name, basename(pattern))) i = push(i, e->d_name);\n"
	"	}\n"
	"	return i;\n"
	"}\n"
	"ite* mkite(char **argv) {\n"
	"	struct ite *i = 0;\n"
	"	for(; *argv; ++argv) i = push(i, *argv);\n"
	"	return i;\n"
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
	test(tmpnam(name));
	(void)strcat(name, ".c");
	FILE *file = fopen(name, "a+");
	test(file);
	atexit(rmsrc); /* comment out this line to keep the source file */
	test(fputs(builtins, file) != EOF); /* comment out this line if you don't want the builtins */
	test(fputs("int main(int argc, char **argv) { ", file) != EOF);
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
	atexit(rmobj);
	pid = fork();
	test(pid != -1);
	if(!pid) test(execvp("./a.out", argv) != -1);
	test(wait(&stat) != -1);
	test(WIFEXITED(stat));
	return WEXITSTATUS(stat);
}


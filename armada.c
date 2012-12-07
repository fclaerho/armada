/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void test(_Bool b) {
	if(b) return;
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
	"void test(_Bool b) {\n"
	"	if(b) return;\n"
	"	perror(0);\n"
	"	exit(EXIT_FAILURE);\n"
	"}\n"
	"typedef const struct ite {\n"
	"	struct ite *next;\n"
	"	char *val;\n"
	"} ite;\n"
	"struct ite* append(struct ite *i, const char *s) {\n"
	"	struct ite *j;\n"
	"	for(j = i; j && j->next; j = j->next);\n"
	"	struct ite *k = malloc(sizeof(ite) + strlen(s) + 1);\n"
	"	test(k);\n"
	"	if(j) j->next = k;\n"
	"	k->val = (char*)(k + 1);\n"
	"	strcpy(k->val, s);\n"
	"	return i? i: k;"
	"}\n"
	"ite* str(const char *fmt, ...) {\n"
	"	va_list lst, cpy;\n"
	"	va_start(lst, fmt);\n"
	"	va_copy(cpy, lst);\n"
	"	size_t len = vsnprintf(0, 0, fmt, lst);\n"
	"	va_end(lst);\n"
	"	char buf[len + 1];\n"
	"	(void)vsnprintf(buf, len + 1, fmt, cpy);\n"
	"	va_end(cpy);\n"
	"	return append(0, buf);"
	"}\n"
	"ite* glob(char *pattern) {\n"
	"	DIR *dir = opendir(basename(pattern));\n"
	"	struct ite *i = 0;\n"
	"	for(struct dirent *e = readdir(dir); e; e = readdir(dir)) {\n"
	"		if(strcmp(e->d_name, \".\") && strcmp(e->d_name, \"..\")) i = append(i, e->d_name);\n"
	"	}\n"
	"	return i;\n"
	"}\n"
	"ite* next(ite *i) {\n"
	"	ite *j = i? i->next: 0;\n"
	"	if(i) free((void*)i);\n"
	"	return j;\n"
	"}\n"
	"ite* mkite(char **argv) {\n"
	"	struct ite *i = 0;\n"
	"	for(; *argv; ++argv) i = append(i, *argv);\n"
	"	return i;\n"
	"}\n"
	"ite* slice(ite *i, size_t min, size_t max) {\n"
	"	struct ite *j = 0;\n"
	"	for(size_t cur = 0; i; i = i->next) if(min <= cur && cur<=max) j = append(j, i->val);\n"
	"	return j;\n"
	"}\n"
	"char* index(ite *i, size_t at) {\n"
	"	while(i && at--) i = i->next;\n"
	"	return i->val;\n"
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


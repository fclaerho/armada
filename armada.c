/*                         _      
 *  __ _ _ _ _ __  __ _ __| |__ _ 
 * / _` | '_| '  \/ _` / _` / _` |
 * \__,_|_| |_|_|_\__,_\__,_\__,_|
 * Your C99 one-liner.
 * Copyright (c) 2012 f.claerhout, licensed under the GPL.
 */

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
	#error Assertions cannot be disabled, please undefine NDEBUG.
#endif

char srcname[L_tmpnam + 2], objname[L_tmpnam];

void rmsrc(void) { (void)remove(srcname); }

void rmobj(void) { (void)remove(objname); }

int shell(const char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	size_t len = vsnprintf(0, 0, fmt, list);
	char *buf = malloc(len + 1);
	assert(buf);
	va_start(list, fmt);
	assert(vsnprintf(buf, len + 1, fmt, list) == len);
	va_end(list);
	int code = system(buf);
	free(buf);
	assert(code != -1 && code != 127);
	return code;
}

int main(int argc, char **argv) {
	assert(tmpnam(srcname));
	(void)strcat(srcname, ".c");
	FILE *srcfile = fopen(srcname, "a+");
	assert(srcfile);
	atexit(rmsrc);
	assert(setenv("ARMADA_INCLUDE", "#include <string.h>\n#include <stdlib.h>\n#include <stdio.h>\n#include <math.h>\n", 0) != -1);
	assert(fputs(getenv("ARMADA_INCLUDE"), srcfile) != EOF);
	assert(fputs("int main(int argc, char **argv) { ", srcfile) != EOF);
	char text[1024];
	while(fgets(text, sizeof(text), stdin)) assert(fputs(text, srcfile) != EOF);
	assert(fputs("}\n", srcfile) != EOF);
	rewind(srcfile);
	assert(tmpnam(objname));
	assert(setenv("ARMADA_CFLAGS", "-O3", 0) != -1);
	assert(!shell("c99 %s -o %s %s", getenv("ARMADA_CFLAGS"), objname, srcname));
	atexit(rmobj);
	size_t len = 0;
	for(size_t i = 1; i < argc; ++i) len += strlen(argv[i]);
	char args[len + 1];
	args[0] = 0;
	for(size_t i = 1; i < argc; ++i) strcat(args, argv[i]);
	return shell("%s %s", objname, args);	
}

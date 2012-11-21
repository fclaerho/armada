/* Copyright (c) 2012 f.claerhout, licensed under the GPL */

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* cc[] = {"cc", "-x", "c", "-std=c99", "-c", "-", 0};
char* nl[] = {"nl", 0};

void run(char* cmd[], FILE *source) {
	pid_t pid = fork();
	switch(pid) {
		case -1:
			perror("fork() failed");
			abort();
		case 0:
			if(dup2(fileno(source), STDIN_FILENO) == -1) { // set source as stdin
				perror("dup2() failed");
				abort();
			}
			rewind(source);
			execvp(cmd[0], cmd);
			perror("execvp() failed");
			abort();
		default:
			(void)waitpid(pid, 0, 0);
	}
}

void history(void) {
	fprintf(stderr, "base=%i, current=%i, total=%i\n", history_base, where_history(), history_length);
	//for(HIST_ENTRY *entry = current_history(); entry; entry = next_history()) {
	for(int i = 0; i < history_length; ++i) {
		HIST_ENTRY *entry = history_get(history_base - 1 + i);
		if(entry) fprintf(stderr, "%i: %s\n", i, entry->line);
		else fprintf(stderr, "%i: no entry\n", i);
	}
}

int main(void) {
	const char *prompt = "armada> ";
	FILE *source = tmpfile();
	setvbuf(source, 0, _IONBF, 0); /* disable buffering */
	for(;;) {
		char *line = readline(prompt);
		assert(line);
		if(!strcmp(line, "quit")) break;
		add_history(line);
		history();
		if(!strcmp(line, "list")) run(nl, source);
		else if(!strcmp(line, "history")) history();
		else if(!strcmp(line, "droptop")) remove_history(0);
		else {
			fprintf(source, "%s\n", line);
			//run(cc, source);
		}
		//free(line);
	}
}

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define NULL ((char*)0)

char* getline() {
	char c, buf[512];
	int pos = 0;
	while (read(0, &c, sizeof(char))) {
		if (c == '\n') break;
		else buf[pos++] = c;
	}
	if (pos == 0) return NULL;
	char *res = (char*)malloc(sizeof(char) * pos);
	memmove(res, buf, pos);
	return res;
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(2, "Usage: [xgargs cmd args...]\n");
		exit(1);
	}
	char *line;
	char *argv1[MAXARG];
	for (int i = 0; i < argc - 1; i++){
		argv1[i] = argv[i + 1];
	}
	while ((line = getline()) != NULL){
		argv1[argc - 1] = line;
		if (fork() == 0){
			exec(argv1[0], argv1);
			free(line);
			exit(0);
		}
		else wait(0);
	}
	exit(0);
}
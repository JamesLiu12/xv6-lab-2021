#include "kernel/types.h"
#include "user/user.h"

void primes(int *pl){
	int num, pr[2];
	pipe(pr);
	if (read(pl[0], &num, sizeof(int))) fprintf(1, "prime %d\n", num);
	else exit(0);
	if (fork() == 0){
		close(pr[1]);
		primes(pr);
		close(pr[0]);
	}
	else {
		int nxt;
		close(pl[1]);
		close(pr[0]);
		while (read(pl[0], &nxt, sizeof(int))){
			if (nxt % num) write(pr[1], &nxt, sizeof(int));
		}
		close(pl[0]);
		close(pr[1]);
		wait(0);
		exit(0);
	}
}

int main() {
	int p[2];
	pipe(p);
	if (fork() == 0) {
		close(p[1]);
		primes(p);
		close(p[0]);
	}
	else{
		close(p[0]);
		for (int i = 2; i <= 35; i++) write(p[1], &i, sizeof(int));
		close(p[1]);
		wait(0);
	}
	exit(0);
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int cmp(char *path, char *file) {
	char *p;
	for (p = path + strlen(path); p >= path && *p != '/'; p--);
	return strcmp(++p, file);
}

void find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (cmp(path, file) == 0) fprintf(1, "%s\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
	  if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
      p[DIRSIZ] = 0;
	  find(buf, file);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(2, "Usage: [path, file]");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}

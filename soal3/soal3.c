#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main() {
  int link[2];
  pid_t pid;
  char foo[4096];
  int status;
  int link2[2];
  if (pipe(link2)==-1)
    die("pipe");

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {

    execl("/usr/bin/unzip", "unzip", "/home/syauqi/modul2/campur2.zip", (char *)0);
    die("execl");

  } else {
    while((wait(&status))>0);
    int statu;

    if ((pid = fork()) == -1)
    die("fork");

    if (pid == 0) {
      dup2 (link[1], STDOUT_FILENO);
      close(link[0]);
      close(link[1]);
      execl("/bin/ls", "ls", "/home/syauqi/modul2/campur2/", (char *)0);
      die("execl");
    }

    else {
      if ((pid = fork()) == -1)
        die("fork");

      if (pid == 0) {
        dup2 (link[0], STDIN_FILENO);
        dup2 (link2[1], STDOUT_FILENO);
        close(link[1]);
        close(link[0]);
	close(link2[1]);
        close(link2[0]);
        execl("/bin/grep", "grep", ".txt$", (char *)0);
        die("execl");
      }

      else{
        FILE *f = fopen("daftar.txt", "w");
        close(link2[1]);
	close(link[1]);
	close(link[0]);
        int nbytes = read(link2[0], foo, sizeof(foo));
        fprintf(f, "%.*s\n", nbytes, foo);
        fclose(f);
      }
    }
  }
  return 0;
}

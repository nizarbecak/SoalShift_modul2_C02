#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main(int argc, char **argv) {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/syauqi/modul2/hatiku/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    char mode[] = "0777";
    char buf[100] = "elen.ku";
    int i;
    i = strtol(mode, 0, 8);
    chmod (buf,i);

    struct stat info;
    stat(buf, &info);
    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);

    if(strcmp(pw->pw_name, "www-data")==0 && strcmp(gr->gr_name, "www-data")==0){
      int status;
      status = remove(buf);
    }
    
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}

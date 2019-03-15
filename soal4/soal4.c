#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

static time_t getFileAccessedTime(const char *path)
{
    struct stat attr;
    if (stat(path, &attr) == 0)
    {
        return attr.st_atime;
    }
    return 0;
}

int i = 1;

int main() {
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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    time_t t1 = getFileAccessedTime("/home/becak/Documents/makanan/makan_enak.txt");
    time_t t2 = time(NULL);
    char filename [FILENAME_MAX];

    if (t2 - t1 <= 30) {
        sprintf(filename, "/home/becak/Documents/makanan/makan_sehat%d.txt", i);
        FILE *new_file;
        new_file = fopen(filename,"w");
        fclose(new_file);
        i++;
    }

    sleep(5);
  }

  exit(EXIT_SUCCESS);
}

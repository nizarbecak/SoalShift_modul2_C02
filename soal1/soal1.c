#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

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

  if ((chdir("/home/syauqi/modul2/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    struct dirent *de;

    DIR *dr = opendir(".");

    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL)
    {
      char stre[256];
      char newpath[256];
      int i=0;
      snprintf(stre, 256, "%s", de->d_name);
      if(strstr(stre, ".png"))
      {
	for(i=0; i < sizeof(de->d_name); i++)
	  if(stre[i]=='.') break;

	stre[i]='\0';
	snprintf(newpath, 256, "gambar/%s_grey.png", stre);
	rename(de->d_name, newpath);
      }
    }

    closedir(dr);
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}

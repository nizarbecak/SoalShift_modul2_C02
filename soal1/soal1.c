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
      if(strstr(de->d_name, ".png"))
      {
	int len = strlen(de->d_name)-4;
	char newname[len+9];
	strncpy(newname,de->d_name,len-1);
	newname[len-1]='\0';
	strcat(newname, "_grey.png");
	char newpath[100]="/home/syauqi/modul2/gambar/";
	strcat(newpath, newname);
	printf("%s\n", newpath);
	rename(de->d_name, newpath);
      }
    }

    closedir(dr);
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}

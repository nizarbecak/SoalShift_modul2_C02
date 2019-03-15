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
    char filename[FILENAME_MAX];
    char directory[FILENAME_MAX];
    char appendlog[100];
    char text[999999];
    FILE *syslog, *loglog;
    int i = 30;

    if (i == 30){
    	struct tm *timenow;
    	time_t now = time(NULL);
    	timenow = gmtime(&now);
    	strftime(directory, sizeof(directory), "/home/becak/log/%d:%m:%Y-%H:%M", timenow);
    	mkdir(directory, ACCESSPERMS);
    	i = 0;
    }

    sprintf(appendlog, "/log%d.log", i);
    strcpy(filename, directory);
    strcat(filename, appendlog);
    syslog = fopen("/var/log/syslog","r");
    loglog = fopen(filename, "w");
    while((fgets(text, sizeof(text), syslog)) != NULL) {
    	fputs(text, loglog);
    }

    fclose(syslog);
    fclose(loglog);
    i++;

    sleep(60);
  }

  exit(EXIT_SUCCESS);
}


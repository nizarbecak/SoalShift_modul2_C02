#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main()
{
  FILE *pidnya;
  pidnya = fopen("/home/becak/modul2/pid_nomor5.txt", "r");
  int pid_kill;

  fscanf(pidnya, "%d", &pid_kill);

  kill(pid_kill, SIGKILL);

  fclose(pidnya);
  return 0;
}

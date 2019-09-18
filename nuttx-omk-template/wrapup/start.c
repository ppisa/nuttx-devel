#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
  syslog(LOG_INFO, "start application called\n");
  return 0;
}

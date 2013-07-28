#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERROR(_msg) \
  do { perror(_msg); exit(EXIT_FAILURE); } while(0)

int main() {
  int fd, err;
  char buf[sizeof(int)];
  printf("Attempting to open file\n");
  fd = open("/dev/mersenne", O_RDONLY);
  if (fd < 0) {
    ERROR("device open");
  }
  printf("Attempting to read an integer\n");
  err = read(fd, &buf, sizeof(int));
  if (err < 0) {
    ERROR("device read");
  }
  printf("success! num=%d\n", *((int*)&buf));
  exit(EXIT_SUCCESS);
}

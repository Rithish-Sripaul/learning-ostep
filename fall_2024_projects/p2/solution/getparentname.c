#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(int argc, char *argv[]) {
  char parentbuf[256];
  char childbuf[256];

  if (getparentname(parentbuf, childbuf, 256, 256) < 0) {
    printf(2, "XV6_TEST_ERROR getparentname call failed!\n");
  }
  printf(1, "XV6_TEST_OUTPUT Parent name: %s, Child name: %s\n", parentbuf,
         childbuf);
  exit();
}

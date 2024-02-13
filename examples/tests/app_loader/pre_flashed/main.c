
#include <stdio.h>

#include <tock.h>
#include <app_loader.h>


int main(void) {
  printf("Simple test app to load a pre-flashed app\n");


  int ret;
  ret = app_loader_command_setup(8192);
  printf("loaded with err %i\n", ret);

  return 0;
}

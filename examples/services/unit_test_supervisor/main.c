#include <tock.h>
#include <unit_test.h>

int main(void) {
  unit_test_service();

  while (1) {
    yield();
  }
}

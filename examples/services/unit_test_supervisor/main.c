#include <libtock-sync/services/unit_test.h>
#include <libtock/tock.h>

int main(void) {
  unit_test_service();

  while (1) {
    yield();
  }
}

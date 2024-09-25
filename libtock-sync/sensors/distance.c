#include "distance.h"

struct data {
  bool fired;
  int dist;
  returncode_t result;
};

static struct data result = { .fired = false };

static void dist_cb(returncode_t ret, int distance) {
  result.dist   = distance;
  result.fired  = true;
  result.result = ret;
}

returncode_t libtocksync_distance_read(int* distance) {
  returncode_t err;
  result.fired = false;

  err = libtock_distance_read(dist_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.result != RETURNCODE_SUCCESS) return result.result;

  *distance = result.dist;

  return RETURNCODE_SUCCESS;
}

// Function to get minimum distance
int libtocksync_distance_get_minimum_distance(void) {
  return libtock_distance_get_minimum_distance();
}

// Function to get maximum distance
int libtocksync_distance_get_maximum_distance(void) {
  return libtock_distance_get_maximum_distance();
}

#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>

void main() {
  const char kMessage[] = "poopy & boba & ";
  const uint8_t kMessageLen = sizeof(kMessage) - 1;
  uint8_t i = 0;

  while (1) {
    // Slow down without a hot loop by waiting for two vertical blank
    // interrupts.
    for (uint8_t i = 0; i < 4; i++)
      wait_vbl_done();

    if (i < kMessageLen) {
      printf("%c", kMessage[i]);
      i++;
    } else {
      i = 0;
    }
  }
}

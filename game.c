#include <gb/drawing.h>
#include <gb/gb.h>
#include <gbdk/platform.h>
#include <stdint.h>
#include <stdio.h>

const uint8_t kBallWidth = 16;
const uint8_t kBallHeight = 16;

typedef struct ball {
  uint8_t x;
  uint8_t y;
  int8_t vx;
  int8_t vy;
} ball_t;

ball_t ball_new() {
  ball_t ball;
  ball.x = GRAPHICS_WIDTH / 2;
  ball.y = GRAPHICS_HEIGHT / 2;
  ball.vx = 3;
  ball.vy = 1;
  return ball;
}

ball_t ball_tick(ball_t ball, uint8_t *bounce_count) {
  if (ball.x > GRAPHICS_WIDTH - kBallWidth - ball.vx) {
    ball.x = GRAPHICS_WIDTH - kBallWidth;
    ball.vx *= -1;
    (*bounce_count)++;
  } else if (ball.x < -1 * ball.vx) {
    ball.x = 0;
    ball.vx *= -1;
    (*bounce_count)++;
  } else {
    ball.x += ball.vx;
  }

  if (ball.y > GRAPHICS_HEIGHT - kBallHeight - ball.vy) {
    ball.y = GRAPHICS_HEIGHT - kBallHeight;
    ball.vy *= -1;
    (*bounce_count)++;
  } else if (ball.y < -1 * ball.vy) {
    ball.y = 0;
    ball.vy *= -1;
    (*bounce_count)++;
  } else {
    ball.y += ball.vy;
  }

  return ball;
}

void ball_draw(ball_t ball) {
  box(ball.x, ball.y, ball.x + kBallWidth, ball.y + kBallHeight, M_NOFILL);
}

void main() {
  ball_t ball;
  ball = ball_new();

  uint8_t bounce_count = 0;

  while (1) {
    // Slow down without a hot loop by waiting for a vertical blank interrupt.
    wait_vbl_done();

    ball = ball_tick(ball, &bounce_count);

    color(DKGREY, LTGREY, XOR);
    ball_draw(ball);

    color(BLACK, WHITE, XOR);
    gotogxy(1, 1);
    gprintf("BOUNCE COUNT: %u", (int)bounce_count);
  }
}

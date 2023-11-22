#include <gb/gb.h>
#include <gbdk/platform.h>
#include <stdbool.h>
#include <stdint.h>

const uint8_t kBallWidth = 8;
const uint8_t kBallHeight = 8;
const uint8_t kUpDeltaV = 3;
const uint8_t kMaxSpeed = 10;

typedef struct coord {
  uint8_t pos;
  uint8_t speed;
  bool negative;
} coord_t;

const coord_t kDefaultCoord = {0, 0, false};

coord_t coord_tick(coord_t coord) {
  if (coord.negative) {
    coord.pos -= coord.speed;
  } else {
    coord.pos += coord.speed;
  }
  return coord;
}

bool coord_would_tick_oob(coord_t coord, uint8_t lo, uint8_t hi) {
  coord = coord_tick(coord);
  return lo > coord.pos || coord.pos > hi;
}

typedef struct ball {
  coord_t x;
  coord_t y;
} ball_t;

ball_t ball_new() {
  ball_t ball;
  ball.x = kDefaultCoord;
  ball.y = kDefaultCoord;
  return ball;
}

ball_t ball_tick(ball_t ball) {
  // Add gravity. This computation is complicated by the fact that I am using
  // uint8_t speeds with separate sign bits.
  if (ball.y.speed == 0) {
    // If we're at the top of an arc with zero speed, start falling.
    ball.y.negative = false;
    ball.y.speed = 1;
  } else if (!ball.y.negative) {
    // Increase speed if we're already falling and we haven't reached terminal
    // velocity.
    if (ball.y.speed < kMaxSpeed) {
      ball.y.speed++;
    }
  } else if (ball.y.speed > 0) {
    // If we're moving upward with positive speed, reduce speed.
    ball.y.speed--;
  }

  // If ticking any coordinate forward in time would go out of bounds, negate
  // that coordinate's velocity.
  if (coord_would_tick_oob(ball.x, 0, SCREENWIDTH)) {
    ball.x.negative = !ball.x.negative;
  }
  if (coord_would_tick_oob(ball.y, 0, SCREENHEIGHT)) {
    ball.y.negative = !ball.y.negative;
  }

  ball.x = coord_tick(ball.x);
  ball.y = coord_tick(ball.y);

  return ball;
}

void ball_draw(ball_t ball) { move_sprite(0, ball.x.pos, ball.y.pos); }

// The tile is 8x8 pixels with two bits per pixel. Surprisingly, the two bits
// that make up a pixel are not sequential within a byte; instead, the pixel's
// value is composed of the ith bit from bytes n and n+1 (for even values of n).
const uint8_t kSpriteTile[16] = {
    0b11111111, //
    0b11111111, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b10000001, //
    0b11111111, //
    0b11111111, //
};

int main() {
  disable_interrupts();
  DISPLAY_OFF;
  LCDC_REG = LCDCF_OFF | LCDCF_WIN9C00 | LCDCF_WINON | LCDCF_BG8800 |
             LCDCF_BG9800 | LCDCF_OBJ16 | LCDCF_OBJON | LCDCF_BGON;
  BGP_REG = OBP0_REG = OBP1_REG = 0xE4U;

  set_sprite_data(0, 1, kSpriteTile);
  set_sprite_prop(0, 0x00);
  set_sprite_tile(0, 0x00);

  ball_t ball;
  ball = ball_new();
  ball.x.speed = 1;

  DISPLAY_ON;
  enable_interrupts();

  while (true) {
    // Slow down without a hot loop by waiting for a vertical blank interrupt.
    wait_vbl_done();

    uint8_t buttons = joypad();

    // Pause the game as long as the select button is pressed down.
    if (buttons & J_SELECT) {
      waitpadup();
    }

    if (buttons & J_UP) {
      if (ball.y.negative && ball.y.speed <= kMaxSpeed - kUpDeltaV) {
        ball.y.speed += kUpDeltaV;
      } else if (!ball.y.negative && ball.y.speed >= kUpDeltaV) {
        ball.y.speed -= kUpDeltaV;
      } else if (ball.y.speed == 0) {
        ball.y.negative = true;
        ball.y.speed = 1;
      }
    }
    if (buttons & J_RIGHT) {
      if (!ball.x.negative && ball.x.speed < kMaxSpeed) {
        ball.x.speed++;
      } else if (ball.x.negative && ball.x.speed > 0) {
        ball.x.speed--;
      } else if (ball.x.speed == 0) {
        ball.x.negative = false;
        ball.x.speed = 1;
      }
    }
    if (buttons & J_LEFT) {
      if (ball.x.negative && ball.x.speed < kMaxSpeed) {
        ball.x.speed++;
      } else if (!ball.x.negative && ball.x.speed > 0) {
        ball.x.speed--;
      } else if (ball.x.speed == 0) {
        ball.x.negative = true;
        ball.x.speed = 1;
      }
    }

    ball = ball_tick(ball);
    ball_draw(ball);
  }
}

#ifndef __GAME_H__
#define __GAME_H__

#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "../lib/include/st7735.h"
#include "../lib/include/st7735_gfx.h"
#include "../lib/include/st7735_font.h"
#include "../lib/include/spi.h"

#include "port.h"

// #define DEBUG
#include "../include/debug.h"

#define BALL_SIZE 9 // square 3x3
#define RACKET_SIZE 40 // rectangle 20x2

#define WIN_POINTS 3

enum {
    NO_COLLISION,
    V_COLLISION,
    H_COLLISION,
    // V_AND_H_COLLISION,
};

typedef struct {
    uint8_t player_1;
    uint8_t player_2;
} score_t;

typedef struct {
    int16_t x;
    int16_t y;
    int8_t  dx;
    int8_t  dy;
} pixel_t;

typedef pixel_t ball_t[BALL_SIZE];

typedef pixel_t racket_t[RACKET_SIZE];


void game_init(ball_t ball, racket_t racket_1, racket_t racket_2, score_t *score);

void game_renderObject(pixel_t object[], uint8_t size);
void game_eraseObject(pixel_t object[], uint8_t size);

void game_copyObject(const pixel_t src[], pixel_t dst[], uint8_t size);
uint8_t game_ball_racketCollision(ball_t ball, racket_t racket);
uint8_t game_ball_boundaryCollision(ball_t ball);
void game_ball_setSpeed(ball_t ball, int8_t dx, int8_t dy);
void game_ball_move(ball_t ball);

void game_racket_move(racket_t racket, int8_t dy);

void game_handleGoal(ball_t ball, racket_t racket1, racket_t racket2, score_t *score);

bool game_controller_1_isLeftBtnClicked();
bool game_controller_1_isRightBtnClicked();
bool game_controller_2_isLeftBtnClicked();
bool game_controller_2_isRightBtnClicked();

void game_renderScore(score_t *score);

void game_init_LEDs();

#endif
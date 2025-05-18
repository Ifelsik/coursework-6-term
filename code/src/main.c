#include "../include/main.h"

// #define F_CPU 16000000UL
#define BAUD 9600U


int main() {
    // disable in product mode 
    // in PCB uart's pins are used as GPIO
    // uart_init(F_CPU, BAUD);

    spi_init();
    DEBUG_PRINT("SPI initialized\r\n");
    st7735_init();
    DEBUG_PRINT("ST7735 initialized\r\n");

    st7735_set_orientation(ST7735_LANDSCAPE);
    DEBUG_PRINT("display orientation set LANDSCAPE\r\n");
    st7735_fill_rect(0, 0, 160, 128, ST7735_COLOR_BLACK);

    // st7735_draw_mono_bitmap(16, 4, (PGM_P) logo_bw, ST7735_COLOR_WHITE, ST7735_COLOR_BLACK);
    // DEBUG_PRINT("img loaded\r\n");
    _delay_ms(500);

    ball_t ball_current;
    racket_t racket_1, racket_2;
    score_t score;

    game_init(ball_current, racket_1, racket_2, &score);
    DEBUG_PRINT("game initialized\r\n");

    bool btn_1_left_clicked = false;
    bool btn_1_right_clicked = false;
    bool btn_2_left_clicked = false;
    bool btn_2_right_clicked = false;
    uint8_t collision_type;

    while(1) {
        collision_type = NO_COLLISION;

        game_renderObject(ball_current, BALL_SIZE);
        game_renderObject(racket_1, RACKET_SIZE);
        game_renderObject(racket_2, RACKET_SIZE);

        btn_1_left_clicked = game_controller_1_isLeftBtnClicked();
        btn_1_right_clicked = game_controller_1_isRightBtnClicked();
        btn_2_left_clicked = game_controller_2_isLeftBtnClicked();
        btn_2_right_clicked = game_controller_2_isRightBtnClicked();
     
        ball_t ball_next;
        game_copyObject(ball_current, ball_next, BALL_SIZE);
        game_ball_move(ball_next);  // virtual movement to detect collision


        collision_type |= game_ball_boundaryCollision(ball_next);
        collision_type |= game_ball_racketCollision(ball_next, racket_1);
        collision_type |= game_ball_racketCollision(ball_next, racket_2);
        if (collision_type & V_COLLISION) {
            game_ball_setSpeed(ball_current, -ball_current[0].dx, ball_current[0].dy);
        }
        if (collision_type & H_COLLISION) {
            game_ball_setSpeed(ball_current, ball_current[0].dx, -ball_current[0].dy);
        }

        int8_t racket1_dy = 2 * (btn_1_left_clicked - btn_1_right_clicked);
        int8_t racket2_dy = 2 * (btn_2_left_clicked - btn_2_right_clicked);

        _delay_ms(25); // 16 ms almost 60 fps

        game_eraseObject(ball_current, BALL_SIZE);
        game_eraseObject(racket_1, RACKET_SIZE);
        game_eraseObject(racket_2, RACKET_SIZE);

        game_racket_move(racket_1, racket1_dy);
        game_racket_move(racket_2, racket2_dy);
        game_ball_move(ball_current);

        game_handleGoal(ball_current, racket_1, racket_2, &score);
    }

    return 0;
}


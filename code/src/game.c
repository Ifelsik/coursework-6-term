#include "../include/game.h"
#include "../lib/include/free_sans.h"


const ball_t BALL_START = {  // to PROGMEM
    {78, 62, 1, 1},
    {79, 62, 1, 1},
    {80, 62, 1, 1},
    {78, 63, 1, 1},
    {79, 63, 1, 1},
    {80, 63, 1, 1},
    {78, 64, 1, 1},
    {79, 64, 1, 1},
    {80, 64, 1, 1}
};

const racket_t RACKET_START_1 = {  // to progmem
    {0, 53}, {1, 53},
    {0, 54}, {1, 54},
    {0, 55}, {1, 55},
    {0, 56}, {1, 56},
    {0, 57}, {1, 57},
    {0, 58}, {1, 58},
    {0, 59}, {1, 59},
    {0, 60}, {1, 60},
    {0, 61}, {1, 61},
    {0, 62}, {1, 62},
    {0, 63}, {1, 63},
    {0, 64}, {1, 64},
    {0, 65}, {1, 65},
    {0, 66}, {1, 66},
    {0, 67}, {1, 67},
    {0, 68}, {1, 68},
    {0, 69}, {1, 69},
    {0, 70}, {1, 70},
    {0, 71}, {1, 71},
    {0, 72}, {1, 72},
};

const racket_t RACKET_START_2 = {  // to progmem
    {158, 53}, {159, 53},
    {158, 54}, {159, 54},
    {158, 55}, {159, 55},
    {158, 56}, {159, 56},
    {158, 57}, {159, 57},
    {158, 58}, {159, 58},
    {158, 59}, {159, 59},
    {158, 60}, {159, 60},
    {158, 61}, {159, 61},
    {158, 62}, {159, 62},
    {158, 63}, {159, 63},
    {158, 64}, {159, 64},
    {158, 65}, {159, 65},  
    {158, 66}, {159, 66},
    {158, 67}, {159, 67},
    {158, 68}, {159, 68},
    {158, 69}, {159, 69},
    {158, 70}, {159, 70},
    {158, 71}, {159, 71},
    {158, 72}, {159, 72},
    };

static inline void __draw_object_color(pixel_t object[], uint8_t size, uint16_t color) {
    for (uint8_t i = 0; i < size; i++) {
        st7735_draw_pixel(object[i].x, object[i].y, color);
    }
}

static inline uint8_t __abs(int8_t value) {
    return value < 0 ? -value : value;
}

static inline bool __isObjectsCollision(pixel_t obj1[], pixel_t obj2[], uint8_t obj1_size, uint8_t obj2_size) {
    for (uint8_t i = 0; i < obj1_size; i++) {
        for (uint8_t j = 0; j < obj2_size; j++) {
            if (obj1[i].x == obj2[j].x && obj1[i].y == obj2[j].y) {
                return true;
            }
        }
    }
    return false;
}

void game_init(ball_t ball, racket_t racket1, racket_t racket2, score_t *score) {
    // buttons initialization
    setPinIOMode(&DDRC, DDC3, PORT_MODE_IN);
    setPinIOMode(&DDRC, DDC2, PORT_MODE_IN);
    setPinIOMode(&DDRC, DDC1, PORT_MODE_IN);
    setPinIOMode(&DDRC, DDC0, PORT_MODE_IN);

    // enable pull-up resistor for buttons
    setPinValue(&PORTC, PORTC3, 1); 
    setPinValue(&PORTC, PORTC2, 1);
    setPinValue(&PORTC, PORTC1, 1);
    setPinValue(&PORTC, PORTC0, 1);

    game_init_LEDs();

    st7735_set_orientation(ST7735_LANDSCAPE);

    st7735_fill_rect(0, 1, st7735_default_height_18, st7735_default_width - 2, ST7735_COLOR_BLACK);
    st7735_draw_line(0, 0, st7735_default_height_18, 0, ST7735_COLOR_WHITE);
    st7735_draw_line(0, st7735_default_width - 1, st7735_default_height_18,
        st7735_default_width - 1, ST7735_COLOR_WHITE);

    game_copyObject(BALL_START, ball, BALL_SIZE);

    game_copyObject(RACKET_START_1, racket1, RACKET_SIZE);
    game_copyObject(RACKET_START_2, racket2, RACKET_SIZE);

    score->player_1 = 0;
    score->player_2 = 0;
}

void game_renderObject(pixel_t object[], uint8_t size) {
    __draw_object_color(object, size, ST7735_COLOR_WHITE);
}

void game_eraseObject(pixel_t object[], uint8_t size) {
    __draw_object_color(object, size, ST7735_COLOR_BLACK);
}

uint8_t game_ball_racketCollision(ball_t ball, racket_t racket) {
    uint8_t collision = NO_COLLISION;

    /*
        virtual ball movement over axis x and y
    */
    ball_t ball_with_x_movement_only, ball_with_y_movement_only;
    game_copyObject(ball, ball_with_x_movement_only, BALL_SIZE);
    game_copyObject(ball, ball_with_y_movement_only, BALL_SIZE);

    for (uint8_t i = 0; i < BALL_SIZE; i++) {
        ball_with_x_movement_only[i].y -= ball_with_x_movement_only[i].dy;
        ball_with_y_movement_only[i].x -= ball_with_y_movement_only[i].dx;
    }

    if (
        __isObjectsCollision(
            racket,
            ball_with_x_movement_only,
            RACKET_SIZE,
            BALL_SIZE
        )
    ) collision += V_COLLISION;

    if (
        __isObjectsCollision(
            racket,
            ball_with_y_movement_only,
            RACKET_SIZE,
            BALL_SIZE
        )
    ) collision += H_COLLISION;;
    
    return collision;
}

uint8_t game_ball_boundaryCollision(ball_t ball) {
    uint8_t horizontal = NO_COLLISION;
    for (uint8_t i = 0; i < BALL_SIZE; i++) {
        if (ball[i].y < 1 || ball[i].y >= st7735_default_width - 1) {
            horizontal = H_COLLISION;
        }
    }
    return horizontal;
}

void game_copyObject(const pixel_t src[], pixel_t dst[], uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void game_ball_setSpeed(ball_t ball, int8_t dx, int8_t dy) {
    for (uint8_t i = 0; i < BALL_SIZE; i++) {
        ball[i].dx = dx;
        ball[i].dy = dy;
    }
}

void game_ball_move(ball_t ball) {
    for (int8_t i = 0; i < BALL_SIZE; i++) {
        ball[i].x += ball[i].dx;
        ball[i].y += ball[i].dy;
    }
}

void game_racket_move(racket_t racket, int8_t dy) {
    bool is_collided = false;
    for (uint8_t i = 0; i < RACKET_SIZE; i++) {
        if (racket[i].y + dy < 1 || racket[i].y + dy >= st7735_default_width - 1) {
            is_collided = true;
        }
    }

    if (!is_collided) {
        for (uint8_t i = 0; i < RACKET_SIZE; i++) {
            racket[i].y += dy;
        }
    }
}

void game_handleGoal(ball_t ball, racket_t racket1, racket_t racket2, score_t *score) {
    uint8_t player_points = 0;
    int8_t dx = ball[0].dx;

    DEBUG_PRINT("dx: %d\r\n", dx);

    if (ball[0].x < 0) {
        player_points = ++(score->player_2);
    } else if (ball[0].x >= st7735_default_height_18) {
        player_points = ++(score->player_1);
    } else {
        return;
    }
    // reverse the ball direction to the side who scored
    dx *= (-1);

    DEBUG_PRINT("dx after: %d\r\n", dx);

    game_renderScore(score);

    _delay_ms(1000);

    game_copyObject(BALL_START, ball, BALL_SIZE);
    game_copyObject(RACKET_START_1, racket1, RACKET_SIZE);
    game_copyObject(RACKET_START_2, racket2, RACKET_SIZE);

    game_ball_setSpeed(ball, dx, ball[0].dy);

    if (player_points > WIN_POINTS) {
        score->player_1 = 0;
        score->player_2 = 0;
    }

    // clear the score text
    st7735_fill_rect(0, 1, st7735_default_height_18, st7735_default_width - 2, ST7735_COLOR_BLACK);
}

bool game_controller_1_isLeftBtnClicked() {
    return !getPinValue(PINC, PINC2);
}

bool game_controller_1_isRightBtnClicked() {
    return !getPinValue(PINC, PINC3);
}

bool game_controller_2_isLeftBtnClicked() {
    return !getPinValue(PINC, PINC0);
}

bool game_controller_2_isRightBtnClicked() {
    return !getPinValue(PINC, PINC1);
}

char* _numbers[] = {
    "0",
    "1",
    "2",
    "3"
};

void game_renderScore(score_t *score) {
    uint16_t x = st7735_default_width / 2 - 6;
    uint16_t winner_x = st7735_default_width / 2 - 42;
    uint16_t y = st7735_default_height_18 / 2 + 16;

    DEBUG_PRINT("score: %d %d\n", score->player_1, score->player_2);

    if (score->player_1 <= WIN_POINTS && score->player_2 <= WIN_POINTS) {
        st7735_set_orientation(ST7735_PORTRAIT_INV);
        st7735_draw_text(
            x,
            y,
            _numbers[score->player_1],
            &FreeSans,
            1,
            ST7735_COLOR_RED
        );

        st7735_set_orientation(ST7735_PORTRAIT);
        st7735_draw_text(
            x,
            y,
            _numbers[score->player_2],
            &FreeSans,
            1,
            ST7735_COLOR_BLUE
        );
    } else if (score->player_1 > WIN_POINTS) {
        st7735_set_orientation(ST7735_PORTRAIT_INV);
        st7735_draw_text(
            winner_x,
            y,
            "WINNER!",
            &FreeSans,
            1,
            ST7735_COLOR_RED
        );
    } else if (score->player_2 > WIN_POINTS) {
        st7735_set_orientation(ST7735_PORTRAIT);

        st7735_draw_text(
            winner_x,
            y,
            "WINNER!",
            &FreeSans,
            1,
            ST7735_COLOR_BLUE
        );
    }

    st7735_set_orientation(ST7735_LANDSCAPE);
}

void game_init_LEDs() {
    setPinIOMode(&DDRD, DDD0, PORT_MODE_OUT);
    setPinIOMode(&DDRD, DDD1, PORT_MODE_OUT);
    setPinIOMode(&DDRD, DDD2, PORT_MODE_OUT);

    setPinValue(&PORTD, PORTD0, 1);
    setPinValue(&PORTD, PORTD1, 1);
    setPinValue(&PORTD, PORTD2, 1);
}

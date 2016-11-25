// Some settings for the game

#define BLANK ' '
#define DFL_SYMBOL 'o'
#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define X_INIT 10
#define Y_INIT 10
#define TICKS_PER_SEC 50

#define X_TTM 5
#define Y_TTM 8

struct ppball {
    int y_pos;
    int x_pos;
    int y_ttm;
    int x_ttm;
    int y_ttg;
    int x_ttg;
    int y_dir;
    int x_dir;
    char symbol;
};
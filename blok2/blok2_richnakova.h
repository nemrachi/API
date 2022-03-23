#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <math.h>

// terminal colors
#define RED 1
#define GREEN 2
#define BLUE 4
#define YELLOW 3
#define PURPLE 5
// extended ascii symbols
// simple lines
#define HOR_LINE 196 // ─
#define VER_LINE 179 // │
// up
#define U_L_CORNER 218 // ┌
#define U_R_CORNER 191 // ┐
#define U_MID_CORNER 194 // ┬
// midle
#define MID_L_LINE 180 // ┤
#define MID_R_LINE 195 // ├
#define MID_MID_LINE 197 // ┼
// bottom
#define B_L_CORNER 192 // └
#define B_MID_CORNER 193 // ┴
#define B_R_CORNER 217 // ┘

// ul2-1-1 +1
void ul2_1_1();

// ul2-1-2 +1
void ul2_1_2();

// ul2-1-3 +3
void ul2_1_3();

// ul2-1-4 +2
void ul2_1_4();

// ul2-2 +3
void ul2_2();

// ul2-5-1 +1
void ul2_5_1();
void printColoredNumbersRange(int a, int b, int color);

// ull2-5-2 +3
void gotoxy(int x, int y);
void consoleColor(int color);
void resetConsoleColor();
int intlen(int i);
void printLine(int line);
void printTabHorLine(int left, int middleSplitLine, int right, int leftLen, int rightLen);
void ul2_5_2();
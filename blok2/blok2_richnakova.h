// ISO-8859-2 and Windows-1250 encodind
#define ISO_L 165 // Ľ
#define WIN_L 188 // Ľ
#define ISO_S 169 // Š
#define WIN_S 138 // Š
#define ISO_T 171 // Ť
#define WIN_T 141 // Ť
#define ISO_Z 174 // Ž
#define WIN_Z 142 // Ž
#define ISO_l 181 // ľ
#define WIN_l 190 // ľ
#define ISO_s 185 // š
#define WIN_s 154 // š
#define ISO_t 187 // ť
#define WIN_t 157 // ť
#define ISO_z 190 // ž
#define WIN_z 158 // ž 
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

// void ul2-3-1 +2
void ul2_3_1();

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
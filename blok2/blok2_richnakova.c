#include "blok2_richnakova.h"

static int a, b, result;
/////////////////////////////////////////////////////////////
///////////////////////////ul2-1-1///////////////////////////
/////////////////////////////////////////////////////////////
void ul2_1_1() { // a + b = result in asm
    scanf("%d %d", &a, &b);

    asm("mov a,%eax \n"
        "mov b,%ebx \n"
        "add %eax,%ebx \n" 
        "mov %ebx,result \n"
    ); // also can use __asm__()

    printf("%d + %d = %d", a, b, result);
}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-1-2///////////////////////////
/////////////////////////////////////////////////////////////
void ul2_1_2() { // a * 2 in asm
    scanf("%d", &a);

    asm("mov a,%eax \n"
        "shl $1,%eax \n"
        "mov %eax,result \n"
    );
    
    printf("%d * 2 = %d", a, result);}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-1-3///////////////////////////
/////////////////////////////////////////////////////////////
void ul2_1_3() {
    scanf("%d", &a);
    if (a < 0 || a > 15) { return; }

    asm("mov a,%eax \n"
        "cmp $10,%eax \n"
        "jge L1 \n"
        "jmp L2 \n"
        "L1: \n"
        "add $55,%eax \n"
        "jmp L3 \n"
        "L2: \n"
        "add $48,%eax \n"
        "L3: \n"
        "mov %eax,result \n"
    );

    printf("dec %d = char %c", result, (char)result);
}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-1-4///////////////////////////
/////////////////////////////////////////////////////////////
static char arr[3];

void ul2_1_4() {
    asm("mov $0,%eax \n"
        "cpuid \n"
        "mov %ebx,arr+0 \n"
        "mov %edx,arr+4 \n"
        "mov %ecx,arr+8 \n"
    );
        
    printf("Processor: %s", arr);
}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-2///////////////////////////
/////////////////////////////////////////////////////////////
void ul2_2() {
    char str[] = "architektura_pocitacov_je_super_predmet_milujem_ho";
    printf("String: \t\t%s\n", str);

    char *strP = str;
    printf("String pointer: \t%p\n", strP);
    printf("String pointer +1: \t%p\n", ++strP);

    *strP = 'm'; // can't properly write string in 1 char memory slot, it changes only char
    printf("Second string: \t\t%s\n", strP);

    strP += INT_MAX;
    *strP = 'n';
    printf("Third string: %s\n", strP);
}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-5-1///////////////////////////
/////////////////////////////////////////////////////////////
void printColoredNumbersRange(int a, int b, int color) {
    printf("\033[0;3%dm", color); // set color
    for (int i = a; i <= b; i++) {
        printf("%03d\n", i);
        Sleep(100);
    }
    printf("\033[0m"); // reset color
}

void ul2_5_1() {
    printColoredNumbersRange(0, 10, RED);
    printColoredNumbersRange(11, 22, GREEN);
    printColoredNumbersRange(23, 35, BLUE);
    printColoredNumbersRange(36, 50, YELLOW);
}

/////////////////////////////////////////////////////////////
///////////////////////////ul2-5-2///////////////////////////
/////////////////////////////////////////////////////////////
// set cursor in console to position (x, y)
void gotoxy(int x, int y) {
    COORD cord;
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

void consoleColor(int color) {
    printf("\033[0;3%dm", color); // set color of console
}

void resetConsoleColor() {
    printf("\033[0m");
}

// source: https://stackoverflow.com/questions/3068397/finding-the-length-of-an-integer-in-c?msclkid=d0ce1292aaf411eca5c928721781f128
int intlen(int i) {
    return floor(log10(abs(i))) + 1;
}

void printLine(int line) {
    consoleColor(BLUE);
    printf("%c", line);
    consoleColor(PURPLE);
}

/*  param left = most left symbol
    param middleSplitLine = middle symbol, which splits line
    param right = most right symbol
    param leftLen = number of horizontal lines in left section
    param rightLen = number of horizontal lines in right section
*/
void printTabHorLine(int left, int middleSplitLine, int right, int leftLen, int rightLen) {
    consoleColor(BLUE);
    printf("%c", left);
    for (int i = 0; i < leftLen; i++) printf("%c", HOR_LINE);
    printf("%c", middleSplitLine);
    for (int i = 0; i < rightLen; i++) printf("%c", HOR_LINE);
    printf("%c", right);
    consoleColor(PURPLE);
}

void ul2_5_2() {
    int x = 6, y = 3, maxLen;
    char name[200], telNum[17], tmp;
    float height, weight;

    printf("Enter x y:\t\t");
    scanf("%d %d", &x, &y);

    printf("Enter name:\t\t");
    scanf("%c", &tmp);
    fgets(name, 200, stdin);
    name[strlen(name)-1] = '\0';

    printf("Enter height:\t\t");
    scanf("%f", &height);

    printf("Enter weight:\t\t");
    scanf("%f", &weight);

    printf("Enter phone number:\t");
    scanf("%c", &tmp);
    fgets(telNum, 17, stdin);
    telNum[strlen(telNum)-1] = '\0';
    
    // get max lenght of value cell
    maxLen = strlen(name) + 2;
    if (maxLen < intlen((int)height)+8) { maxLen = intlen((int)height) + 8; }
    if (maxLen < intlen((int)weight)+8) { maxLen = intlen((int)weight) + 8; }
    if (maxLen < strlen(telNum)) { maxLen = strlen(telNum)+2; }
    
    system("cls"); // clear screen
    gotoxy(x, y); // point cursor in terminal

    // print table
    int half = (maxLen)/2;
    printTabHorLine(U_L_CORNER, HOR_LINE, HOR_LINE, 0, half-3);
    printf(" Z%cznam 001 ", 160);
    printTabHorLine(HOR_LINE, HOR_LINE, U_R_CORNER, 0, maxLen-half-2);
    gotoxy(x, ++y);

    printTabHorLine(MID_R_LINE, U_MID_CORNER, MID_L_LINE, 10, maxLen);
    gotoxy(x, ++y);

    printLine(VER_LINE);
    printf(" Meno%4s ", "");
    printLine(VER_LINE);
    printf(" %*s%s ", maxLen-strlen(name)-2 , "", name);
    printLine(VER_LINE);
    gotoxy(x, ++y);

    printTabHorLine(MID_R_LINE, MID_MID_LINE, MID_L_LINE, 10, maxLen);
    gotoxy(x, ++y);

    printLine(VER_LINE);
    printf(" V%c%cka%4s", 236, 231, "");
    printLine(VER_LINE);
    printf(" %*s%.2f cm ", maxLen-intlen(height)-8, "", height);
    printLine(VER_LINE);
    fflush(stdin);
    gotoxy(x, ++y);

    printTabHorLine(MID_R_LINE, MID_MID_LINE, MID_L_LINE, 10, maxLen);
    gotoxy(x, ++y);

    printLine(VER_LINE);
    printf(" Hmotnos%c ", 156);
    printLine(VER_LINE);
    printf(" %*s%.2f kg ", maxLen-intlen(weight)-8, "", weight);
    printLine(VER_LINE);
    gotoxy(x, ++y);

    printTabHorLine(MID_R_LINE, MID_MID_LINE, MID_L_LINE, 10, maxLen);
    gotoxy(x, ++y);

    printLine(VER_LINE);
    printf(" Tel.%5s", "");
    printLine(VER_LINE);
    printf(" %*s%s ", maxLen - strlen(telNum)-2, "", telNum);
    printLine(VER_LINE);
    gotoxy(x, ++y);

    printTabHorLine(B_L_CORNER, B_MID_CORNER, B_R_CORNER, 10, maxLen);

    resetConsoleColor();
}

///////////////////////////main///////////////////////////
int main() {
    // cd "c:\Users\emari\Projects\API\blok2\" && "C:\Program Files (x86)\MinGW\bin\"gcc blok2_richnakova.c -o blok2_richnakova && "c:\Users\emari\Projects\API\blok2\"blok2_richnakova
    //ul2_1_1();
    //ul2_1_2();
    //ul2_1_3();
    //ul2_1_4();
    //ul2_2();
    //ul2_5_1();
    ul2_5_2();

    return 0;
}
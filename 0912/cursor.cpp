#include <stdio.h>
#include <windows.h>
void gotoxy(int x, int y);
int main(void)
{
gotoxy(2,4);
printf("Hello");

gotoxy(40, 20);
printf("Hello");
return 0;
}
void gotoxy(int x, int y)
{
COORD Pos = {(SHORT)(x - 1), (SHORT)(y - 1)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


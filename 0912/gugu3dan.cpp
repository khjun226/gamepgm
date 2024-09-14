#include <stdio.h>
#include <windows.h>

void gotoxy(int x, int y)
{
    COORD Pos = {(SHORT)(x - 1), (SHORT)(y - 1)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int i;
int main(void)
{
    for (i = 1; i <= 9; i++)
    {
        gotoxy(35, 5 + i); 
        printf("%d*%d=%2d", 3, i, 3 * i);
    }
    printf("\n");
    return 0;
}
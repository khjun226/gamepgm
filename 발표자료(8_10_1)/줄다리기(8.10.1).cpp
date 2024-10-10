#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define LINE_LENGTH 41
#define MAX_NAME_LENGTH 7
#define USER_START (LINE_LENGTH / 4)
#define COMP_START ((LINE_LENGTH * 3) / 4)
#define CENTER (LINE_LENGTH / 2)
#define RANKING_FILE "rankings.txt"
#define MAX_RANK 5

typedef struct {
    char name[MAX_NAME_LENGTH];
    int difficulty;
    double remaining_time;
} Rank;

void gotoxy(int x, int y);
void display_line(int position, char team_name[]);
void display_start(char team_name[], int wins, int losses, int round);
void make_decision(int position, int *wins, int *losses, char team_name[], int time_out, int difficulty, double remaining_time);
void game_control(int *position, double *remaining_time, int difficulty, clock_t start, double game_time);
double intro_game(char team_name[]);
void display_title();
void countdown();
void clear_line(int y);
void set_console_size();
void set_text_color(int color);
void display_result_screen(int user_won);
void show_rankings();
void update_rankings(char team_name[], int difficulty, double remaining_time);
int compare_rank(const void *a, const void *b);
void save_rankings(Rank rankings[], int count);
int load_rankings(Rank rankings[]);
void show_instructions();

int main(void) {
    int score_wins = 0, score_losses = 0;
    char team_name[MAX_NAME_LENGTH];
    double game_time;
    clock_t start;

    srand((unsigned int)time(NULL));
    set_console_size();

    int first_run = 1;

    while (1) {
        score_wins = 0;
        score_losses = 0;

        if (first_run) {
            display_title();
            int key = _getch();
            if (key == '1') {
                show_rankings();

                printf("\n\n\n                            �ƹ� Ű�� ������ ���� ȭ������ ���ư�����.");
                _getch();
                system("cls");
                continue;
            }
            else if (key == '2') {
                show_instructions();

                printf("\n\n\n                            �ƹ� Ű�� ������ ���� ȭ������ ���ư�����.");
                _getch();
                system("cls");
                continue;
            }
            system("cls");
            game_time = intro_game(team_name);
            first_run = 0;
        }

        int difficulty;

        printf("\n\n\n");
        printf("                         ���̵��� �����ϼ��� (1: ����, 2: �߰�, 3: �����): ");
        if (scanf("%d", &difficulty) != 1 || difficulty < 1 || difficulty > 3) {
            printf("�߸��� ���̵��� �Է��߽��ϴ�. ������ �����մϴ�.\n");
            return 1;
        }

        if (difficulty == 1) game_time = 15;
        else if (difficulty == 2) game_time = 12;
        else if (difficulty == 3) game_time = 10;

        countdown();

        int round = 1;

        do {
            system("cls");
            display_start(team_name, score_wins, score_losses, round);
            start = clock();
            double remaining_time = game_time;
            int position = CENTER;
            int time_out = 0;

            while (remaining_time > 0 && (position > 0 && position < LINE_LENGTH - 1)) {
                game_control(&position, &remaining_time, difficulty, start, game_time);

                while (_kbhit()) {
                    if (_getch() == 32) {
                        position -= 3;
                        if (position < 0) position = 0;
                    }
                }

                if (remaining_time <= 0) {
                    time_out = 1;
                    break;
                }
            }

            make_decision(position, &score_wins, &score_losses, team_name, time_out, difficulty, remaining_time);
            round++;
        } while (score_wins < 2 && score_losses < 2);

        display_result_screen(score_wins == 2);

        printf("\n                           ������ �ٽ� �����Ϸ��� 'y' Ű�� ������, �����Ϸ��� 'n' Ű�� ��������.\n");

        while (1) {
            int key = _getch();
            if (key == 'n' || key == 'N') {
                return 0;
            } else if (key == 'y' || key == 'Y') {
                first_run = 1;
                system("cls");
                break;
            }
        }
    }

    return 0;
}

void set_console_size() {
    system("mode con: cols=100 lines=30");
    SetConsoleTitle("Tug Of War");
}

void set_text_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void display_title() {
    system("cls");
    set_text_color(10);

    gotoxy(20, 5);
    printf("______                 _____   __   _    _              ");
    gotoxy(20, 6);
    printf("|_   _|               |  _  | / _| | |  | |             ");
    gotoxy(20, 7);
    printf("  | |   _   _   __ _  | | | || |_  | |  | |  __ _  _ __ ");
    gotoxy(20, 8);
    printf("  | |  | | | | / _` | | | | ||  _| | |/\\| | / _` || '__|");
    gotoxy(20, 9);
    printf("  | |  | |_| || (_| | \\ \\_/ /| |   \\  /\\  /| (_| || |   ");
    gotoxy(20, 10);
    printf("  \\_/   \\__,_| \\__, |  \\___/ |_|    \\/  \\/  \\__,_||_|   ");
    gotoxy(20, 11);
    printf("                __/ |                                   ");
    gotoxy(20, 12);
    printf("               |___/                                    ");

    set_text_color(14);
    gotoxy(35, 14);
    printf("Welcome to the Tug of War Game!");
    gotoxy(33, 16);
    printf("1. ��ŷ����   2. ���� ��� ����");
    gotoxy(37, 18);
    printf("�ƹ�Ű�� ������ �����ϱ�");
    set_text_color(7);

    Sleep(500);
    gotoxy(38, 22);
    set_text_color(12);
    printf("<<< Are you ready? >>>");

    set_text_color(7);
}

void countdown() {
    system("cls");
    gotoxy(48, 12);
    for (int i = 3; i > 0; i--) {
        set_text_color(11);
        printf("%d", i);
        Sleep(1000);
        gotoxy(48, 12);
        printf("   ");
    }
    system("cls");
    gotoxy(46, 12);
    set_text_color(10);
    printf("����!\n");
    Beep(750, 500);
    gotoxy(38, 15);
    printf("�����̽��ٸ� ��Ÿ���ּ���!!");
    Sleep(1000);
    set_text_color(7);
    system("cls");
}

double intro_game(char team_name[]) {
    gotoxy(35, 10);
    printf("�� �̸��� �Է��ϼ���: ");
    scanf("%s", team_name);
    return 20;
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void game_control(int *position, double *remaining_time, int difficulty, clock_t start, double game_time) {
    int computer_move;
    int sleep_time;

    // ���̵��� ���� ��ǻ�� �̵� �Ÿ� �� Sleep �ð� ����
    if (difficulty == 1) {
        computer_move = (rand() % 2);  // 0 �Ǵ� 1ĭ �̵�
        sleep_time = 150;
    } else if (difficulty == 2) {
        computer_move = (rand() % 2) + 1;  // 1 �Ǵ� 2ĭ �̵�
        sleep_time = 100;
    } else {
        computer_move = (rand() % 3) + 1;  // 1~3ĭ �̵�
        sleep_time = 80;
    }

    Sleep(sleep_time);
    *position += computer_move;

    if (*position >= LINE_LENGTH) *position = LINE_LENGTH - 1;

    clock_t current_time = clock();
    double elapsed_time = (double)(current_time - start) / CLOCKS_PER_SEC;
    *remaining_time = game_time - elapsed_time;

    if (*remaining_time < 0) *remaining_time = 0;

    display_line(*position, "User Team");
    gotoxy(40, 2);
    printf("���ѽð� : %4.1f��", *remaining_time);
}

void display_line(int position, char team_name[]) {
    int line[LINE_LENGTH] = { 0 };
    line[CENTER] = 1;
    line[USER_START] = 2;
    line[COMP_START] = 3;

    gotoxy(20, 10);
    set_text_color(10);
    printf("[User Team]");
    set_text_color(7);
    gotoxy(70, 10);
    set_text_color(12);
    printf("[Computer Team]");
    set_text_color(7);

    gotoxy(30, 11);
    for (int i = 0; i < LINE_LENGTH; i++) {
        if (i == position) {
            printf("|");
        }
        else if (line[i] == 1) {
            printf("��");
        }
        else if (line[i] == 2) {
            printf("��");
        }
        else if (line[i] == 3) {
            printf("��");
        }
        else {
            printf("��");
        }
    }
}

void display_start(char team_name[], int wins, int losses, int round) {
    gotoxy(0, 0);
    printf("==================================================================================================\n");
    gotoxy(42, 1);
    printf("Tug of War Game\n");
    gotoxy(0, 2);
    printf("==================================================================================================\n");
    gotoxy(20, 3);
    printf("%s�� �¸�: %d", team_name, wins);
    gotoxy(70, 3);
    printf("��ǻ���� �¸�: %d", losses);
    gotoxy(45, 4);
    printf("Round %d\n", round);
    gotoxy(0, 5);
    printf("==================================================================================================\n");

    display_line(CENTER, team_name);

    gotoxy(35, 22);
    set_text_color(12);
    printf("�����̽��ٸ� ���� ���� ��⼼��.");
    set_text_color(7);
}

void make_decision(int position, int *wins, int *losses, char team_name[], int time_out, int difficulty, double remaining_time) {
    if (position <= 0) {
        (*wins)++;
        gotoxy(35, 14);
        printf("%s���� �̰���ϴ�!", team_name);

        if (*wins == 2) {
            update_rankings(team_name, difficulty, remaining_time);
        }
    }
    else if (position >= LINE_LENGTH - 1 || time_out) {
        (*losses)++;
        if (time_out) {
            gotoxy(35, 14);
            printf("�ð� �ʰ�! ��ǻ������ �̰���ϴ�.");
        }
        else {
            gotoxy(35, 14);
            printf("��ǻ������ �̰���ϴ�.");
        }
    }
    else {
        gotoxy(35, 14);
        printf("�����ϴ�.");
    }

    if (*wins < 2 && *losses < 2) {
        Sleep(2000);
    }
}

void display_result_screen(int user_won) {
    system("cls");
    if (user_won) {
        set_text_color(10);
        gotoxy(38, 12);
        printf(" __     __          __          ___       _ ");
        gotoxy(38, 13);
        printf(" \\ \\   / /          \\ \\        / (_)     | |");
        gotoxy(38, 14);
        printf("  \\ \\_/ /__  _   _   \\ \\  /\\  / / _ _ __ | |");
        gotoxy(38, 15);
        printf("   \\   / _ \\| | | |   \\ \\/  \\/ / | | '_ \\| |");
        gotoxy(38, 16);
        printf("    | | (_) | |_| |    \\  /\\  /  | | | | |_|");
        gotoxy(38, 17);
        printf("    |_|\\___/ \\__,_|     \\/  \\/   |_|_| |_(_)");
        gotoxy(57, 19);
        printf("You Win!");
    }
    else {
        set_text_color(12);
        gotoxy(35, 12);
        printf("  ____                         ___                 ");
        gotoxy(35, 13);
        printf(" / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ ");
        gotoxy(35, 14);
        printf("| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|");
        gotoxy(35, 15);
        printf("| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |   ");
        gotoxy(35, 16);
        printf(" \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   ");
        gotoxy(57, 18);
        printf("Game Over!");
    }
    set_text_color(7);
}

void show_rankings() {
    system("cls");
    Rank rankings[MAX_RANK];
    int count = load_rankings(rankings);

    gotoxy(40, 5);
    set_text_color(14);
    printf("=== ��ŷ ===");
    set_text_color(7);

    for (int i = 0; i < count; i++) {
        gotoxy(35, 7 + i);
        printf("%d��: %s - ���̵�: %d, ���� �ð�: %.1f��", i + 1, rankings[i].name, rankings[i].difficulty, rankings[i].remaining_time);
    }

    if (count == 0) {
        gotoxy(40, 9);
        printf("���� ��ŷ�� �����ϴ�.");
    }
}

void update_rankings(char team_name[], int difficulty, double remaining_time) {
    Rank rankings[MAX_RANK + 1];
    int count = load_rankings(rankings);

    // ���ο� ��� �߰�
    strcpy(rankings[count].name, team_name);
    rankings[count].difficulty = difficulty;
    rankings[count].remaining_time = remaining_time;
    count++;

    // ��ŷ ����
    qsort(rankings, count, sizeof(Rank), compare_rank);

    if (count > MAX_RANK) {
        count = MAX_RANK;
    }

    save_rankings(rankings, count);
}

int compare_rank(const void *a, const void *b) {
    Rank *rankA = (Rank *)a;
    Rank *rankB = (Rank *)b;

    // ���̵� �������� ��
    if (rankA->difficulty != rankB->difficulty) {
        return rankB->difficulty - rankA->difficulty;
    }
    else {
        // ���� �ð� �������� ��
        if (rankA->remaining_time < rankB->remaining_time) {
            return 1;
        }
        else if (rankA->remaining_time > rankB->remaining_time) {
            return -1;
        }
        else {
            return 0;
        }
    }
}

void save_rankings(Rank rankings[], int count) {
    FILE *fp = fopen(RANKING_FILE, "w");
    if (fp == NULL) {
        printf("��ŷ ������ �� �� �����ϴ�.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d %lf\n", rankings[i].name, rankings[i].difficulty, rankings[i].remaining_time);
    }

    fclose(fp);
}

int load_rankings(Rank rankings[]) {
    FILE *fp = fopen(RANKING_FILE, "r");
    int count = 0;

    if (fp == NULL) {
        return 0;
    }

    while (fscanf(fp, "%s %d %lf", rankings[count].name, &rankings[count].difficulty, &rankings[count].remaining_time) == 3) {
        count++;
        if (count >= MAX_RANK) {
            break;
        }
    }

    fclose(fp);
    return count;
}

void show_instructions() {
    system("cls");
    set_text_color(14);
    gotoxy(40, 5);
    printf("=== ���� ��� ===");
    set_text_color(7);

    gotoxy(20, 7);
    printf("1. ������ ���۵Ǹ�, ���� �ð� ���� �����̽��ٸ� ������ ���� ���� �����⼼��.");
    gotoxy(20, 9);
    printf("2. ���̵��� �������� ���� �ð��� ª������ ��� ���� �� �������ϴ�.");
    gotoxy(20, 11);
    printf("3. ���� �ð��� �����ų� ��� ������ �������� ���忡�� �й��մϴ�.");
    gotoxy(20, 13);
    printf("4. 2���带 ���� �¸��ϸ� ���ӿ��� �¸��մϴ�.");
}


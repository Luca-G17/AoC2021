#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct tile {
    int value;
    bool activated;
} tile;

typedef tile* board[5][5];

void freeBoards(int b, board bds[b]){
    for (int i = 0; i < b; i++){ // TODO: Change 3
        for (int f = 0; f < 5; f++){
            for (int j = 0; j < 5; j++){
                free(bds[i][f][j]);
            }
        }
    }
}
void printBoard(board boards[], int n){
    n--;
    for (int i = 0; i < 5; i++){
        for (int f = 0; f < 5; f++){
            printf("%2d ", boards[n][i][f]->value);
        }
        printf("\n");
    }
    printf("\n");
}
int readFile(char* filename, int b, board boards[b], int n, int nums[n]){
    FILE *in = fopen(filename, "r");
    char buffer[400];
    char* num;
    fgets(buffer, sizeof(buffer), in);
    num = strtok(buffer, ",");
    int x = 0;
    while (num != NULL){
        nums[x++] = atoi(num);
        num = strtok(NULL, ",");
    }
    nums[x] = -1;

    int row = 0;
    int col = 0;
    int boardCount = 0;
    fgets(buffer, sizeof(buffer), in);
    while (!feof(in)){
        if (strlen(buffer) > 2){
            num = strtok(buffer, " \n");
            while (num != NULL){
                tile *t = malloc(sizeof(tile));
                t->activated = false;
                t->value = atoi(num);
                boards[boardCount][row][col++] = t;
                num = strtok(NULL, " \n");
            }
            col = 0;
            row++;
        }
        else if (row != 0){
            row = 0;
            boardCount++;
        }
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return boardCount;
}
bool activatePos(int row, int col, board b){
    b[row][col]->activated = true;
    bool win = true;
    for (int i = 0; i < 5; i++)
        win = win && b[row][i]->activated;
    if (win) return win;
    win = true;
    for (int i = 0; i < 5; i++)
        win = win && b[i][col]->activated;
    return win;
}
bool findNum(board b, int n){
    bool win = false;
    for (int i = 0; i < 5; i++){
        for (int f = 0; f < 5; f++){
            if (b[i][f]->value == n)
                win = win || activatePos(i, f, b);
        }
    }
    return win;
}
int calculateScore(board b, int n){
    int score = 0;
    for (int i = 0; i < 5; i++){
        for (int f = 0; f < 5; f++){
            if (!(b[i][f]->activated))
                score += b[i][f]->value;
        }
    }
    return score * n;
}
int findLastWinningScore(int b, board boards[b], int nums[]){
    int i = 0;
    int bCount = b;
    while (nums[i] != -1 && bCount > 1){
        for (int f = 0; f < b; f++){
            if (boards[f][0][0]->value != -1){
                if (findNum(boards[f], nums[i])){
                    boards[f][0][0]->value = -1;
                    bCount--;
                }
            }
        }
        i++;
    }
    int lastWinningBoard = 0;
    for (int f = 0; f < b; f++){
        if (boards[f][0][0]->value != -1){
            lastWinningBoard = f;
        }
    }
    bool won = false;
    while (nums[i] != -1 && !won){
        if (findNum(boards[lastWinningBoard], nums[i]))
            won = true;
        i++;
    }
    if (won)
        return calculateScore(boards[lastWinningBoard], nums[--i]);
    else
        return -1;
}
int findWinningScore(int b, board boards[b], int nums[]){
    int winner = -1;
    int i = 0;
    while (nums[i] != -1 && winner == -1){
        for (int f = 0; f < b; f++){
            if (findNum(boards[f], nums[i]))
                winner = f;    
        }
        i++;
    }
    if (winner != -1)
        return calculateScore(boards[winner], nums[--i]);
    else
        return winner;
}

int main(int n, char *args[n]){
    board boards[400];
    board boards2[400];
    int nums[400];
    int b = readFile(args[1], 400, boards, 400, nums) + 1;
    readFile(args[1], 400, boards2, 400, nums);
    int s = findWinningScore(b, boards, nums);
    int l = findLastWinningScore(b, boards2, nums);
    printf("Winnng Score: %d\n", s);
    printf("Last Winners Score: %d\n", l);
    freeBoards(b, boards);
    freeBoards(b, boards2);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct Pos{
    int val;
    bool flashed;
} pos;

void freeGrid(pos* grid[10][10]){
    for (int i = 0; i < 10; i++){
        for (int f = 0; f < 10; f++){
            free(grid[i][f]);
        }
    }   
}
void printGrid(pos* grid[10][10]){
    for (int i = 0; i < 10; i++){
        for (int f = 0; f < 10; f++){
            printf("%d", grid[i][f]->val);
        }
        printf("\n");
    }
    printf("\n");
}
int computeStepAtPos(int row, int col, pos* grid[10][10]){
    if (grid[row][col]->val >= 9 && !grid[row][col]->flashed){
        grid[row][col]->flashed = true;
        grid[row][col]->val = 0;
        int total = 1;
        int startX = col == 0 ? col : col - 1; int endX = col == 9 ? col : col + 1;
        int startY = row == 0 ? row : row - 1; int endY = row == 9 ? row : row + 1;
        for (int i = startY; i <= endY; i++){
            for (int f = startX; f <= endX; f++){
                if (!grid[i][f]->flashed)
                    total += computeStepAtPos(i, f, grid);
            }
        }
        return total;
    }
    else{
        grid[row][col]->val++;
        return 0;
    }
}
int computeStep(pos* grid[10][10]){
    int flashes = 0;
    for (int i = 0; i < 10; i++){
        for (int f = 0; f < 10; f++){
            if (!grid[i][f]->flashed)
                flashes += computeStepAtPos(i, f, grid);
        }
    }
    for (int i = 0; i < 10; i++){
        for (int f = 0; f < 10; f++){
            if (grid[i][f]->flashed)
                grid[i][f]->flashed = false;
        }
    }
    return flashes;
}
int computeNsteps(int n, pos* grid[10][10]){
    int flashes = 0;
    for (int i = 0; i < n; i++)
        flashes += computeStep(grid);
    return flashes;
}
int stepsUntilSynchronised(pos* grid[10][10]){
    int flashes = 0;
    int steps = 0;
    while (flashes != 100){
        flashes = computeStep(grid);
        steps++;
    }
    return steps;
}
void readFile(char* filename, pos* grid[10][10]){
    FILE *in = fopen(filename, "r");
    char c = fgetc(in);
    int row = 0;
    int col = 0;
    while(!feof(in)){
        if (c != '\n'){
            pos *p = malloc(sizeof(pos));
            p->val = c - '0';
            p->flashed = false;
            grid[row][col++] = p;
        }
        else{
            row++;
            col = 0;
        }
        c = fgetc(in);
    }
    fclose(in);
}
int main(int n, char *args[n]){
    pos* g[10][10];
    int steps = 100;
    readFile(args[1], g);
    printGrid(g);
    int t = computeNsteps(steps, g);
    printf("After %d steps there were %d flashes\n", steps, t);
    t = stepsUntilSynchronised(g) + 100; // hacky method - assumes they won't be synchronised after 100 steps
    printf("After %d steps all flashes are synchronised\n", t);
    freeGrid(g);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct Coord{
    int x;
    int y;
} coord;

typedef coord* grid[900];

void freeGrid(grid g){
    for (int i = 0; i < 900; i++){
        if (g[i] != NULL)
            free(g[i]);
    }
}
void initGrid(grid g){
    for (int i = 0; i < 900; i++){
        g[i] = NULL;
    }
}
int readFile(char* filename, grid g, int folds[10]){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    bool readingCoords = true;
    int f = 0;
    int i = 0;
    while (!feof(in)){
        if (strcmp(buffer, "\n") == 0){
            readingCoords = false;
            fgets(buffer, sizeof(buffer), in);
        }
        if (readingCoords){
            char* x = strtok(buffer, ",\n");
            char* y = strtok(NULL, ",\n");
            coord *c = malloc(sizeof(coord));
            c->x = atoi(x);
            c->y = atoi(y);
            g[i++] = c;
        }
        else{
            strtok(buffer, " ");
            strtok(NULL, " ");
            char* fold = strtok(NULL, " ");
            char* dir = strtok(fold, "=");
            char* val = strtok(NULL, "=");
            // If Y - fold val becomes -ve
            int v = (dir[0] == 'y') ? atoi(val) * -1 : atoi(val);
            folds[f++] = v;                  
        }
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return f;
}
int calculateNewCoord(int fold, int val){
    int diff = val - fold;
    return fold - diff;
}
bool checkForOverlaps(grid g, int x, int y){
    for (int i = 0; i < 900; i++){
        if (g[i] != NULL){
            if (g[i]->x == x && g[i]->y == y){
                return true;
            }
        }
    }
    return false;
}
int foldCoord(grid g, int fold, int i){
    int x = g[i]->x;
    int y = g[i]->y;
    int newX = x;
    int newY = y;
    if (fold >= 0){ // Vertical fold
        if (x < fold) return 0;
        newX = calculateNewCoord(fold, x);
    }
    else{ // Horizontal fold
        if (y < abs(fold)) return 0;
        newY = calculateNewCoord(abs(fold), y);
    }
    if (checkForOverlaps(g, newX, newY)){
        free(g[i]);
        g[i] = NULL;
    }
    else{
        g[i]->x = newX;
        g[i]->y = newY;
    }
    return 0;
}
void performFold(grid g, int fold){
    for (int i = 0; i < 900; i++){
        if (g[i] != NULL)
            foldCoord(g, fold, i);
    }
}
void performFolds(grid g, int start, int f, int folds[10]){
    for (int i = start; i < f; i++)
        performFold(g, folds[i]);
}
int countDots(grid g){
    int t = 0;
    for (int i = 0; i < 900; i++){
        if (g[i] != NULL) t++;
    }
    return t;
}
void printGrid(grid g){
    for (int i = 0; i < 7; i++){
        for (int f = 0; f < 50; f++){
            bool printHash = false;
            for (int j = 0; j < 900 && !printHash; j++){
                if (g[j] != NULL)
                    if (g[j]->x == f && g[j]->y == i) printHash = true;
            }
            if (printHash) printf("#");
            else printf(".");
        }
        printf("\n");
    }
}
int main(int n, char *args[n]){
    grid g;
    initGrid(g);
    int folds[20];
    int f = readFile(args[1], g, folds);
    performFold(g, folds[0]);
    int t = countDots(g);
    printf("After 1 fold there was %d dots\n", t);
    performFolds(g, 1, f, folds);
    printGrid(g);
    freeGrid(g);
}

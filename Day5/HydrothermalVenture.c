#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Line {
    int x1;
    int y1;
    int x2;
    int y2;
} line;

#define MAX_X 1000
#define MAX_Y 1000

typedef int grid[MAX_Y][MAX_X];

void initGrid(grid g){
    for (int i = 0; i < MAX_Y; i++){
        for (int f = 0; f < MAX_X; f++){
            g[i][f] = 0;
        }
    }
}
void freeLines(int l, line* ls[l]){
    for (int i = 0; i < l; i++)
        free(ls[i]);
}
int max(int a, int b){
    if (a > b) return a;
    return b;
}
int min(int a, int b){
    if (a > b) return b;
    return a;
}
int readFile(char* filename, line* ls[]){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    int x = 0;
    while (!feof(in)){
        char* lStr = strtok(buffer, ", -> ");
        line *l = malloc(sizeof(line));
        l->x1 = atoi(lStr);
        lStr = strtok(NULL, ", -> ");
        l->y1 = atoi(lStr);
        lStr = strtok(NULL, ", -> ");
        l->x2 = atoi(lStr);
        lStr = strtok(NULL, ", -> ");
        l->y2 = atoi(lStr);
        ls[x++] = l;
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return x;
}
void printLines(int l, line* lines[l]){
    for (int i = 0; i < l; i++){
        line *ltmp = lines[i];
        printf("%d, %d -> %d, %d\n", ltmp->x1, ltmp->y1, ltmp->x2, ltmp->y2);
    }
}
void printFloorMap(int maxX, int maxY, grid fm){
    for (int y = 0; y < maxY; y++){
        for (int x = 0; x < maxX; x++){
            printf("%2d", fm[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}
void generateFloorMap(int l, line* ls[l], grid fm){
    for (int i = 0; i < l; i++){
        line *ltmp = ls[i];
        int start = ltmp->x2;
        int end = ltmp->x1;
        if (ltmp->x1 == ltmp->x2){
            start = min(ltmp->y1, ltmp->y2);
            end = max(ltmp->y1, ltmp->y2);
            for (int f = start; f <= end; f++){
                fm[f][ltmp->x1]++;
            }
        }
        else if (ltmp->y1 == ltmp->y2){
            start = min(ltmp->x1, ltmp->x2);
            end = max(ltmp->x1, ltmp->x2);
            for (int f = start; f <= end; f++){
                fm[ltmp->y1][f]++;
            }
        }
    }
}
void generateFloorMap2(int l, line* ls[l], grid fm){
    for (int i = 0; i < l; i++){
        line *ltmp = ls[i];
        int start = ltmp->x2;
        int end = ltmp->x1;
        int grad = 0;
        // printf("%d, %d -> %d, %d\n", ltmp->x1, ltmp->y1, ltmp->x2, ltmp->y2);
        if (ltmp->x1 != ltmp->x2 && ltmp->y1 != ltmp->y2){
            int startY = ltmp->y2;
            int endY = ltmp->y1;
            grad = (ltmp->y1 - ltmp->y2) / (ltmp->x1 - ltmp->x2);
            if (ltmp->x1 < ltmp->x2){
                start = ltmp->x1;
                end = ltmp->x2;
                startY = ltmp->y1;
                endY = ltmp->y2;
            }
            // printf("startX: %d, StartY: %d, EndX: %d, EndY: %d\n", start, startY, end, endY);
            for (int f = 0; f <= (end - start); f++){
                fm[startY + (f * grad)][start + f]++;
            }
        }
        else if (ltmp->x1 == ltmp->x2){
            start = min(ltmp->y1, ltmp->y2);
            end = max(ltmp->y1, ltmp->y2);
            for (int f = start; f <= end; f++){
                fm[f][ltmp->x1]++;
            }
        }
        else if (ltmp->y1 == ltmp->y2){
            start = min(ltmp->x1, ltmp->x2);
            end = max(ltmp->x1, ltmp->x2);
            for (int f = start; f <= end; f++){
                fm[ltmp->y1][f]++;
            }
        }
    }
}
int countIntersections(grid fm){
    int intersections = 0;
    for (int i = 0; i < 1000; i++){
        for (int f = 0; f < 1000; f++)
            if (fm[i][f] > 1) intersections++;
    }
    return intersections;
}
int main(int n, char *args[n]){
    grid floorMap;
    line* lines[500];
    int l = readFile(args[1], lines);
    initGrid(floorMap);
    generateFloorMap(l, lines, floorMap);
    // printFloorMap(10, 10, floorMap);
    int i = countIntersections(floorMap);
    printf("Hydrothermal Vent intersections: %d\n", i);

    initGrid(floorMap);
    generateFloorMap2(l, lines, floorMap);
    // printFloorMap(10, 10, floorMap);
    i = countIntersections(floorMap);
    printf("Hydrothermal Vent intersections (with diagonals): %d\n", i);
    freeLines(l, lines);
}

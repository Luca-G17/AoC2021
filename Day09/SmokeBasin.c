#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Loc {
    int val;
    bool traversed;
    int lowPointX;
    int lowPointY;
} loc;

typedef loc* heightMap[100][102];
void freeHeightMap(int r, heightMap h){
    for (int i = 0; i < r; i++){
        int f;
        for (f = 0; h[i][f] != NULL; f++){
            free(h[i][f]);
        }
        // free(h[i][f + 1]);
    }
}
void printHeightMap(int r, heightMap h){
    for (int row = 0; row < r; row++){
        for (int col = 0; h[row][col] != NULL; col++)
            printf("%d", h[row][col]->val);
        printf("\n");
    }
}
int readFile(char* filename, heightMap h){
    FILE *in = fopen(filename, "r");
    char x = fgetc(in);
    int row = 0;
    int col = 0;
    while (!feof(in)){
        if (x != '\n'){
            loc *l = malloc(sizeof(loc));
            l->traversed = false;
            l->val = x - '0';
            h[row][col++] = l;
        }
        else{
            h[row][col] = NULL;
            row++;
            col = 0;
        }
        x = fgetc(in);
    }
    fclose(in);
    return row;
}
int min(int a, int b, bool eq){
    if (eq)
        if (a == b) return -1;
    if (a < b) return a;
    return b;
}
void initPointsFound(int r, int pointsFound[r][102]){
    for (int row = 0; row < r; row++){
        for (int col = 0; col < 101; col++){
            pointsFound[row][col] = 0;
        } 
    }
}
int findLocalMin(int x, int row, int col, int r, heightMap h, bool eq){
    int minX = x; 
    if (row != 0) minX = min(minX, h[row - 1][col]->val, eq);     
    if (row != r - 1) minX = min(minX, h[row + 1][col]->val, eq);
    if (col != 0) minX = min(minX, h[row][col - 1]->val, eq);
    if (h[row][col + 1] != NULL) minX = min(minX, h[row][col + 1]->val, eq);
    return minX;
}
int findNearestMin(int row, int col, int r, heightMap h, int pointsFound[r][102]){
    int x = h[row][col]->val;
    loc* route[100];
    int f = 0;
    bool minFound = false;
    int minX = x;
    if (x != 9) route[f++] = h[row][col];
    while (!minFound){
        x = minX;
        minX = findLocalMin(minX, row, col, r, h, false);
        if (row != 0) if (minX == h[row - 1][col]->val) row--;
        if (row != r - 1) if (minX == h[row + 1][col]->val) row++;
        if (col != 0) if (minX == h[row][col - 1]->val) col--;
        if (h[row][col + 1] != NULL) if (minX == h[row][col + 1]->val) col++;
        if (!h[row][col]->traversed) route[f++] = h[row][col];
        if (minX == x) minFound = true;
    }
    x = findLocalMin(minX, row, col, r, h, true);
    if (x != -1) {
        pointsFound[row][col] = 1;
        for (int i = 0; i < f; i++){
            route[i]->lowPointX = col;
            route[i]->lowPointY = row;
            route[i]->traversed = true;
        }
    }
    return minX;
}
int sumMinPoints(int r, heightMap h, int pointsFound[r][102]){
    
    int total = 0;
    int count = 0;
    for (int row = 0; row < r; row++){
        for (int col = 0; h[row][col] != NULL; col++)
            findNearestMin(row, col, r, h, pointsFound);
    }
    for (int row = 0; row < r; row++){
        for (int col = 0; h[row][col] != NULL; col++){
            if (pointsFound[row][col] > 0){
                total += h[row][col]->val;
                count++;
            }
        } 
    }
    return total + count;
}
int max(int a, int b){
    if (a > b) return a;
    return b;
}
int productThreeLargest(int r, heightMap h, int pointsFound[r][102]){
    int maxX = 0;
    int maxY = 0;
    int prod = 1;
    for (int i = 0; i < 3; i++){
        int max = 0;
        for (int row = 0; row < r; row++){
            for (int col = 0; h[row][col] != NULL; col++){
                int cur = pointsFound[row][col] - 1;
                if (cur > max){
                    max = cur;
                    maxY = row;
                    maxX = col;
                }
            }
        }
        prod *= max;
        pointsFound[maxY][maxX] = 0;
    }
    return prod;
}
int findBasins(int r, heightMap h, int pointsFound[r][102]){
    for (int row = 0; row < r; row++){
        for (int col = 0; h[row][col] != NULL; col++){
            loc *l = h[row][col];
            if (l->traversed){
                pointsFound[l->lowPointY][l->lowPointX] += 1;
            }
        }
    }
    return productThreeLargest(r, h, pointsFound);
}
int main(int n, char *args[n]){
    heightMap h;
    int r = readFile(args[1], h);
    int pointsFound[r][102];
    initPointsFound(r, pointsFound);
    // printHeightMap(r, h);
    int t = sumMinPoints(r, h, pointsFound);
    printf("The sum total of the heights of all the min points is %d\n", t);
    t = findBasins(r, h, pointsFound);
    printf("The product of the three largest basin areas is %d\n", t);
    freeHeightMap(r, h);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

typedef struct Point{
    int x;
    int y;
    int weight;
    int rank;
    int gCost;
    struct Point* parent;
    bool goal;
} point;

#define GRID_X 500
#define GRID_Y 500
#define MAX_SIZE 500500

typedef point* grid[GRID_Y][GRID_X];

void initGrid(grid g){
    for (int i = 0; i < GRID_Y; i++){
        for (int f = 0; f < GRID_X; f++)
            g[i][f] = NULL;
    }
}
void freeGrid(int r, grid g){
    for (int i = 0; i < r; i++){
        for (int f = 0; f < r; f++)
            free(g[i][f]);
    }
}
int readFile(char* filename, grid g){
    FILE *in = fopen(filename, "r");
    char c = fgetc(in);
    int row = 0;
    int col = 0;
    while (!feof(in)){
        if (c != '\n'){
            point *p = malloc(sizeof(point));
            p->rank = 0;
            p->weight = c - '0';
            p->goal = false;
            p->gCost = 0;
            p->parent = NULL;
            p->x = col;
            p->y = row;
            g[row][col++] = p;
        }
        else{
            col = 0;
            row++;
        }
        c = fgetc(in);
    }   
    fclose(in);
    return row; 
}
int readFile2(char* filename, grid g){
    FILE *in = fopen(filename, "r");
    char c = fgetc(in);
    int row = 0;
    int col = 0;
    while (!feof(in)){
        if (c != '\n'){
            point *p = malloc(sizeof(point));
            p->rank = 0;
            p->weight = c - '0';
            p->goal = false;
            p->gCost = 0;
            p->parent = NULL;
            p->x = col;
            p->y = row;
            g[row][col++] = p;
        }
        else{
            col = 0;
            row++;
        }
        c = fgetc(in);
    }   
    fclose(in);
    return row; 
}
void printGrid(int r, grid g){
    for (int i = 0; i < r; i++){
        for (int f = 0; f < r; f++)
            printf("%d", g[i][f]->weight);
        printf("\n");
    }
}
int parent(int i){
    return (i - 1) / 2;
}
int leftChild(int i){
    return 2 * i + 1;
}
int rightChild(int i){
    return 2 * i + 2;
}
void swap(point *x, point *y){
    point tmp = *x;
    *x = *y;
    *y = tmp;    
}
void insert(point* a[], point* data, int *n){
    if (*n >= MAX_SIZE){
        printf("The heap is full, cannot insert");
        return;
    }
    a[*n] = data;
    *n = *n + 1;

    int i = *n - 1;
    while (i != 0 && a[parent(i)]->rank > a[i]->rank){
        swap(a[parent(i)], a[i]);
        i = parent(i);
    }
}
void minHeapify(point* a[], int i, int n){
    int left = leftChild(i);
    int right = rightChild(i);
    int smallest = i;
    if (left < n && a[left]->rank < a[smallest]->rank){
        smallest = left;
    }
    if (right < n && a[right]->rank < a[smallest]->rank){
        smallest = right;
    }
    if (smallest != i){
        point* tmp = a[i];
        a[i] = a[smallest];
        a[smallest] = tmp;
        minHeapify(a, smallest, n);
    }
}
void buildMinHeap(point* a[], int n){
    int i;
    for (i = n / 2; i >= 0; i--){
        minHeapify(a, i, n);
    }
}
point *getMin(point* a[]){
    return a[0];
}
point *extractMin(point* a[], int *n){
    point* minItem = a[0];
    a[0] = a[*n - 1];
    *n = *n - 1;
    minHeapify(a, 0, *n);
    return minItem;
}
void removeAt(point* a[], int *n, int i){
    free(a[i]);
    for (int f = i; f < *n - 1; f++){
        a[f] = a[f + 1];
    }
    *n -= 1;
}
int computeGridHash(point* p){
    return (p->x * 1000 + p->y);
}
void removeFromHashSetAt(point* a[], point* p){
    int i = computeGridHash(p);
    free(a[i]);
    a[i] = NULL;
}
int heuristic(int x, int y, int tx, int ty){
    int dx = (x - tx);
    int dy = (y - ty);
    return sqrt(dx * dx + dy * dy);
}
void print_heap(point* a[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("(%d, %d) ", a[i]->x, a[i]->y);
    }
    printf("\n");
}
bool comparePoints(point* p1, point* p2){
    if (p1->x == p2->x && p1->y == p2->y)
        return true;
    return false;
}

void neighborCost(point* neighbor, point* curr, point* closed[], int *m, point* open[], int *n, point* target){
    int cost = curr->gCost + neighbor->weight;
    bool isOpen = false;
    bool isClosed = false;
    for (int i = 0; i < *n; i++){
        if (comparePoints(neighbor, open[i])){
            if (cost < neighbor->gCost)
                removeAt(open, n, i);
            else
                isOpen = true;
        }
    }
    int addr = computeGridHash(neighbor);
    if (addr >= 0 && addr <= 500500){
        if (closed[addr] != NULL){
            if (cost < neighbor->gCost)
                removeFromHashSetAt(closed, neighbor);
            else
                isClosed = true;
        }
    }
    if (!isOpen && !isClosed){
        point *p = malloc(sizeof(point));
        p->gCost = cost;
        p->parent = curr;
        p->rank = cost + heuristic(neighbor->x, neighbor->y, target->x, target->y);
        p->weight = neighbor->weight;
        p->x = neighbor->x;
        p->y = neighbor->y;
        insert(open, p, n);
        neighbor->gCost = cost;
        neighbor->parent = curr;
    }
}
void freeQueue(point* a[], int n){
    for (int i = 0; i < n; i++){
        if (a[i] != NULL)
            free(a[i]);
    }
}
void buildClosed(point* closed[]){
    for (int i = 0; i < MAX_SIZE; i++)
        closed[i] = NULL;
}
int pathfinding(point* target, grid g){
    point** open = malloc(sizeof(point) * MAX_SIZE);
    point** closed = malloc(sizeof(point) * MAX_SIZE);
    buildClosed(closed);
    int n = 0;
    int m = 0;
    point *targetcpy = malloc(sizeof(point));
    targetcpy->x = target->x;
    targetcpy->y = target->y;
    point *start = malloc(sizeof(point));
    start->x = 0;
    start->y = 0;
    start->weight = 1;
    start->gCost = 0;
    start->rank = 0;
    start->parent = NULL;
    open[n++] = start;

    // int pCounts = 0;
    buildMinHeap(open, n);
    point* curr = NULL;
    while (!comparePoints(getMin(open), targetcpy)){
        curr = extractMin(open, &n);
        /*
        pCounts = (pCounts + 1) % 10000;
        if (pCounts == 9999)
            printf("+10000 iterations\n");
        */
        closed[computeGridHash(curr)] = curr;
        int cX = curr->x;
        int cY = curr->y;
        if (cY != 0) neighborCost(g[cY - 1][cX], curr, closed, &m, open, &n, targetcpy);
        if (cY != targetcpy->y) neighborCost(g[cY + 1][cX], curr, closed, &m, open, &n, targetcpy);
        if (cX != 0) neighborCost(g[cY][cX - 1], curr, closed, &m, open, &n, targetcpy);
        if (cX != targetcpy->x) neighborCost(g[cY][cX + 1], curr, closed, &m, open, &n, targetcpy);
    }
    int total = 0;
    int o = 0;
    curr = g[targetcpy->y][targetcpy->x];
    point* route[MAX_SIZE];
    route[o++] = curr;
    while (curr->x != 0 && curr->y != 0){
        total += curr->weight;
        route[o++] = curr->parent;
        curr = curr->parent;
    }
    free(targetcpy);
    freeQueue(open, n);
    freeQueue(closed, MAX_SIZE);
    free(closed);
    free(open);
    return total;
}
void generateLargerGrid(grid g){
    int rowLen = 100;
    for (int i = 0; i < 5; i++){
        for (int f = 0; f < 5; f++){
            for (int j = 0; j < 100; j++){
                for (int k = 0; k < 100; k++){
                    point *p = malloc(sizeof(point));
                    int newWeight = (g[j][k]->weight + i + f);
                    if (newWeight > 9) newWeight = (newWeight % 10) + 1;
                    p->weight = newWeight;
                    p->x = k + rowLen * f;
                    p->y = j + rowLen * i;
                    p->parent = NULL;
                    p->gCost = 0;
                    if (i == 0 && f == 0)
                        free(g[j][k]);
                    g[j + (rowLen * i)][k + (rowLen * f)] = p;
                }
            }
        }
    }
}
int main(int n, char *args[n]){
    grid g;
    int r = readFile(args[1], g);
    clock_t begin = clock();
    int t = pathfinding(g[99][99], g);
    clock_t end = clock();
    double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("(100x100) The lowest risk path has a total risk value of %d (execution time: %lf)\n", t, timeSpent);
    
    grid g2;
    int r2 = readFile2(args[1], g2);
    generateLargerGrid(g2);
    begin = clock();
    t = pathfinding(g2[499][499], g2);
    end = clock();
    timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("(500x500) The lowest risk path has a total risk value of %d (execution time: %lf)\n", t + 13, timeSpent);

    freeGrid(r2 * 5, g2);
    freeGrid(r, g);
}

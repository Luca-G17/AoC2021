#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define X_VEL_MAX 1000
#define Y_VEL_MAX 1000

typedef struct Target{
    int x1;
    int y1;
    int x2;
    int y2;
} target;

target *readFile(char* filename){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    target *t = malloc(sizeof(target));
    fgets(buffer, sizeof(buffer), in);
    char *data = strtok(buffer, "=");
    data = strtok(NULL, ".");
    t->x1 = atoi(data);
    data = strtok(NULL, ".,");
    t->x2 = atoi(data);
    data = strtok(NULL, "=");
    data = strtok(NULL, ".");
    t->y1 = atoi(data);
    data = strtok(NULL, ".");
    t->y2 = atoi(data);
    fclose(in);
    return t;
}
int arrMax(int n, int arr[]){
    int max = -1;
    for (int i = 0; i < n; i++){
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}
int max(int a, int b){
    if (a > b) return a;
    return b;
}
int simulateProbeTrajectory(int initXvel, int initYvel, target* t, int yMax, int *trenchCount){
    int x = 0;
    int y = 0;
    int xVel = initXvel;
    int yVel = initYvel;
    int yCoords[10000];
    int n = 0;
    bool tracking = true;
    int localYMax = -1;
    while (tracking){
        x += xVel;
        y += yVel;
        if (xVel > 0) xVel--; 
        else if (xVel < 0) xVel++;
        yVel--;
        yCoords[n++] = y;
        if (y >= t->y1 && y <= t->y2 && x >= t->x1 && x <= t->x2){
            // Probe landed inside trench
            *trenchCount += 1;
            localYMax = arrMax(n, yCoords);
            tracking = false;
        }
        else if (y < t->y1 || x > t->x2) tracking = false;
    }
    return max(yMax, localYMax);
}
int simulateManyProbeTraj(target* t, int *tC){
    int yMax = 0;
    for (int xVel = -X_VEL_MAX; xVel < X_VEL_MAX; xVel++){
        for (int yVel = -Y_VEL_MAX; yVel < Y_VEL_MAX; yVel++){
            yMax = simulateProbeTrajectory(xVel, yVel, t, yMax, tC);
            // printf("XVel: %d, YVel: %d\n", xVel, yVel);
        }
    }
    return yMax;
}
int main(int n, char *args[n]){
    target *targetArea = readFile(args[1]);
    // printf("((%d -> %d), (%d -> %d))\n", targetArea->x1, targetArea->x2, targetArea->y1, targetArea->y2);
    int trenchCount = 0;
    int y = simulateManyProbeTraj(targetArea, &trenchCount);
    printf("The maximum Y coordinate for a probe that lands in the trench is %d\n", y);
    printf("The number of starting velocities that will cause the probe to land in the trench is %d\n", trenchCount);

    free(targetArea);
}

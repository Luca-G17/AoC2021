#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct pos {
    int x;
    int y;
    int aim;
} pos;

void calculatePos2(char* d, int x, pos *subPos){
    if (strcmp(d, "forward") == 0){
        subPos->x += x;
        subPos->y += x * (subPos->aim);
    }
    else if (strcmp(d, "up") == 0){
        subPos->aim -= x;
    }
    else{
        subPos->aim += x;
    }
}
void calculatePos1(char* d, int x, pos *subPos){
    if (strcmp(d, "forward") == 0){
        subPos->x += x;
    }
    else if (strcmp(d, "up") == 0)
        subPos->y -= x;
    else
        subPos->y += x;
}
void initPos(pos *subPos){
    subPos->x = 0;
    subPos->y = 0;
    subPos->aim = 0;
}

void readFile(char* filename){
    FILE *in = fopen(filename, "r");
    pos *subPos1 = malloc(sizeof(pos));
    pos *subPos2 = malloc(sizeof(pos));
    initPos(subPos1);
    initPos(subPos2);

    char line[256];
    fgets(line, sizeof(line), in);
    while(!feof(in)){
        char* dir = strtok(line, " \n");
        int val = atoi(strtok(NULL, " \n"));
        calculatePos1(dir, val, subPos1);
        calculatePos2(dir, val, subPos2);
        fgets(line, sizeof(line), in);
    }
    fclose(in);
    printf("Position Product 1: %d\n", subPos1->x * subPos1->y);
    printf("Position Product 2: %d\n", subPos2->x * subPos2->y);
    free(subPos1);
    free(subPos2);
}

int main(int n, char *args[n]){
    readFile(args[1]);
}

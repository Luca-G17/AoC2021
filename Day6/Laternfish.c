#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct LaternfishWave {
    long count;
    int age;
} wave;

void freeWaves(int w, wave* ws[], wave* hld[]){
    for (int i = 0; i < w; i++)
        free(ws[i]);
    
    for (int i = 0; i < 2; i++){
        if (hld[i] != NULL)
            free(hld[i]);
    }
}
int readFile(char* filename, wave* nums[]){
    FILE *in = fopen(filename, "r");
    char buffer[1000];
    fgets(buffer, sizeof(buffer), in);
    char* num = strtok(buffer, ",");
    int x = 0;
    while (num != NULL){
        int n = atoi(num);
        bool numExists = false;
        for (int i = 0; i < x; i++){
            if (nums[i]->age == n){
                nums[i]->count++;
                numExists = true;
            }
        }
        if (!numExists){
            wave *w = malloc(sizeof(wave));
            w->age = n;
            w->count = 1;
            nums[x++] = w;
        }
        num = strtok(NULL, ",");
    }
    fclose(in);
    return x;
}
int computeDay(int w, wave* ws[], wave* holding[]){
    int newW = w;
    int x = -1;
    for (int i = 0; i < w; i++){
        if (ws[i]->age == 0){
            /*
            
            */
            x = i;
            ws[i]->age = 6;
        }
        else
            ws[i]->age--;
    }
    
    if (holding[0] != NULL)
        holding[0]->age--;
    if (holding[1] != NULL)
        holding[1]->age--;
    long holdingCount = 0;
    if (holding[0] != NULL){
        if (holding[0]->age < 7){
            bool ageExists = false;
            for (int f = 0; f < w; f++){
                if (ws[f]->age == holding[0]->age){
                    ageExists = true;
                    ws[f]->count += holding[0]->count;
                    holdingCount = holding[0]->count;
                    free(holding[0]);
                    f = w;
                }
            }
            if (!ageExists){
                ws[newW++] = holding[0];
            }
            holding[0] = holding[1];
            holding[1] = NULL;
        }
    }
    if (x != -1){
        wave *nw = malloc(sizeof(wave));
        nw->age = 8;
        nw->count = ws[x]->count - holdingCount;
        if (holding[0] == NULL){
            holding[0] = nw;
        }
        else
            holding[1] = nw;
    }

    return newW;
}
long countFish(int w, wave* ws[], wave* hld[2]){
    long total = 0;
    for (int i = 0; i < w; i++)
        total += ws[i]->count;
    for (int i = 0; i < 2; i++){
        if (hld[i] != NULL)
            total += hld[i]->count;
    }
    return total;
}
void printFishCounts(int w, wave* ws[], wave* hld[2]){
    for (int i = 0; i < w; i++){
        printf("There are %2ld fish aged %d\n", ws[i]->count, ws[i]->age);
    }
    for (int i = 0; i < 2; i++){
        if (hld[i] != NULL)
            printf("There are %2ld fish aged %d\n", hld[i]->count, hld[i]->age);
    }
}
int simulateDays(int d, int w, wave* ws[], wave* hld[]){
    int newN = w;
    hld[0] = NULL; hld[1] = NULL;
    for (int i = 0; i < d; i++){
        newN = computeDay(newN, ws, hld);
        // printf("Day %d\n", i + 1);
        // printFishCounts(newN, ws, hld);
    }
    return newN;
}
int main(int n, char *args[n]){
    wave* waves[100];
    wave* hld[2];
    int x = readFile(args[1], waves);
    int days = 256;
    // int fish = simulateDaysWithPrint(days, x, ns);
    int w = simulateDays(days, x, waves, hld);
    long fish = countFish(w, waves, hld);
    printf("After %d days there are %ld Laternfish\n", days, fish);
    freeWaves(x, waves, hld);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef char line[16][10];

int readFile(char* filename, int l, line lines[l]){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    int x = 0;
    while(!feof(in)){
        char* signal = strtok(buffer, " \n");
        int word = 0;
        while (signal != NULL){
            strcpy(lines[x][word++], signal);
            signal = strtok(NULL, " \n");
        }
        x++;
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return x;
}
void printLines(int l, int x, line ls[x]){
    for (int i = 0; i < l; i++){
        for (int w = 0; w < 15; w++){
            printf("%s ", ls[i][w]);
        }
        printf("\n");
    }
}
int findLength(int len, int start, line l){
    for (int i = start; i < 10; i++)
        if (len == strlen(l[i])) return i;
    return -1;
}
void initMap(char arr[10][10]){
    for (int i = 0; i < 10; i++)
        strcpy(arr[i], "-");
}
bool contains(char* str1, char* str2){
    for (int i = 0; i < strlen(str2); i++){
        bool containsLetter = false;
        for (int f = 0; f < strlen(str1); f++){
            if (str2[i] == str1[f])
                containsLetter = true;
        }
        if (!containsLetter)
            return false;
    }
    return true;
}
void commonLetters(char out[10], char* str1, char* str2){
    int j = 0;
    for (int i = 0; i < strlen(str1); i++){
        for (int f = 0; f < strlen(str2); f++){
            if (str1[i] == str2[f])
                out[j++] = str1[i];
        }
    }
    out[j] = '\0';
}
void generateMapping(char map[10][10], line l){
    char c[10];
    char c1[2] = " \0";
    char c2[2] = " \0";
    initMap(map);
    strcpy(map[1], l[findLength(2, 0, l)]);
    strcpy(map[8], l[findLength(7, 0, l)]);
    strcpy(map[4], l[findLength(4, 0, l)]);
    for (int i = 0; i < 10; i++){
        if (strlen(l[i]) == 6){
            c1[0] = map[1][0]; 
            c2[0] = map[1][1];
            if (contains(l[i], c1) != contains(l[i], c2)) strcpy(map[6], l[i]);
            else if (contains(l[i], map[4])) strcpy(map[9], l[i]);
            else strcpy(map[0], l[i]);
        }
    }
    for (int i = 0; i < 10; i++){
        int len = strlen(l[i]);
        switch (len){
            case 3: strcpy(map[7], l[i]); break;
            case 5:
                if (contains(l[i], map[1])) strcpy(map[3], l[i]);
                else{
                    commonLetters(c, map[6], map[9]);
                    if (contains(l[i], c)) strcpy(map[5], l[i]);
                    else
                        strcpy(map[2], l[i]);
                }
                break;
        }
    }
}
int primeProduct(char* num){
    int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
                     37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
                     79, 83, 89, 97, 101 };
    int prod = 1;
    for (int i = 0; num[i] != '\0'; i++)
        prod *= primes[num[i] - 'a'];
    return prod;
}
void primeProducts(int start, int end, char nums[][10], int products[]){
    for (int i = start; i < end; i++){
        products[i - start] = primeProduct(nums[i]);
    }
}
int decodeOutput(line l){
    char map[10][10];
    int mapProducts[10];
    int lineProducts[10];
    generateMapping(map, l);
    primeProducts(0, 10, map, mapProducts);
    primeProducts(11, 15, l, lineProducts);
    int total = 0;
    for (int i = 0; i < 4; i++){
        for (int f = 0; f < 10; f++){
            if (mapProducts[f] == lineProducts[i])
                total += f * pow(10, (3 - i));
        }
    }
    return total;
}
int sumOutputs(int l, line ls[]){
    int total = 0;
    for (int i = 0; i < l; i++){
        total += decodeOutput(ls[i]);
    }
    return total;
}
int digitCount(int l, line ls[]){
    int total = 0;
    for (int i = 0; i < l; i++){
        for (int f = 11; f < 15; f++){
            int s = strlen(ls[i][f]);
            if (s == 2 || s == 4 || s == 7 || s == 3)
                total += 1;
        }
    }
    return total;
}
void printMap(char map[10][10]){
    for (int i = 0; i < 10; i++)
        printf("%s\n", map[i]);
}
int main(int n, char *args[n]){
    int lMax = 200;
    line lines[lMax];
    int l = readFile(args[1], lMax, lines);
    // printLines(l, lMax, lines);
    int c = digitCount(l, lines);
    printf("There are %d 1s, 4s, 7s and 8s\n", c);
    int t = sumOutputs(l, lines);
    printf("The sum total of the ouput of each display is %d\n", t);
}

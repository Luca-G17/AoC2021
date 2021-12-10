#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Stack{
    int top;
    char letters[100];
} stack;

int sSize(stack *s){
    return s->top + 1;
}
char sPop(stack *s){
    s->top--;
    return (s->letters[s->top + 1]);
}
void sPush(stack *s, char c){
    s->letters[++s->top] = c;
}
char sPeek(stack *s){
    return (s->letters[s->top]);
}
void sPrint(stack *s){
    for (int i = 0; i <= s->top; i++){
        printf("%c ", s->letters[i]);
    }
    printf("\n");
}
int readFile(char* filename, int l, char strs[l][256]){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    int x = 0;
    while (!feof(in)){
        strcpy(strs[x++], buffer);
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return x;
}
void printLines(int l, char strs[l][256]){
    for (int i = 0; i < l; i++)
        printf("%s", strs[i]);
}
int letterIsInArr(char l, int n, char arr[n]){
    for (int i = 0; i < n; i++)
        if (arr[i] == l) return i;
    return -1;
}
char findIllegalChar(char line[256]){
    char opens[] = { '(', '[', '<', '{' };
    char closes[] = { ')', ']', '>', '}' };
    stack *s = malloc(sizeof(stack));
    s->top = -1;
    for (int i = 0; line[i] != '\n'; i++){
        char l = line[i];
        int x = 0;
        if (letterIsInArr(l, 4, opens) != -1) sPush(s, l);
        if ((x = letterIsInArr(l, 4, closes)) != -1){
            if (sPop(s) != opens[x]){
                free(s);
                return l;
            } 
        }
    }
    free(s);
    return '-';
}
stack *findMissingChars(char line[256]){
    char opens[] = { '(', '[', '<', '{' };
    char closes[] = { ')', ']', '>', '}' };
    stack *s = malloc(sizeof(stack));
    s->top = -1;
    for (int i = 0; line[i] != '\n'; i++){
        char l = line[i];
        int x = 0;
        if (letterIsInArr(l, 4, opens) != -1) sPush(s, l);
           if ((x = letterIsInArr(l, 4, closes)) != -1){
                if (sPop(s) != opens[x]){
                    free(s);
                    return NULL;
                } 
        }
    }
    return(s);
}
long missingCharScore(char line[256]){
    long total = 0;
    stack *s = findMissingChars(line);
    if (s == NULL) return -1;
    int size = sSize(s);
    for (int i = 0; i < size; i++){
        char l = sPop(s);
        total *= 5;
        switch (l){
            case ('('): total += 1;
            break;
            case ('['): total += 2;
            break;
            case ('{'): total += 3;
            break;
            case ('<'): total += 4;
            break;
        }
    }
    free(s);
    return total;
}
void printCharScores(int x, long s[100]){
    for (int i = 0; i < x; i++)
        printf("%ld\n", s[i]);
}
void merge(long arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    long L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(long arr[], int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
int middleMissingCharScore(int l, char lines[l][256]){
    long scores[100];
    int x = 0;
    for (int i = 0; i < l; i++){
        long s = missingCharScore(lines[i]);
        if (s != -1) scores[x++] = s;
    }
    printCharScores(x, scores);
    mergeSort(scores, 0, x - 1);
    printf("\n");
    printCharScores(x, scores);
    return scores[x / 2];
}
int illegalCharScore(int l, char lines[l][256]){
    int total = 0;
    for (int i = 0; i < l; i++){
        char illegalC = findIllegalChar(lines[i]);
        switch (illegalC){
            case (')'):
                total += 3;
                break;
            case (']'):
                total += 57;
                break;
            case ('}'):
                total += 1197;
                break;
            case ('>'):
                total += 25137;
                break;
        }
    }
    return total;
}
int main(int n, char *args[n]){
    char lines[110][256];
    int l = readFile(args[1], 110, lines);
    // printLines(l, lines);
    int t = illegalCharScore(l, lines);
    printf("The total illegal character score is %d\n", t);
    t = middleMissingCharScore(l, lines);
    printf("The middle autocomplete score is %d\n", t);
}

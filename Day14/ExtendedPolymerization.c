#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// PART 1 ---------------------------------------------------------------------------- 
typedef struct Rule{
    char pair[3];
    char insert;
} rule;

typedef struct Elem{
    struct Elem* next;
    char letter;
} elem;

typedef struct List{
    int size;
    struct Elem* current;
    struct Elem* last;
    struct Elem* terminal;
} list;

void resetCurr(list *l){
    l->current = l->terminal;
}
void freeList(list *l){
    int f = 0;
    resetCurr(l);
    while (f < l->size){
        elem *n = l->current->next;
        free(l->current);
        l->current = n;
        f++;
    }
}
list *newList(){
    list *l = malloc(sizeof(list));
    elem *t = malloc(sizeof(list));
    t->letter = '-';
    t->next = NULL;
    l->current = t;
    l->last = t;
    l->terminal = t;
    l->size = 1;
    return l;
}
char getAt(list *l, int i){
    l->current = l->terminal;
    int f = 0;
    while (f != i && f < l->size){
        l->current = l->current->next;
        f++;
    }
    return l->current->letter;
}
void append(list *l, char letter){
    elem *e = malloc(sizeof(elem));
    e->letter = letter;
    e->next = NULL;
    l->last->next = e;
    l->last = e;
    l->size++;
}
char getNext(list *l){
    return l->current->next->letter;
}
char getCurrent(list *l){
    return l->current->letter;
}
void advCurrent(list *l){
    l->current = l->current->next;
}
void insertNext(list *l, char newLetter){
    elem *e = malloc(sizeof(elem));
    e->letter = newLetter;
    e->next = l->current->next;
    l->current->next = e;
    l->size++;
}
void freeRules(rule* rules[]){
    for (int i = 0; rules[i] != NULL; i++)
        free(rules[i]);
}
list *readFile(char *filename, rule* rules[101]){
    list *l = newList();
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    int p = 0;
    char polymer = buffer[p++];
    while (polymer != '\n'){
        append(l, polymer);
        polymer = buffer[p++];    
    }

    fgets(buffer, sizeof(buffer), in);
    fgets(buffer, sizeof(buffer), in);
    int x = 0;
    while (!feof(in)){
        char* r = strtok(buffer, " -> ");
        rule *newRule = malloc(sizeof(rule));
        strcpy(newRule->pair, r);
        r = strtok(NULL, " -> ");
        newRule->insert = r[0];
        rules[x++] = newRule;
        fgets(buffer, sizeof(buffer), in);
    }
    rules[x] = NULL;
    fclose(in);
    return l;
}
void printList(list *l){
    resetCurr(l);
    int f = 1;
    advCurrent(l);
    while (f < l->size){
        printf("%c", l->current->letter);
        advCurrent(l);
        f++;
    }
    printf("\n");
    resetCurr(l);
}
char findInsert(rule* rules[], char c1, char c2){
    for (int i = 0; rules[i] != NULL; i++){
        if (rules[i]->pair[0] == c1 && rules[i]->pair[1] == c2)
            return rules[i]->insert;
    }
    return '-';
}
void computePairInsertionPass(rule* rules[], list *l){
    resetCurr(l);
    advCurrent(l);
    for (int i = 1; i < l->size - 1; i++){
        char c1 = getCurrent(l);
        char c2 = getNext(l);
        char insert = findInsert(rules, c1, c2);
        if (insert != '-'){
            insertNext(l, insert);
            advCurrent(l);
            i++;
        }
        advCurrent(l);
    }
    resetCurr(l);
}
void computeNpasses(rule* rules[], list *l, int n){
    for (int i = 0; i < n; i++){
        computePairInsertionPass(rules, l);
        // printList(l);
        /*
        clock_t begin = clock();
        computePairInsertionPass(rules, l);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Pass: %d, Execution Time: %lf\n", i + 1, time_spent);
        */
    }
}
void initCounts(long counts[26]){
    for (int i = 0; i < 26; i++)
        counts[i] = 0;
}
long arrMax(int n, long counts[n]){
    long max = 0;
    for (int i = 0; i < n; i++){
        if (counts[i] > max)
            max = counts[i];
    }
    return max;
}
long arrMin(int n, long counts[n]){
    long min = LONG_MAX;
    for (int i = 0; i < n; i++){
        if (counts[i] < min && counts[i] > 0)
            min = counts[i];
    }
    return min;
}
void countElems(list *l, long counts[26]){
    initCounts(counts);
    advCurrent(l);
    for (int i = 1; i < l->size; i++){
        counts[getCurrent(l) - 'A']++;
        advCurrent(l);
    }
}
// PART 2 ----------------------------------------------------------------------------
typedef struct Pair {
    char str[3];
    long count;
} pair;

void freePairs(int p, pair* pairs[200]){
    for (int i = 0; i < p; i++)
        free(pairs[i]);
}
int readFile2(char* filename, rule* rules[101], pair* pairs[200]){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);    
    int p = 0;
    int x = 0;
    char p1 = buffer[p];
    char p2 = buffer[p + 1];
    while (p2 != '\n'){
        pair* pa = malloc(sizeof(pair));
        pa->count = 1;
        pa->str[0] = p1;
        pa->str[1] = p2;
        pa->str[2] = '\0';
        pairs[x++] = pa;
        p1 = buffer[++p];
        p2 = buffer[p + 1];
    }

    fgets(buffer, sizeof(buffer), in);
    fgets(buffer, sizeof(buffer), in);
    int y = 0;
    while (!feof(in)){
        char* r = strtok(buffer, " -> ");
        rule *newRule = malloc(sizeof(rule));
        strcpy(newRule->pair, r);
        r = strtok(NULL, " -> ");
        newRule->insert = r[0];
        rules[y++] = newRule;
        fgets(buffer, sizeof(buffer), in);
    }
    rules[y] = NULL;
    fclose(in);
    return x;
}
int checkPairExists(char p[3], int n, pair* pairs[]){
    for (int i = 0; i < n; i++){
        if (strcmp(p, pairs[i]->str) == 0)
            return i;
    }
    return -1;
}
char findRule(rule* rules[], pair* p){
    for (int i = 0; rules[i] != NULL; i++){
        if (strcmp(rules[i]->pair, p->str) == 0)
            return rules[i]->insert;
    }
    return '-';
}
int computePass2(rule* rules[], int p, pair* pairs[], long counts[26]){
    int pCount = p;
    long prevCounts[300];
    for (int i = 0; i < p; i++){
        prevCounts[i] = pairs[i]->count;
        pairs[i]->count = 0; 
    }
    for (int i = 0; i < p; i++){
        char l = findRule(rules, pairs[i]);
        char newP1[3] = { l, pairs[i]->str[1], '\0' };
        char newP2[3] = { pairs[i]->str[0], l, '\0' };
        int x = -1;
        counts[l - 'A'] += prevCounts[i];
        if ((x = checkPairExists(newP1, pCount, pairs)) != -1)
            pairs[x]->count += prevCounts[i];
        else{
            pair *newP = malloc(sizeof(pair));
            newP->count = prevCounts[i];
            strcpy(newP->str, newP1);
            pairs[pCount++] = newP;
        }
        if ((x = checkPairExists(newP2, pCount, pairs)) != -1)
            pairs[x]->count += prevCounts[i];
        else{
            pair *newP = malloc(sizeof(pair));
            newP->count = prevCounts[i];
            strcpy(newP->str, newP2);
            pairs[pCount++] = newP;
        }
    }
    return pCount;
}
int computeNPasses2(int p, pair* pairs[], rule* rules[], long counts[26], int n){
    initCounts(counts);
    counts[1] = 1; counts[2] = 1; counts[13] = 2;
    for (int i = 0; i < n; i++) {
        p = computePass2(rules, p, pairs, counts);
    }
    return p;
}
void printCounts(long counts[26]){
    for (int i = 0; i < 26; i++)
        printf("%ld ", counts[i]);
    printf("\n");
}
void printPairs(int p, pair* pairs[300]){
    for (int i = 0; i < p; i++)
        printf("%s ", pairs[i]->str);
    printf("\n");
}
int main(int n, char *args[n]){
    rule* rules[101];
    long counts[26];
    list *l = readFile(args[1], rules);
    
    int passes = 10;
    computeNpasses(rules, l, passes);
    countElems(l, counts);
    printf("The difference between the most common polymer and the least common polymer after %d passes is %ld\n", passes, arrMax(26, counts) - arrMin(26, counts));

    passes = 40;
    pair* pairs[300];
    rule* rules2[101];
    long counts2[26];
    initCounts(counts2);
    int x = readFile2(args[1], rules2, pairs);
    int p = computeNPasses2(x, pairs, rules2, counts2, passes);
    printf("The difference betweem the most common polymer and the least common polymer after %d passes is %ld\n", passes, arrMax(26, counts2) - arrMin(26, counts2));

    freePairs(p, pairs);
    freeList(l);
    freeRules(rules2);
    freeRules(rules);
}

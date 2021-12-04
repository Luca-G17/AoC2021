#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


int max(int a, int b){
    if (a > b) return a;
    return b;
}
void printArr(int x, int numbers[x]){
    for (int i = 0; i < x; i++)
        printf("%d\n", numbers[i]);
}
int increases(int x, int nums[x]){
    int incs = 0;
    int tmp = nums[0];
    for (int i = 1; i < x; i++){
        if (nums[i] > tmp)
            incs++;
        tmp = nums[i];
    }
    printf("Task 1 - Increases: %d\n", incs);
    return incs;
}
int windowIncreases(int x, int nums[x]){
    int incs = 0;
    int tmp = nums[0] + nums[1] + nums[2];
    for (int i = 3; i < x; i++){
        if ((tmp + nums[i] - nums[i - 3]) > tmp)
            incs++;
        tmp = tmp + nums[i] - nums[i - 3];
    }
    printf("Task 2 - Increases: %d\n", incs);
    return incs;
}
void assert(int line, bool b){
    if (b) return;
    printf("The test on line %d fails.\n", line);
    exit(1);
}

void test(){
    int nums[] = { 200, 208, 210, 200, 207, 240, 269, 260, 263};
    assert(__LINE__, increases(9, nums) == 7);
    printf("All tests pass\n");
}
void readFile(char* filename){
    FILE *in = fopen(filename, "r");
    char c = fgetc(in);
    char str[5];
    int i = 0;
    int x = 0;
    int nums[2001];
    while(!feof(in)){
        if (c != '\n'){
            str[i] = c;
            i++; 
        }
        else {
            i = 0;
            nums[x++] = atoi(str);
        }
        c = fgetc(in);
    }
    fclose(in);
    increases(x, nums);
    windowIncreases(x, nums);
    // printArr(x, nums);
}
int main(int n, char *args[n]){
    if (n == 2)
        readFile(args[1]);
    else if (n == 1){
        test();
    }
}

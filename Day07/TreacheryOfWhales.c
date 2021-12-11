#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define SEARCH_RANGE 150
int readFile(char* filename, int nums[]){
    FILE *in = fopen(filename, "r");
    char buffer[4000];
    fgets(buffer, sizeof(buffer), in);
    char* num = strtok(buffer, ",");
    int x = 0;
    while (num != NULL){
        nums[x++] = atoi(num);
        num = strtok(NULL, ",");
    }
    fclose(in);
    return x;
}
int calculateFuelCost(int t, int n, int nums[], bool part2){
    int total = 0;
    for (int i = 0; i < n; i++){
        if (!part2)
            total += abs(nums[i] - t);
        else{
            int x = abs(nums[i] - t);
            total += 0.5 * x * (x + 1);
        }
    }
    return total;
}
int calculateAverage(int n, int nums[]){
    int total = 0;
    int i;
    for (i = 0; i < n; i++)
        total += nums[i];   
    return total / i; 
}
int findTarget(int n, int nums[], bool part2){
    int avg = calculateAverage(n, nums);
    int start = avg - SEARCH_RANGE;
    start = start < 0 ? 0 : start; 
    int end = avg + SEARCH_RANGE ;
    end = end > n ? n : end;
    int minFuel = INT_MAX;
    for (int i = start; i < end; i++){
        int fuel = calculateFuelCost(i, n, nums, part2);
        if (fuel < minFuel)
            minFuel = fuel;
    }
    return minFuel;
}
void printArr(int n, int nums[]){
    for (int i = 0; i < n - 1; i++){
        printf("%d, ", nums[i]);
    }
    printf("%d\n", nums[n - 1]);
}
int main(int n, char *args[n]){
    int nums[1000];
    int x = readFile(args[1], nums);
    int leastFuel = findTarget(x, nums, false);
    printf("The least fuel required to align crabs is %d\n", leastFuel);
    leastFuel = findTarget(x, nums, true);
    printf("The least fuel required to align crabs is (part 2) %d\n", leastFuel);
}

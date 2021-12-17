#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

int readFile(char* filename, unsigned int binary[]){
    FILE *in = fopen(filename, "r");
    unsigned int i = 0;
    unsigned char nibble = fgetc(in);
    unsigned int offset = 28;
    int x = 0;
    while (!feof(in)){
        if (nibble >= '0' && nibble <= '9') i += (nibble - (unsigned int)'0') << offset;
        else if (nibble >= 'A' && nibble <= 'F') i += (nibble - (unsigned int)'A' + 10) << offset;
        if (offset == 0){
            offset = 32;
            binary[x++] = i;
            i = 0;
        }
        offset -= 4;
        nibble = fgetc(in);      
    }
    if (offset != 28)
        binary[x++] = i;
    fclose(in);
    return x;
}
void printBin(int b, unsigned int bin[]){
    for (int i = 0; i < b; i++)
        printf("%X ", bin[i]);
    printf("\n");
}
unsigned int extractNextNbits(int b, unsigned int bin[], int *binPtr, int *bitPtr, int n){
    if (*bitPtr == 32){
        *bitPtr = 0;
        *binPtr += 1;
    }
    if (n <= 32 - *bitPtr){
        unsigned int ret = (bin[*binPtr] >> (32 - n - *bitPtr)) & ((1u << n) - 1);
        *bitPtr += n;
        return ret;
    }
    else{
        int r = 32 - *bitPtr;
        unsigned int ret = (bin[*binPtr] & ((1u << r) - 1)) << (n - r);
        *binPtr += 1;
        ret += bin[*binPtr] >> (32 - (n - r));
        *bitPtr = n - r;
        return ret;
    }
}
long min(long a, long b){
    if (a < b) return a;
    return b;
}
long max(long a, long b){
    if (a > b) return a;
    return b;
}
long performOperation(int ID, long operand, long total){
    if ((total == -1 && ID < 5) || (total == -1 && ID == 7))
        return operand;
    switch (ID){
        case 0: return total + operand; break;
        case 1: return total * operand; break;
        case 2: return min(operand, total); break;
        case 3: return max(operand, total); break;
        case 5: return (total > operand) ? 1 : 0; break;  
        case 6: return (operand > total) ? 1 : 0; break;
        case 7: return (operand == total) ? 1 : 0; break;
    }
    return -1;
}
int parsePackets(int b, unsigned int bin[], int *binPtr, int *bitPtr, int *vTotal, long *literal){
    int v = extractNextNbits(b, bin, binPtr, bitPtr, 3);
    *vTotal += v;
    int ID = extractNextNbits(b, bin, binPtr, bitPtr, 3);
    *literal = -1;
    if (ID == 4){
        int litGroup = extractNextNbits(b, bin, binPtr, bitPtr, 5);
        *literal = 0;
        int litGroupC = 1;
        while ((litGroup & 16) == 16){
            litGroupC++;
            *literal += litGroup - 16;
            *literal = *literal << 4;
            litGroup = extractNextNbits(b, bin, binPtr, bitPtr, 5);
        }
        *literal += litGroup;
        return 6 + 5 * litGroupC;
    }
    else{
        int mode = extractNextNbits(b, bin, binPtr, bitPtr, 1);
        long opResult = -1;
        if (mode == 0){
            int subPacketLength = extractNextNbits(b, bin, binPtr, bitPtr, 15);
            int len = 0;
            while (len < subPacketLength && *binPtr < b){
                len += parsePackets(b, bin, binPtr, bitPtr, vTotal, literal);
                if (*literal != -1)
                    opResult = performOperation(ID, *literal, opResult);
            }
            *literal = opResult;
            return len + 21;
        }
        else{
            int subPacketCount = extractNextNbits(b, bin, binPtr, bitPtr, 11);
            int len = 0;
            for (int i = 0; i < subPacketCount; i++){
                len += parsePackets(b, bin, binPtr, bitPtr, vTotal, literal);
                if (*literal != -1)
                    opResult = performOperation(ID, *literal, opResult);
            }
            *literal = opResult;
            return len + 18;
        }
    }
}
int main(int n, char *args[n]){
    unsigned int bin[180];
    int b = readFile(args[1], bin);
    // printBin(b, bin);
    int binPtr = 0;
    int bitPtr = 0;
    int v = 0;
    long literal = 0;
    parsePackets(b, bin, &binPtr, &bitPtr, &v, &literal);
    printf("The sum of the version numbers in the BITS transmission is %d\n", v);
    printf("The result of the evaluated BITS transmission is %ld\n", literal);
}

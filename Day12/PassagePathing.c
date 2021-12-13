#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    char id[20];
    bool visited;
    bool big;
    struct Node* adj[20];
    int adjCount;
} node;

typedef node* graph[100];

void freeGraph(int x, graph g){
    for (int i = 0; i < x; i++)
        free(g[i]);
}
void addAdjNode(node* n1, node* n2){
    n1->adj[n1->adjCount++] = n2;
    n2->adj[n2->adjCount++] = n1;
}
node *newNode(char* nodeStr){
    node *n = malloc(sizeof(node));
    n->adjCount = 0;
    strcpy(n->id, nodeStr);
    n->visited = false;
    n->big = (nodeStr[0] - 'A' >= 26) ? false : true; 
    return n;
}
int insertNodes(char* node1, char* node2, int nodeCount, graph g){
    node *n1 = NULL;
    node *n2 = NULL;
    for (int i = 0; i < nodeCount; i++){
        if (strcmp(g[i]->id, node1) == 0) n1 = g[i];
        else if (strcmp(g[i]->id, node2) == 0) n2 = g[i];
    }
    if (n1 == NULL){
        n1 = newNode(node1);
        g[nodeCount++] = n1;
    }
    if (n2 == NULL){ 
        n2 = newNode(node2);
        g[nodeCount++] = n2;
    }
    addAdjNode(n1, n2);
    return nodeCount;
}
int readFile(char* filename, graph g){
    FILE *in = fopen(filename, "r");
    char buffer[256];
    fgets(buffer, sizeof(buffer), in);
    int x = 0;
    while (!feof(in)){
        char* node1 = strtok(buffer, "-");
        char* node2 = strtok(NULL, "\n");
        x = insertNodes(node1, node2, x, g);
        fgets(buffer, sizeof(buffer), in);
    }
    fclose(in);
    return x;
}
void printGraph(int x, graph g){
    for (int i = 0; i < x; i++){
        printf("%s: ", g[i]->id);
        for (int f = 0; f < g[i]->adjCount; f++){
            printf("%s ", g[i]->adj[f]->id);
        }
        printf("\n");
    }
}
bool nodeIsInArr(node* n, int x, node* arr[]){
    for (int i = 0; i < x; i++){
        if (strcmp(n->id, arr[i]->id) == 0) return true;
    }
    return false;
}
void printRoute(int x, node* route[]){
    for (int i = 0; i < x; i++){
        printf("%s ", route[i]->id);
    }
    printf("\n");
}
bool canGoThroughNode(node* n, int v, node* visited[]){
    if (strcmp(n->id, "start") == 0)
        return false;
    if (!(n->big) && nodeIsInArr(n, v, visited)){
        return false;
    }
    return true;
}
int countPathsHelper(node* curr, node* end, int v, node* visited[], int count, graph g){
    node* newVisited[100];
    // printRoute(v, visited);
    int x = v;
    for (int f = 0; f < x; f++)
        newVisited[f] = visited[f];
    for (int i = 0; i < curr->adjCount; i++){
        node* nextCurr = curr->adj[i];
        if (strcmp(nextCurr->id, end->id) == 0){
            count++;
            printf("%s\n", curr->id);
            newVisited[x++] = nextCurr;
            printRoute(x, newVisited);
        }
        else if (canGoThroughNode(nextCurr, v, visited)){
            newVisited[x++] = nextCurr;
            count = countPathsHelper(nextCurr, end, x, newVisited, count, g);
        }
    }
    return count;
}
int countPaths(int x, graph g){
    node *start;
    node *end;
    for (int i = 0; i < x; i++){
        if (strcmp(g[i]->id, "start") == 0) start = g[i];
        if (strcmp(g[i]->id, "end") == 0) end = g[i];
    }
    node* visited[] = { start };
    return countPathsHelper(start, end, 1, visited, 0, g);
}

int main(int n, char *args[n]){
    graph g;
    int x = readFile(args[1], g);
    printGraph(x, g); 
    int p = countPaths(x, g);
    printf("The total path count through the cave is %d\n", p);
    freeGraph(x, g);
}

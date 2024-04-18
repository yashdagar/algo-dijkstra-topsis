#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Constants
#define MAX_INT 65536

// Macros
#define or ||
#define and &&
#define not !
#define range(len) for(int i = 0; i < len; i++)
#define range2(len2) for(int j = 0; j < len2; j++)
#define range3(len3) for(int k = 0; k < len3; k++)

// Function prototypes
void createGraph();
void displayGraph();
void multiAttributeDijkstra(int destination);
int input(char *s);

// Global variables
int SIZE = 0;
int PARAMETER_SIZE = 0;
struct Node **nodes = NULL;
struct Parameter *parameters = NULL;

// Data structures
struct Parameter {
    char name[100];
    int weight;
};

struct Node;

struct Pointer {
    struct Node *node;
    float *values;
    float *normalizedValues;
};

struct Node {
    int index;
    struct Pointer *pointers;
};

struct ListNode {
    struct Node *node;
    struct ListNode *next;
};

// Function to create a new node
struct Node *newNode(int index){
    struct Node *new = malloc(sizeof(struct Node));
    new->index = index;
    new->pointers = malloc(SIZE * (sizeof(struct Node) + 2 * PARAMETER_SIZE * sizeof(float)));
    range(SIZE) {
        new->pointers[i].node = NULL;
        new->pointers[i].values = malloc(PARAMETER_SIZE * sizeof(float));
        new->pointers[i].normalizedValues = malloc(PARAMETER_SIZE * sizeof(float));
    }
    return new;
}

// Function to add a new node
void addNode(struct Node *n1, struct Node *n2){
    n2->pointers[n1->index].node = n1;
    n1->pointers[n2->index].node = n2;
    range(PARAMETER_SIZE){
        printf("Enter the %s: ", parameters[i].name);
        float val;
        scanf(" %f", &val);
        n1->pointers[n2->index].values[i] = val;
        n2->pointers[n1->index].values[i] = val;
    }
}

// Function to check if the parameters of a node are full
bool areParametersFull(struct Node *n){
    bool areFull = true;
    range(SIZE) {
        if (not n->pointers[i].node) {
            if(i == n->index) continue;
            areFull = false;
            break;
        }
    }
    return areFull;
}

// Function to create a new list node
struct ListNode *newListNode(struct Node *node){
    struct ListNode *new = malloc(sizeof(struct ListNode));
    new->node = node;
    new->next = NULL;
    return new;
}

// Function to display the linked list
void displayLinkedList(struct ListNode* n){
    printf("%d", n->node->index);
    n = n->next;
    if(not n){
        printf(" -> NULL");
    }
    while(n){
        printf(" -> %d", n->node->index);
        n = n->next;
    }
    printf("\n");
}

// Function to take an int input
int input(char *s){
    printf(" %s", s);
    int a;
    scanf(" %d", &a);
    return a;
}

/*
 *   Function to apply the TOPSIS (Technique for Order of Preference by
 *   Similarity to Ideal Solution) algorithm to all the nodes in the
 *   graph
 */
void applyTOPSISToAllPointers(){
    // Calculating the denominators for normalization of the values
    float denominators[PARAMETER_SIZE];
    range(PARAMETER_SIZE)
        denominators[i] = 0.0;

    range(SIZE){
        struct Node *n = nodes[i];
        range2(SIZE) {
            if(n->pointers[j].node == NULL or n->pointers[j].node->index <= n->index) continue;
            float *pointer = n->pointers[j].values;
            range3(PARAMETER_SIZE)
                denominators[k] += pow(pointer[k], 2);
        }
    }

    range(PARAMETER_SIZE)
        denominators[i] = sqrtf(denominators[i]);

    // Dividing the values by the denominators to normalize them
    range(SIZE){
        struct Node *n = nodes[i];
        range2(SIZE) {
            if(n->pointers[j].node == NULL) continue;
            float *pointer = n->pointers[j].values;
            float *res = n->pointers[j].normalizedValues;
            range3(PARAMETER_SIZE)
                res[k] = pointer[k] / denominators[k];
        }
    }
}

// Function to take input for the graph
void createGraph(){
    PARAMETER_SIZE = input("Enter the number of parameters: ");

    parameters = malloc(PARAMETER_SIZE * sizeof(struct Parameter));

    range(PARAMETER_SIZE){
        printf("Enter the name of the parameter: ");
        scanf(" %[^\n]", parameters[i].name);
        parameters[i].weight = input("Enter the weight of the struct Parameter(with positive or negative sign): ");
    }

    SIZE = input("Enter the number of struct Nodes: ");

    nodes = malloc(SIZE * sizeof(struct Node*));
    range(SIZE) nodes[i] = newNode(i);

    range(SIZE){
        if(areParametersFull(nodes[i])) continue;
        printf("Enter the details for struct Node %d:\n", i);
        while(1) {
            if(areParametersFull(nodes[i])) break;

            printf("Enter a value that struct Node %d points to or any number greater than %d to go to the next struct Node: ", i, SIZE-1);
            int n = input("");
            if(n == nodes[i]->index){
                printf("The node can not point to itself.\n");
                continue;
            }
            if(n >= SIZE or n < 0) break;
            addNode(nodes[i], nodes[n]);
        }
    }

    applyTOPSISToAllPointers();
}

// Function to get the shortest unvisited node
int getShortestUnvisitedNode(int distances[SIZE][2]){
    int shortest = -1, distance = MAX_INT;
    range(SIZE){
        if(distances[i][1] == 0){ // if the node is unvisited
            if(distances[i][0] < distance){
                shortest = i;
                distance = distances[i][0];
            }
        }
    }
    return shortest;
}

/*
 * Function to apply multi attribute Dijkstra algorithm.
 * Steps:
 *   1. Initialize distances and visited status for all nodes.
 *   2. Find the next unvisited node with the shortest distance from the destination.
 *   3. Update distances for neighboring nodes based on the shortest path.
 *   4. Repeat steps 2 and 3 until all nodes are visited.
 */
void multiAttributeDijkstra(const int destination){
    if(destination >= SIZE or destination < 0) {
        printf("Invalid input.\n");
        return;
    }

    // Distances from the destination of nodes and their visited status
    int distances[SIZE][2];
    range(SIZE) {
        distances[i][0] = MAX_INT;  // Distance from destination node
        distances[i][1] = 0;        // Visited status
    }
    distances[destination][0] = 0;  // Set the visited status of the destination node to 0

    // Path from source node to the destination node
    struct ListNode *paths[SIZE];
    range(SIZE)
        paths[i] = newListNode(nodes[i]);

    range3(SIZE){
        // Get the current shortest unexplored node
        int toExplore = getShortestUnvisitedNode(distances);

        // If mo unvisited nodes exist or the node is not connected to the destination node
        if(toExplore == -1){
            printf("One or more nodes are not connected to the destination node.\n");
            break;
        }

        // Explore all the nodes
        range(SIZE){
            // Pointer to the current node
            struct Pointer pointer = nodes[toExplore]->pointers[i];

            if(pointer.node == NULL)
                continue;

            // Calculate the rank of the path to the node
            float rank = 0.0;
            range2(PARAMETER_SIZE)
                rank += pointer.normalizedValues[j] * (float) parameters[j].weight;

            // If the current distance is shorter than the previous distance
            // then update the distance
            if(distances[i][0] > distances[toExplore][0] + rank){
                distances[i][0] = distances[toExplore][0] + rank;
                paths[i]->next = paths[toExplore];
            }
        }
        // Set the node to visited
        distances[toExplore][1] = 1;
    }

    // Print the paths from all the nodes
    range(SIZE){
        if(i == destination) continue;
        displayLinkedList(paths[i]);
    }
}

// Display the data of the graph
void displayGraph(){
    if(nodes == NULL or parameters == NULL){
        printf("The graph has not been initialized.\n");
        return;
    }
    range(SIZE){
        printf("[%d]: \n", i);
        bool flag = true;
        range2(SIZE){
            if(nodes[i]->pointers[j].node) {
                printf("\tIndex: %d\n\tValues: \n", nodes[i]->pointers[j].node->index);
                range3(PARAMETER_SIZE){
                    if(nodes[i]->pointers[j].node) {
                        printf("\t\t%s: \n", parameters[k].name);
                        printf("\t\t\tValue: %.4f\n", nodes[i]->pointers[j].values[k]);
                        printf("\t\t\tNormalized value: %.4f\n", nodes[i]->pointers[j].normalizedValues[k]);
                    }
                }
                flag = false;
            }
        }
        if(flag) printf("\tNULL");
        printf("\n");
    }
}

int main(){
    // Menu driven program 
    while (true){
        printf("Menu.\n1. Create graph\n2. Display graph\n3. Get routes\n4. Exit\n");
        int choice = input("Enter your choice: ");

        if(choice == 1)
            createGraph();
        else if(choice == 2)
            displayGraph();
        else if(choice == 3) {
            int c = input("Enter the source node: ");
            multiAttributeDijkstra(c);
        }
        else if(choice == 4) {
            printf("Thank you!\n");
            break;
        }
        else
            printf("Invalid choice.\n");
    }
}

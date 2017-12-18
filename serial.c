#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stackHw.h"
#include "timing.h"

typedef struct node{
    int num; 
    long long index;
    struct node* prev;
}Node;

typedef struct Stack{
    Node* top;
}Stack;

Stack stack;

void create_stack(){
    Node *node1 = (Node *) malloc(sizeof(Node)); //remember to malloc when define a struct every time
    node1->num  = 0;
    node1->index = 0;
    node1->prev = NULL; 
    stack.top = node1;
}


long long GetStackCount(){
    return stack.top->index;
}


int Pop(){
    int value;
    Node *temp_node = (Node*) malloc(sizeof(Node));
    if(GetStackCount() != 0){
        temp_node = stack.top;
        value = temp_node->num;
        stack.top = temp_node->prev;
        free(temp_node);
    }else{
        printf("The Stack is Empty ! \n");
    }
    printf("Pop %d, Size of Stack %llu\n",value,stack.top->index);
    return value; 
}


void Push(int value){
    Node *node = (Node*) malloc(sizeof(Node)); 
    node->prev = stack.top;
    node->num = value;
    node->index = stack.top->index + 1;
    stack.top = node;
    //free(node);
    printf("Push %d, Size of Stack: %lld\n",value,stack.top->index);
}




int main(int argc, char *argv[]){
    int i,j, n;
    if( argc == 2 ) {
        n = atoi(argv[1]);
    }
    else {
        printf("Wrong Arguments.\n");
        return -1;
    }

    GetNumbers("numbers.txt");
    create_stack();
    timing_start();
    for(j=0;j<n;j++){
        for(i=0;i<numSize;i++){
            Push(numbers[i]);
        } 
    }
    while(GetStackCount() != 0){
        Pop();
    }
    timing_stop();
    print_timing();
    return 0;
}


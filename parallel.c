/*Lock Free Concurrent Stack
 *Author: Chunpai Wang
 *reference and tutorial: https://www.red-gate.com/simple-talk/blogs/inside-the-concurrent-collections-concurrentstack/
 *Description: Each thread will do their own part of push, once finished its own push, each thread will start pop until 
 * stack is empty. So there is no popping size of each thread, which means thread1 may pop more value then it pushed. 
 * 
 * I used the linkedList, which always has a headNode with num = 0, index =0, and next pointer point to NULL.
 *
 * It is hard to print out order of pushing number, but if you comment the part of popping stack code in threadStack function
 * You will find that all expected values are all pushed into stack, and there is no missing value due to race condition
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "timing.h"
#include "stackHw.h"
#include <pthread.h>

typedef struct node{
    int num; 
    long long index;
    struct node* next;
}Node;

typedef struct Stack{
    Node* top;
}Stack;


typedef struct thread_data{
    int thread_id;   
    int n;   //number of loop
    int iteration;
}thread_data;

Stack stack;
long long count;


static void CreateStack(){
    Node *node1 = (Node *) malloc(sizeof(Node)); //remember to malloc when define a struct every time
    node1->num  = 0;
    node1->index = 0;
    node1->next = NULL; 
    stack.top = node1;
}


//Get the index of top node on the current stack
//I don't call this function in the program
long long GetStackCount(){
    return stack.top->index;
}


//check the top node when pushing and changing head to 
//avoid race condition
Node* PushCompareAndSet(Node* top, Node* current, struct node* cur_next){
    Node* original = (Node*)malloc(sizeof(Node)); 
    original = top;
    if(top == cur_next){
        stack.top = current;  //set top of stack as current node
        //printf("Pushing value:%d\n",stack.top->num);
        //printf("Push, Stack size now %d current top is %d\n",stack.top->index,stack.top->num);
    }
    return original;
} 


Node* PopCompareAndSet(Node* top, Node* topNext, Node* cur_next){
    Node* original = (Node*)malloc(sizeof(Node)); 
    original = top;
    if(top == cur_next){
        stack.top = topNext;  //set top of stack as current node
        //printf("Pop, Stack size now %d, current top is %d\n",stack.top->index,stack.top->num);
    }
    return original;
} 


void Push(int value){
    //pthread_mutex_lock(&stack_mutex);
    Node *node = (Node*) malloc(sizeof(Node)); 
    do{
        node->next = stack.top;
        node->num = value;
        node->index = stack.top->index + 1;
    }while(PushCompareAndSet(stack.top,node,node->next) != node->next); 
    //if original top is not same as current node's next, race condition occur, should push to new head

}


int Pop(){
    int value;
    Node *temp_node = (Node*) malloc(sizeof(Node));
    do{
        temp_node = stack.top;
        value = temp_node->num;
        if(temp_node->next == NULL){
            //free(temp_node);
            //printf("Stack Is Empty!\n");
            return -1;
        }
    }while(PopCompareAndSet(stack.top,stack.top->next,temp_node) != temp_node);
    //free(temp_node);
    return value; 
}


//each thread will push its own part of data (some iterations of numbers), and pop until stack is empty.
//if you comment the code of pop part, and check the total count of pushing,
//you will find that all values are pushed into stack, no value is pushed and missing due to race condition
void *threadStack(void *arg){
    thread_data *data = (thread_data *)arg;
    int i,j,iteration;
    iteration = data->iteration;

    //if iterations cannot be equally assigned to different thread
    //then last thread will also have the remainder iterations which is n%nthreads
    if( data->n - (data->thread_id+1)*(data->iteration) < data->iteration){
        iteration = data->n - data->thread_id*data->iteration;

    }
    else{
        iteration = data->iteration;
    }

    for(i=0;i<iteration;i++){
        for(j=0;j<numSize;j++)
            Push(numbers[j]);
    }
    
    //once finished the pushing, the thread can start popping.  
    //you can comment the pop code below, and check the correctness of pushing 
    int value;
    while(stack.top->index !=0){
        value = Pop(data->thread_id);
        //if(value != -1)
        //    printf("In thread %d, Pop %d\n",data->thread_id,value);
    }
    pthread_exit(NULL); //exit threads
}



int main(int argc, char *argv[]){
    timing_start();
    int i,j, n, num_threads; 
    if( argc == 3 ) {
        n = atoi(argv[1]);
        num_threads = atoi(argv[2]);
        if(n/num_threads<=1){
            perror("the input arguments which need to make iterations / num_threads to be greater than or equal to 1");
        }
    }
    else {
        printf("Wrong Arguments.\n");
        return -1;
    }

    GetNumbers("numbers.txt");  //read and get the numbers as global variable
    CreateStack();
    int iteration = n / num_threads;  //the average iterations for each thread assigned
    pthread_t threads[num_threads];   //declare number of threads
    int rc;
    int t;
    for(t=0; t<num_threads;t++){
        thread_data *t_data = (thread_data*) malloc(sizeof(thread_data));
        t_data->thread_id = t;
        t_data->n = n;
        t_data->iteration = iteration;
        rc = pthread_create(&threads[t], NULL, threadStack, (void *) t_data); //create threads
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    void *status;  //used to store the return value of each thread
    for(t=0; t<num_threads;t++){
        rc = pthread_join(threads[t],&status);  // need pthread_join to join threads first
        if (rc){
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    timing_stop();
    print_timing();
    pthread_exit(NULL); //exit threads
}



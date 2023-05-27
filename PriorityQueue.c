#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include <math.h>

struct Node{
  int x , y;
  float gCost , hCost;
  struct Node * prev;
};

typedef struct Node Node;

struct PriorityQueue{
 size_t heap_capacity; //capacitate vector de tati
 size_t heap_lenght; // numar de elemente
 Node * heap; // buffer care contine lista de Noduri

};

typedef struct PriorityQueue PriorityQueue;



PriorityQueue * init(size_t capacity){
   PriorityQueue * queue = (PriorityQueue *) malloc(sizeof(PriorityQueue));
   queue->heap_capacity = capacity;
   queue->heap_lenght=0;
   queue->heap= malloc(sizeof(Node) * queue->heap_capacity);
   printf("\nqueue was init\n");
   return queue;
}

int isEmpty(const PriorityQueue * queue){
  return queue->heap_lenght == 0;
}

void clear(PriorityQueue * queue){
  free(queue->heap);
  queue->heap_capacity = 0;
  queue->heap_lenght = -1;
}

Node * peek(const PriorityQueue * queue){
  if(isEmpty(queue)) return NULL;
  else return &(queue->heap[0]);
}
int atMaxCapacity(const PriorityQueue * queue){
  //printf("\nMax was called\n");
  return queue->heap_lenght >=queue->heap_capacity;
}
void expand(PriorityQueue * queue){
  queue->heap_capacity*=2;
  queue->heap = (Node * ) realloc(queue->heap ,queue->heap_capacity * sizeof(Node));
  //printf("\nexpanded was completed\n");
}
int compareNodes( const PriorityQueue* queue, int i , int j){
  Node * a = &queue->heap[i];
  Node * b = &queue->heap[j];
 
  return abs(a->gCost + a->hCost - b->gCost - b->hCost) <0.00001f;
}



void swap(PriorityQueue * queue , int i , int j){
  Node * temp  = (Node * ) malloc(sizeof(Node));
  memmove(temp , &queue->heap[i] , sizeof(Node));
  memmove(&queue->heap[i] , &queue->heap[j], sizeof(Node));
  memmove(&queue->heap[j] , temp ,sizeof(Node));

  free(temp);
}



int equals(const Node * a , const  Node * b){
      if(a->x == b->x)
        if(a->y == b->y) return 1;
  return 0;
}

void swim(PriorityQueue * queue , int child ){
//printf("\nSwimming has started\n");
     int parent = (child -1)/2; // obtin parintele copilului

     while(child > 0 && compareNodes( queue , child , parent) == 0) { // cat timp nu am ajuns la radacina si copilul este mai mic decat parintele
        swap(queue , parent , child);
        child = parent;
        parent = (child -1) /2; // obtin urmatorul parinte
     }
     //printf("\nSwimming has been completed\n");
}

void sink( PriorityQueue * queue, int parent){
  while(1){
    int left = 2 * parent +1 ; //copilul din stanga
    int right = 2 * parent +2 ; // copilul din dreapta
    int min = left; //presupun ca cel mai mic copil e cel din stanga
    
    if(right < queue->heap_lenght && compareNodes( queue, right , left) == 1 ){ // verific daca cel din dreapta este mai mic
          min = right;
    }
    if(left >= queue->heap_lenght || compareNodes(queue , parent , min)) break; // daca am iesit din limite sau nu se mai poate scufunda

    //se scufunda cu o pozitie
    swap( queue, min , parent);
    parent = min;
  }
}

void add(PriorityQueue * queue , Node node){
  //printf("\nAddition Started\n");
    if(atMaxCapacity(queue)) expand(queue);
    
    queue->heap[queue->heap_lenght] = node;
    swim(  queue ,queue->heap_lenght);
   // printf("\nSwimming has been completed\n");
    queue->heap_lenght++;
    //printf("\nnode was added\n");
}

Node * removeAt(PriorityQueue * queue , int where){
   if(isEmpty(queue)) return NULL;

   queue->heap_lenght--;

   Node * toBeRemoved = &queue->heap[where];
   
   if(where == queue->heap_lenght) return toBeRemoved; // este ultimul element

   sink(queue , where);//se scufunda incet si sigur

   if(equals(&queue->heap[where] , toBeRemoved)) swim(queue , where); // daca nu s-a scufundat inoata :)

   return toBeRemoved;
}

int removeNode(PriorityQueue * queue , Node node){

  for(int i = 0 ; i< queue->heap_lenght ; i++){
    if(equals(&queue->heap[i] , &node)){
      removeAt(queue , i);
      return 1;
    }
  }
  return 0;
}

Node * removeRoot(PriorityQueue * queue){
  return removeAt(queue , 0);
}

 int isMinHeap( PriorityQueue * queue, int k) {
    // If we are outside the bounds of the heap return true
    int heapSize = queue ->heap_lenght;
    if (k >= heapSize) return 1;

    int left = 2 * k + 1;
    int right = 2 * k + 2;

    // Make sure that the current node k is less than
    // both of its children left, and right if they exist
    // return false otherwise to indicate an invalid heap
    if (left < heapSize && !compareNodes( queue , k, left)) return 0;
    if (right < heapSize && !compareNodes( queue, k, right)) return 0;

    // Recurse on both children to make sure they're also valid heaps
    printf("%d , %d , f cost = %.0f\n" , queue->heap[k].x , queue->heap[k].y  , queue->heap[k].gCost + queue->heap[k].gCost);
    return isMinHeap(queue ,left) && isMinHeap(queue, right);
  }

  int contains( PriorityQueue* queue, Node elem) {
    // Linear scan to check containment
    for (int i = 0; i < queue->heap_lenght; i++) if (equals(&queue->heap[i] ,&elem)) return 1;
    return 0;
  }

/*int main(){

  PriorityQueue * queue = init(10);

  for(int i = 0 ; i < 10 ; i++){
    Node node;
    node.x = i;
    node.y = i;
    node.gCost = i;
    node.hCost = 100 - i;
    add(queue, node);
  }

  int res = isMinHeap(queue , 0);
  if(res == 0 ) printf("Not heap");
  else printf("Is a heap");
   getche();
   return 0;

}*/


#include "dict.h"


datum the_stack[SIZE+1];
int top = -1;

bool push(datum d){
  // Pushes a datum on top and returns true if successful and false if not. 
  top += 1;
  if (top < SIZE){
    the_stack[top] = d;
    return true;
  }
  else{
    printf("Stack Overflow\n");
    return false;
  }

}

bool empty(void){
  // Checks if the stack is empty
	return top == -1;

}

datum pop(void){
  // Pops and returns the top 
  if (!empty()){
    datum return_val = the_stack[top];
    top--;
    return return_val;
  }
  else{
    exit(1);
  }
}


datum peek(void){
  // Returns the top withuot popping
  if (!empty()){
    return the_stack[top];
  }
  else{
    exit(1);
  }
}

int stack_depth(){
  // calculated number of elements in stack
  return top + 1;
}

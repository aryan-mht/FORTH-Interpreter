#include "datum.h"



// Declare all the functions needed in multiple files; 
bool push(datum);
datum pop();
bool empty();
datum peek();
void rot();
void dup();
void drop();
void swap();
datum substring(char[], int, int);
datum space();
datum newline();


datum plus(datum, datum);
void minus(datum, datum, datum);
void multiply(datum, datum, datum);
void divide(datum, datum, datum);
void exponent(datum, datum, datum);


int stack_depth();
void not_op();
void and_op();
void or_op();
void equality();
void inequality();
void lessThan();
void lessThanEqual();
void greaterThan();
void greaterThanEqual();

void debug();


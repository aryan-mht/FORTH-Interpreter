#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include "stack.h" 

// Initializing a dict_tag to identify the dict type
enum dict_tag {CONSTANT, VARIABLE};



// initializing a dict 
struct dict{
    dict_tag flag; // flag to recognize type of data
    char name[SIZE+1]; // name of data
    datum data; // the data
    dict *next; // a pointer to the next dictonary 
};
// extern dict *the_dictionaty;  // this will point to the 'head' of the dictionaty list



// declaring functions to be used on dict
void store_const_or_var(char[]);
bool dict_retrieve(char[]);
int lookup();
void set();






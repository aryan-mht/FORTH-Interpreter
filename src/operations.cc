#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include <cstring>
#include "dict.h" 


void rot(){
    // This Function rotates the top three elements in a stack 
    if (stack_depth() < 3){
        printf("Stack Error: rot (contains less then 3 elements)\n");
        return;
    }
    datum d1 = pop();
    datum d2 = pop();
    datum d3 = pop();

    // Push them in different order
    push(d1);
    push(d3);
    push(d2);
}

void dup(){
    // This function duplicated the top element of the stack and pushes it on
    datum dup;
    if (!empty()){
        dup = peek();
        push(dup);
    }
    else{
        // if stack is empty: print: 
        printf("Stack Error: dup (empty)\n");
    } 
}

void drop(){
    // This funciton drops the element on the top of the stack.
    if(!empty()){
        pop();
    }
    else{
        printf("Stack Error: drop (empty)\n");
    }
}

void swap(){
    // This function swaps the top two elements in the stack 
    if (stack_depth()>= 2){
        datum d1 = pop();
        datum d2 = pop();

        push(d1);
        push(d2);
    }
    else{
        printf("Stack Error: swap (contains less than 2 elements)\n");
    }
}

datum substring(char *str, int start, int end){
	// This function modifies the substr string
	// ."hello" --> the dot-quote will be removed and the substring will be stored in substr
    datum return_d; // initialize a datum to return  
    if (start<0){start = 0;} // if start is < 0, make the start index to be 0
    if (end > (int)strlen(str)){end = strlen(str)-1;} // if the end index was more than 0, make the end index the last index


    //since the returning datum has a string tag, make it a string tag. 
    return_d.tag = STRING; 

    // Make a character pointer that would be modified
    char *tempchar = str; 

    // Make the tempChar point to the str[start]'s address 
    tempchar = &(str[start]);

    //determine the tombstone's index
    int null_loc = end-start +1; 
    //place the tombstone
    tempchar[null_loc] = '\0';

    //copy the tempchar to the datum's s field. 
    strcpy(return_d.s, tempchar);
    return return_d;
}

datum space(){
    // This function returns a datum with a STRING tag and a string value which is a space (" ") 
    // ending with a null terminator
    datum return_d;
    return_d.tag = STRING;
    char temp[2] = " ";
    temp[1] = '\0';
    strcpy(return_d.s, temp);
    return return_d;
}

datum newline(){
    // This function returns a datum with a STRING tag and a string value which is a newline ("\n") 
    // ending with a null terminator
    datum return_d;
    return_d.tag = STRING; 
    char temp[2] = "\n";
    temp[1] = '\0';
    strcpy(return_d.s, temp);
    return return_d;
}


void not_op(){
    
    /**
     * This function pops the top value of the stack and checks if it's a boolean 
     * if it is, then it pushes the opposite boolean value 
     * Else prints an error message and pushes the value back
     */
    if(!empty()){
        datum push_datum;
        datum popped = pop();
        if(popped.tag == BOOL){
            push_datum.tag = BOOL;
            push_datum.b = (!popped.b);
            assert(push(push_datum));
        }
        else{
            assert(push(popped)); // push the element back 
            printf("Stack Error: not operator (can only perform not operation on bool values)\n");
        }
    }
    else{
        printf("Stack Empty!\n");
    }
}
void and_op(){
    
    /**
     * This function pops two top value from the stack and checks if they're booleans 
     * if they are, then it pushes the computational value (b1 && b2)
     * Else prints an error message and pushes the value back
     */
    if(stack_depth() >= 2){
        datum push_datum;
        datum popped1 = pop();
        datum popped2 = pop();
        if(popped1.tag == BOOL && popped2.tag == BOOL){
            push_datum.tag = BOOL;
            push_datum.b = popped1.b && popped2.b;
            assert(push(push_datum));
        }
        else{
            assert(push(popped2)); // push elements back in the same order
            assert(push(popped1));
            printf("Stack Error: and operator (can only perform not operation on bool values)\n");
        }
    }
    else{
        printf("Stack Error: and ---> Contains less than 2 elements\n");
    }
}

void or_op(){
    
    /**
     * This function pops two top value from the stack and checks if they're booleans 
     * if they are, then it pushes the computational value (b1 || b2)
     * Else prints an error message and pushes the value back
     */
    if(stack_depth() >= 2){
        datum push_datum;
        datum popped1 = pop();
        datum popped2 = pop();
        if(popped1.tag == BOOL && popped2.tag == BOOL){
            push_datum.tag = BOOL;
            push_datum.b = (popped1.b || popped2.b);
            assert(push(push_datum));
        }
        else{
            assert(push(popped2));// push elements back in the same order
            assert(push(popped1));
            printf("Stack Error: and operator (can only perform not operation on bool values)\n");
        }
    }
    else{
        printf("Stack Error: or ---> Contains less than 2 elements\n");
    }
}

void equality(){
    /**
     * This function checks for the equality of the top two data of the stack and compares them. If they are equal, then it pushes 
     * true else, it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop();
        datum push_datum;
        if (d1.tag == d2.tag){
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0==strcmp(d1.s, d2.s)){push_bool = true;} // if equal, push true
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d1.f == d2.f){push_bool = true;} // if equal, push true
                else{push_bool = false;}
            }
            else{
                if(d1.b == d2.b){ // if equal, push true
                    push_bool = true;}
                else{push_bool = false;}
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else{
            printf("Stack Error: = (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: = ---> Contains less than 2 elements\n");
    }
}

void inequality(){
    /**
     * This function checks for the inequality of the top two data of the stack and compares them. If they are not equal, then it pushes 
     * true, else it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop();
        datum push_datum;
        if (d1.tag == d2.tag){
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0!=strcmp(d1.s, d2.s)){push_bool = true;} // if not equal, push true
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d1.f != d2.f){push_bool = true;} // if not equal, push true
                else{push_bool = false;} 
            }
            else{
                if(d1.b != d2.b){ // if not equal, push true
                    push_bool = true;}
                else{push_bool = false;}
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else{
            printf("Stack Error: <> (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: <> ---> Contains less than 2 elements\n");
    }
}


void lessThan(){
    /**
     * This function checks if the second from top value on the stack is less than the top value of the stack 
     * if it is, then it pushes true, else it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop(); //checks if d2 is less than d1
        datum push_datum;
        if (d1.tag == d2.tag && d1.tag != BOOL){  // if they are of the same type
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0>strcmp(d2.s, d1.s)){push_bool = true;} // if d2.s is less than d1.s
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d2.f < d1.f){push_bool = true;} // if d2.f is less than d1.f
                else{push_bool = false;} 
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else if(d1.tag == d2.tag && d1.tag == BOOL){
            printf("Stack Error: < cannot be performed on booleans\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
        else{
            printf("Stack Error: < (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: < Contains less than 2 elements\n");
    }

}



void lessThanEqual(){
    /**
     * This function checks if the second from top value on the stack is less or equal to the top value of the stack 
     * if it is, then it pushes true, else it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop(); //checks if d2 is less than d1
        // d2<=d1 for it to be true
        datum push_datum;
        if (d1.tag == d2.tag && d1.tag != BOOL){  // if they are of the same type
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0>=strcmp(d2.s, d1.s)){push_bool = true;} // if d2.s is less than or equal to d1.s
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d2.f <= d1.f){push_bool = true;} // if d2.f is less or equal to d1.f
                else{push_bool = false;} 
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else if(d1.tag == d2.tag && d1.tag == BOOL){
            printf("Stack Error: <= cannot be performed on booleans\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
        else{
            printf("Stack Error: <= (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: <= Contains less than 2 elements\n");
    }
}


void greaterThan(){
    /**
     * This function checks if the second from top value on the stack is greater than the top value of the stack 
     * if it is, then it pushes true, else it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop(); //checks if d2 is greater than d1
        datum push_datum;
        if (d1.tag == d2.tag && d1.tag != BOOL){  // if they are of the same type
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0<strcmp(d2.s, d1.s)){push_bool = true;} // if d2.s is greater than d1.s
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d2.f > d1.f){push_bool = true;} // if d2.f is greater than d1.f
                else{push_bool = false;} 
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else if(d1.tag == d2.tag && d1.tag == BOOL){
            printf("Stack Error: > cannot be performed on booleans\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
        else{
            printf("Stack Error: > (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: > Contains less than 2 elements\n");
    }
}

void greaterThanEqual(){
    /**
     * This function checks if the second from top value on the stack is greater or equal to the top value of the stack 
     * if it is, then it pushes true, else it pushes false 
     * The first two data of the stack must be of the same time for this operation to work
     */
    if(stack_depth() >= 2){
        bool push_bool;
        datum d1= pop();
        datum d2 = pop(); //checks if d2 is greater than or equal to d1
        datum push_datum;
        if (d1.tag == d2.tag && d1.tag != BOOL){  // if they are of the same type
            if (d1.tag == STRING){ // only need to check one tag since both are equal
                if(0<=strcmp(d2.s, d1.s)){push_bool = true;} // if d2.s is greater than or equal to d1.s
                else{push_bool = false;}
            }
            else if (d1.tag == FLOAT){
                if(d2.f >= d1.f){push_bool = true;} // if d2.f is greater than or equal to d1.f
                else{push_bool = false;} 
            }
            push_datum.tag = BOOL;
            push_datum.b = push_bool;
            assert(push(push_datum));
        }
        else if(d1.tag == d2.tag && d1.tag == BOOL){
            printf("Stack Error: >= cannot be performed on booleans\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
        else{
            printf("Stack Error: >= (can only be performed on data of the same type)\n");
            assert(push(d2)); // push elements back in the same order
            assert(push(d1));
        }
    }
    else{
        printf("Stack Error: >= Contains less than 2 elements\n");
    }
}

void debug(){
    /**
     * @brief This function gets called when the -d is entered as an argument to the command line. After every 
     * command is read, this function gets called.
     * It prints the top 6 elements of stack as the commands gets executed. 
     * If there are less than 6 elements, it prints all of the elements on the stack 
     * If the stack is empty, it prints "stack empty" 
     */ 
    if(!empty()){ // if not empty
        if(stack_depth() <= 6){ // if the stack has more than or equal to 6 elements 
            for(int i=stack_depth()-1; i<=5 && i >=0; i--){
                switch(the_stack[i].tag){ // check the tag and print accordingly
                    case BOOL: printf("%s", the_stack[i].b ? "true" : "false"); printf(" ");; break;
                    case FLOAT: printf("%0.*f", precision ,the_stack[i].f); printf(" ");break;
                    case STRING: printf("%s", the_stack[i].s); printf(" ");break;
                    default: assert(false); break;
                }   
            }
            printf("\n");
        }
    }
    else{
        printf("Stack empty\n");
    }
}
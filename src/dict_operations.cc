#include "dict.h" 


dict* the_dictionaty = NULL;


void store_const_or_var(char input_cmd[10]){
    /**
     * @brief This function stores the constant name and its value in the dictionary 
     * 
     */
    if (stack_depth() >= 2){
        
        datum d1 = pop(); // any datum 
        datum d2 = pop(); //string datum
        if(d2.tag == STRING){ //continue only if the second popped elemeny is string
            dict* dict_elem = new dict;
            if(0 == strcmp(input_cmd, "constant")){
                dict_elem->flag = CONSTANT;
            }
            else{
                dict_elem->flag = VARIABLE;
            }
            strcpy(dict_elem->name,d2.s); // copy the name of the constant into the dictionary's name field
            // set the data to the d1 datum
            dict_elem->data = d1;
            // iterate forward
            dict_elem->next = the_dictionaty;
            //make the_dictionaty point to the head of the list
            the_dictionaty = dict_elem;
        } 
        else{
            printf("Stack Error: %s -- The name of the %s must be a string!\n", input_cmd, input_cmd); // will print 'constant' or 'variable' in place of %s depending of the user input
            push(d2);
            push(d1);
        }        
    }
    else{
        printf("Stack Error: %s -- Contains less than 2 elements\n", input_cmd); // will print 'constant' or 'variable' in place of %s depending of the user input
    }
}



bool dict_retrieve(char input[SIZE+1]){
    /**
     * Parameter: an input cmd that comes from the user
     * This function looks through  the dictionary to find if the constant exists in the dictionaty or not.  
     * This function will return a true for the success of it being stored in the dict false otherwise.
     * If found, it pushes the value from the dictionary onto the stack. 
     */
    for (dict *iterate = the_dictionaty; NULL!=iterate; iterate = iterate->next){
        if(0 ==strcmp(input, iterate->name)){
            if(iterate->flag != WORD){
                datum push_datum;
                push_datum.tag = iterate->data.tag;
                switch(push_datum.tag){
                    case FLOAT: push_datum.f = iterate->data.f; break;
                    case BOOL: push_datum.b = iterate->data.b; break;
                    case STRING: strcpy(push_datum.s, iterate->data.s); break;
                }
                push(push_datum);
                return true;
            }
            else if(WORD == iterate->flag){
                printf("FD INSIDE RETRIEVE: %d\n", iterate->data.f);
                FILE* file = fdopen(iterate->data.f, "r");
                process(file);
                close(iterate->data.f);
                return true;

            }
        }
    }
    return false;
    
}



int lookup(){
    /**
     * @brief this function looks up if the cmd input is in the dictionary or not. If it is, it returns the dict_tag of that dictionaty. 
     * Else, it returns -1 to represent that no matches were found. 
     */
    if(stack_depth() >= 2){ // there must be 2 values in the stack 
        datum d1 = pop();
        datum d2 = peek(); // should contain string datum containing the name of the constant
        if(d2.tag != STRING){
            push(d1);
            return -1; // if d2 is not string, no need to go through the whole function, just return -1 
        }
    

        for (dict *dict_iterate = the_dictionaty; NULL != dict_iterate; dict_iterate = dict_iterate->next){
            if (strcmp(d2.s, dict_iterate->name) == 0)
            {
                // printf("Returnig true\n");
                push(d1); 
                return dict_iterate->flag;
                
            }
            
        }
        push(d1);
        return -1; // if not found in the dict
        
    }
    else{
        return -1;
    }
}


void set(){
    /**
     * @brief This function takes the name of a variable (as a string datum) and a new value, and changes
     * the datum stored in the dictionary for that variable. 
     * This function cannot be called upon constants!
     * var_name: the name of the variable 
     * val: new value to be changed with  
     * */

    if(stack_depth() >= 2){
        if (-1 == lookup())
        {
            printf("This variable is currently not in the dictionaty. Add the variable to change it later.\n");
            return ;
        }
        datum val = pop();
        datum var_name = pop();
        for (dict *itr_dict = the_dictionaty; NULL!=itr_dict; itr_dict= itr_dict->next){
            if(0==(strcmp(itr_dict->name, var_name.s))){
                if(itr_dict->flag == VARIABLE){
                    switch(val.tag){
                        case FLOAT: 
                            itr_dict->data.tag = FLOAT; 
                            itr_dict->data.f = val.f;
                            break;
                        case BOOL:
                            itr_dict->data.tag = BOOL; 
                            itr_dict->data.b = val.b;
                            break;
                        case STRING:
                            itr_dict->data.tag = STRING; 
                            strcpy(itr_dict->data.s, val.s); 
                            break;                   
                    }
                    break;
                }
                else{
                    printf("Stack Error: set -- can only be performed on variables. Not constants.\n");
                    push(var_name);
                    push(val);
                }
            }
        }
    }
    else{
        printf("Stack Error: set -- Contains less than 2 elements\n");
        return;
    }


}











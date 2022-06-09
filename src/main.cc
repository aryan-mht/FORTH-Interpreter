#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include <string.h>
#include <cmath>
#include "dict.h"


int process(FILE*);
void operations();
bool DEBUG = false; 

// TODO: and q2, and free memory, README.md
int main(int argc, char *argv[])
{

	int opt;

	while(-1 != (opt = getopt(argc, argv, "h:fd")))
	{
		switch (opt)
		{
		case 'h':
			printf("main: my RPN calculator\n");
			printf("options: -h show this for help\n");
			return EXIT_SUCCESS;
			break;
		case 'f': // if f is entered as a command 
			FILE *f;
			for (int i = optind; i < argc; i++){ // optind will be the number of files passed after the -f option
				printf("ARGC: %d\n",argc);
				f = fopen(argv[i], "rt"); // store the file pointer in f
				process(f); // process the data 
				fclose(f); // close the file				
			} 
			break;
		case 'd':
			DEBUG = true; // if the -d is recognized, make the DRBUG variable true 
			break;
		default:
			printf("%s -- unknown option\n", argv[1]);
			return EXIT_FAILURE;
			break;
		}
		opt = -1;
	}
	return process(stdin); // after processing the data from file (if -f was entered), continue to process data from stdin. 
}
// ======================================
char cmd[SIZE+1];  // for input
int precision = 4; // precision for fix operation




void operations(){
	/**
	 * @brief This function gets called if any of the operations (+,-,*,/ or fix) gets inputted
	 * Performs calculation and pushes the new value datum onto the stack
	 */
	datum d1; // to store the first datum popped
	datum d2; // to store the second datum popped
	datum post_exp; // datum that will store values after operations have been performed (post_expressions)
	
	
	if(0  == strcmp(cmd, "+"))
	{
		if (stack_depth() >= 2) // Check if stack has at least 2 elements
		{
			// pop the datum elements 
			d1 = pop();
			d2 = pop();

			// if both are floats then perform + on floats 
			if (d1.tag == FLOAT && d2.tag == FLOAT)
			{	post_exp.tag = FLOAT;
				post_exp.f = d1.f + d2.f;
				push(post_exp);
				// debug();
			}
			// Or do string concatenation if both are String
			else if (d1.tag == STRING && d2.tag == STRING){
				post_exp.tag = STRING;
				strcpy(post_exp.s, strcat(d2.s, d1.s));
				push(post_exp);
				// debug();		
				
			}
			else{ // if not floats or string
				(printf("Cannot perform this operation on the provided data type\n"));
				push(d2);
				// debug();			
				push(d1);
				// debug();				
				}


		}
		else{
			//If less then 2 elements in the stack 
			printf("Not Enough Numbers of Input to Perform Operation\n");
		}
		
	}
	else if(0== strcmp(cmd, "*")){
		// if * then do these
		
		if (stack_depth() >= 2){
			d1 = pop();
			d2 = pop();
			// If both are float then treat them as floats and multiply
			if (d1.tag == FLOAT && d2.tag == FLOAT){
				
				post_exp.tag = FLOAT;
				post_exp.f = d1.f * d2.f;
				push(post_exp);
				// debug();
			}
			else if (d1.tag == FLOAT && d2.tag == STRING)
			{ 
				// Else repeat one string num_copies times. 
				post_exp.tag = STRING;
				int num_copies = d1.f;
				if(num_copies >= 0){
					int counter = 0;
					// Concatenate the string num_copies times and copy it to post_exp
					while (counter < num_copies){
						strcpy(post_exp.s, strcat(post_exp.s, d2.s));
						counter++;
					}
					push(post_exp);
					// debug();
				}
				// if negative provided: print this  
				else {printf("Cannot repeat negative times\n");}
				 

			}
			else{
				printf("Cannot perform this operation on the provided data type\n");
				push(d2);
				// debug();
				push(d1);
				// debug();
			}
		}
		else{
			printf("Not Enough Numbers of Input to Perform Operation\n");
		}

		
	}
	else if(0==strcmp(cmd, "/")){
		// Perform division calculation
		
		if(stack_depth()>= 2){
			d1 = pop();
			d2 = pop();
			if(d1.tag == FLOAT && d2.tag == FLOAT){
				post_exp.tag = FLOAT;
				post_exp.f = d2.f / d1.f;
				push(post_exp);
			}
			else{
				printf("Cannot perform this operation on the provided data type\n");
				push(d2);
				push(d1);
				
			}	
		}	
		else{
			printf("Not Enough Numbers of Input to Perform Operation\n");
		}

		
	}
	else if(0==strcmp(cmd, "-") ){
		// perform subtraction calculation 
		if(stack_depth() >= 2){
			d1 = pop();
			d2 = pop();
			if(d1.tag == FLOAT && d2.tag == FLOAT){
				post_exp.tag = FLOAT;
				post_exp.f = d2.f - d1.f;
				push(post_exp);
			}
			else{
				printf("Cannot perform this operation on the provided data type\n");
				push(d2);
				push(d1);
			}
		}
		else{
			printf("Not Enough Numbers of Input to Perform Operation\n");

		}
		
	}
	else if(0 == strcmp(cmd, "^")){
		// perform exponential calcualtion
		if(stack_depth() >= 2)
		{
			d1 = pop();
			d2 = pop();
			if(d1.tag == FLOAT && d2.tag == FLOAT){
			post_exp.tag = FLOAT;
			post_exp.f = pow(d2.f, d1.f);
			push(post_exp);
			}
			else{
				printf("Cannot perform this operation on the provided data type\n");
				push(d2);
				push(d1);

			}
		}
		else{
			printf("Not Enough Numbers of Input to Perform Operation\n");
		}
		
		
	}
		
	else if (0==strcmp(cmd, "fix")){
		// Set the decimal value to the value input
		d1 = pop();
		precision = d1.f;
	}				
}




// =====================================
int process(FILE* file){
	/**
	 * @brief This function takes in a FILE pointer that could point to a file (if -f argument is entered)
	 * or it could be a stdin in which it will take inputs only from the terminal. 
	 * This is where the processing of the data takes from a file/stdin happens. 
	 */
	

	datum input; // a datum which will be manipulated based on what was entered and then will be pushed 
	
	while(!feof(file))  
	{
		if(fscanf(file,"%s", cmd)!= -1)  // read from file 
		{
			float value; 
			if (sscanf(cmd, "%f", &value) != 0) 
			{
				// If the input is float, then change the input.tag to FLOAT and make its value to the input
				input.tag = FLOAT;
				input.f = value;		
				push(input);
				// if(DEBUG){
				// 	debug();
				// }		
			}
			else if (strcmp(cmd, "true") == 0)
			{
				// If the input is true, then change the input.tag to BOOL and make its value to true
				input.tag = BOOL;
				input.b = true;
				push(input);
				// if(DEBUG){
				// 	debug();
				// }					
			}
			else if (strcmp(cmd, "false") ==0)
			{
				// If the input is false, then change the input.tag to BOOL and make its value to false
				input.tag = BOOL;
				input.b = false;
				push(input);
				// if(DEBUG){
				// 	debug();
				// }					
			}
			else if (cmd[0] == '.' && cmd[1] == '"' && cmd[strlen(cmd)-1] == '"'){
				// If the input has the first character as . and second character as " and the last character as "
				// Then that's a string 
				input.tag = STRING;
				int START_IDX = 2; 
				// Call the substring() function that would remove the delimiters and return a datum 
				push(substring(cmd,START_IDX, strlen(cmd)-2));
				// if(DEBUG){
				// 	debug();
				// }					
			}
			else
			{
				// If the input is any of the special operations, then 
				// Simply call the functions for the corrosponding input
				if ((strcmp(cmd, "rot")==0)){rot();}
				else if (strcmp(cmd, "dup")==0){dup();}
				else if(strcmp(cmd, "drop")==0){drop();}
				else if (strcmp(cmd, "swap")==0){swap();}
				else if (strcmp(cmd, "substring") ==0){
					// Since the function substring accepts three arguments, 
					// Save the three datums by popping them off and providing
					// the function with the arguments. 
					datum end = pop();
					datum start = pop();
					datum str = pop(); 
					// The substring function will slice a string and return a datum with a string
					// that has character from str[start] to str[end] inclusive 
					push(substring(str.s, start.f, end.f));}
				// else if cmd matches with any of the known commands, then call the corrosponding function:
				else if(strcmp(cmd, "space") ==0){push(space());}
				else if (strcmp(cmd, "newline") ==0){push(newline());}
				else if (0== strcmp(cmd, "not")){not_op();}
				else if(0 == strcmp(cmd, "and")){and_op();}
				else if(0== strcmp(cmd, "or")){or_op();}
				else if(0== strcmp(cmd, "=")){equality();}
				else if(0== strcmp(cmd, "<>")){inequality();}
				else if(0 == strcmp(cmd, "<")){lessThan();}
				else if(0 == strcmp(cmd, "<=")){lessThanEqual();}
				else if(0 == strcmp(cmd, ">")){greaterThan();}
				else if(0 == strcmp(cmd, ">=")){greaterThanEqual();}
				// if the cmd was constant or variable
				else if(0 == strcmp(cmd, "constant") || 0 == strcmp(cmd, "variable"))
				{
					// first lookup in the dict
					if(-1 == lookup()) // if not found in the dict
					{store_const_or_var(cmd);} // store it in the dict

					else if(lookup() == CONSTANT) //if found in the dict to be CONSTANT, print this msg
					{printf("Constant name previously defined. A Constant cannot be overwritten!\n");}
					
					else if (lookup() == VARIABLE) { //if found in the dict to be VARIABLE, print this msg
						printf("Variable name previously defined. To overwrite the value, use the 'set' operation\n");
						}
				}
				else if(0 == strcmp(cmd, "set")){
					set(); // call set function
				} 
				else if ((strcmp(cmd, "+") == 0 || strcmp(cmd, "-") == 0) || strcmp(cmd, "*") == 0 || strcmp(cmd, "/") == 0 || strcmp(cmd, "^") == 0 || strcmp(cmd, "fix") == 0){
					operations(); // call the operation function to perform operations
				}
				else if(dict_retrieve(cmd)){
					; // do nothing as the function dict_retrieve will look up the name and if found, it will push the value of the dict on to the stack

				}
				else if (strcmp(cmd, ".")==0)
				// If the input is .
				{
					if(!empty()){ 
						
						datum pop_datum = pop();
						// Determine the tag of data type on the top of stack
						switch(pop_datum.tag)
						{
							// Print the values correctly for the corrosponding data types. 
							case BOOL: printf("%s\n", pop_datum.b ? "true" : "false"); break;
							case FLOAT: printf("%0.*f\n", precision ,pop_datum.f); break;
							case STRING: printf("%s\n", pop_datum.s); break;
							default: assert(false); break;
						}
					}

					else{
						printf("Stack Underflow\n"); // if the stack is empty and a . is recognized, print this
					}
					
				}
				else
				{
					// if an unknown command has entered, print this: 
					printf("USER_ERROR: unrecognized op '%s'\n", cmd);
				}
				


			}
		}
		if(!feof(file) && DEBUG){ 
			// if an only if the DEBUG is true and cntrl+d is not pressed, run the debug()
			// !feof(file) makes sure that debug does not get called when the user press cntrl+d, since it would print an empty message. 
		  	debug(); 
		} 
	}
	
	return EXIT_SUCCESS; 
}
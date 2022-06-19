#include "dict.h"


int process(FILE*);
int process_file(const char*);
int process_cmd(char*);
int process_until(const char*, bool);
void operations(); // for all the aithmetic 
void store_def(); // store user-defined definitions 
void store_commands(int); // store commands (takes a file descriptor as argument)
int cleanup(); // removes temp files 
bool DEBUG = false; 
bool DEFINING = false; 
char tmpfilename[100];
char tmpfiles_array[100][100];
int file_count = 0;


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
			// FILE *f;
			for (int i = optind; i < argc; i++){ // optind will be the number of files passed after the -f option
				process_file(argv[i]);
				// f = fopen(argv[i], "rt"); // store the file pointer in f
				// process(f); // process the data 
				// fclose(f); // close the file				
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
	process(stdin); // after processing the data from file (if -f was entered), continue to process data from stdin. 
	return cleanup();
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


int process_file(const char* fname){
	//Opens a file, processes it, and closes it.  
    FILE* in = fopen(fname, "rt");
    if (NULL == in){
        perror(fname);
        return EXIT_FAILURE;
    }
    int r = process(in);
    fclose(in);
    return r;
}

// =====================================
int process(FILE* file){
	/**
	 * @brief This function takes in a FILE pointer that could point to a file (if -f argument is entered)
	 * or it could be a stdin in which it will take inputs only from the terminal. 
	 * This is where the processing of the data takes from a file/stdin happens. 
	 */
	while(!feof(file))  
	{
		if(fscanf(file,"%s", cmd)!= -1)  // read from file 
		{
			process_cmd(cmd);
		}
		if(!feof(file) && DEBUG){ 
			// if an only if the DEBUG is true and cntrl+d is not pressed, run the debug()
			// !feof(file) makes sure that debug does not get called when the user press cntrl+d, since it would print an empty message. 
		  	debug(); 
		} 
	}
	
	return EXIT_SUCCESS; 
}


int process_cmd(char* cmd){
	// this function taes the input from scanf and processes it. Does the string comparison 
	// and calls the respective function for the operation. 


	datum input; // a datum which will be manipulated based on what was entered and then will be pushed
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
		// else if(0 ==lookup_usrdef(cmd)){
		// 	printf("PRINTING");
		// }
		else if(dict_retrieve(cmd)){  
			; // do nothing as the function dict_retrieve will look up the name and if found, it will push the value of the dict on to the stack

		}

		else if(0==strcmp("if", cmd)){
			if(!empty()){
				datum d = pop();
				if(BOOL == d.tag){
					process_until("else", d.b);
					process_until("then", !d.b);
				}
				else{
					printf("Not a bool!\n");
					// return EXIT_FAILURE
				}
			}
		}
		else if(0==strcmp("else", cmd) || 0 == strcmp("then", cmd)){
			;  // this is to make sure if the user typed something wrong on the top of the stack 
			// that it does not print "Unrecognised op 'else' or 'then'"
		}
		else if(0==strcmp(":", cmd)){
			store_def();
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
	// remove(tmpfilename);
	return EXIT_SUCCESS;
}


int process_until(const char* term, bool exec){
	// this function process the commands until it sees the delimiters 
	// which are going to be "else" or "then"
	while(!feof(stdin)){
		if (1==fscanf(stdin, "%s", cmd)){
			if(0==(strcmp(term, cmd))){
				return EXIT_SUCCESS;
			}
			else{
				if (exec){
					process_cmd(cmd);
				}
				else{
					if(0==strcmp(cmd, "if")){
						process_until("then", false);
					}
				}
			}
		}
		else{
			return EXIT_FAILURE;
		}
	}
	return EXIT_FAILURE;
}



void store_def(){
	/**
	 * @brief This fuction stores user-defined definitions in the dictionary: 
	 * The name of the user-defined function becomes the name field of the dict. 
	 * The file address of the tmp file becomes the s field of the data of the dict (dict->data.s)
	 */
	datum d;
	if(!empty()){
		d = pop();
	}
	if(STRING == d.tag){
		datum new_entry;
		
		dict *dict_elem = new dict;  // new dict 
		dict_elem->flag = WORD; // flag of the dict pt WORD

		// store the filename in the variable
		sprintf(tmpfilename, "tmp/%s.XXXXXX", d.s);

		// make a new datum with tag STRING which will store the tmp file address
		new_entry.tag = STRING; // tag of the datum to be STRING

		// make the file
		int fd = mkstemp(tmpfilename);

		// add the file in a file array which contains all the temp files created during the program 
		// this will come handy when doing cleanup of the files 
		strcpy(tmpfiles_array[file_count], tmpfilename);
		file_count += 1;	


		strcpy(new_entry.s, tmpfilename);  // store the tempfile name in the .s field 
		// new_entry.f =  fd;
		strcpy(dict_elem->name, d.s);
		dict_elem->data = new_entry;
		// iterate forward
		dict_elem->next = the_dictionaty;
		//make the_dictionaty point to the head of the list
		the_dictionaty = dict_elem;
		
		// call this function to store the commands of the definition. 
		store_commands(fd);

	}
}

void store_commands(int fd){
	/**
	 * @brief Stores the body of the definition from (not including : and up to but not including ;)
	 * Opens the temp file for writing and writes the command body in it 
	 */
	while(!feof(stdin)){
		if (1==fscanf(stdin, "%s", cmd)){
			if(0!=strcmp(cmd, ";")){
				write(fd, cmd, strlen(cmd));
				write(fd, " ", strlen(" "));
			}
			else{
				return;
			}
		}
	}
}


int cleanup(){
	/**this function deletes all the temp files crated during the execution of the program
	*/
	for(int i = 0; i < file_count; i++){
		remove(tmpfiles_array[i]);
	}
	return EXIT_SUCCESS;
	
}
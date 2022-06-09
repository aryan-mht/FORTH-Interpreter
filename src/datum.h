const int SIZE = 80;
extern int precision; 
enum data_type {
    BOOL, FLOAT, STRING
};


struct datum{
    // a structure with the following fields: 
    // A tag to recognize which data type it is
    data_type tag;
    bool b; 
    float f;
    char s[SIZE+1]; 
};

// Make an array of datums. 
extern datum the_stack[SIZE+1];
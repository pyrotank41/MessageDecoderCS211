
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char L[] = {'a', 'b', 'c', 'd'};
char L2[] = {'m', 'n', 'o', 'p'};

typedef struct StackStruct
{
    char* darr;  /* pointer to dynamic array  */
    int  allocated;  /* amount of space allocated */
    int  inUse; /* top of stack indicator  - counts how many values are on the stack   */	  
} Stack;

void init (Stack* s)
{
    s->allocated= 2;
    s->darr = (char*) malloc ( sizeof (char) * s->allocated);
    s->inUse  = 0;
}

void print_message(Stack* s) // printing the content of the stack
{
    for (int i = s->inUse-1; i >= 0; --i)
    {
        printf("%c", s->darr[i]);
    }
}


void push (Stack* s, char val, int debug)
{   
    if(s->allocated == s->inUse) // dynamicly increasing array's size if full.
    {
        char* temp = s->darr; //storing old array pointer.
        s->darr = (char*) malloc ( sizeof (char) * (s->allocated + 2)); // new array.
        for (int i = 0; i < s->allocated; i++) //copying over values to new array
        {
            s->darr[i] = temp[i];
        }
        free(temp); // freeing old array.
        s->allocated += 2; // increasing sallocated variable.
    }
    s->darr[s->inUse] = val; // adding value to the top of the stack.
    if(debug == 1) printf("pushing %c\n", s->darr[s->inUse]);
    s->inUse = s->inUse + 1; // simple incrementation of the top of the stack
    
}

int isEmpty (Stack* s)
{
    if ( s->inUse == 0)  return 1;
    else return 0;
}


char top (Stack* s) // returning top char in stack
{
    return (  s->darr[s->inUse-1] );
}

void pop (Stack* s, int debug) // simple pop
{    
    s->inUse = s->inUse - 1;
    if(debug == 1) printf("popping %c\n", s->darr[s->inUse]);
}

void reset (Stack* s) // resetting the top index.
{
    s->inUse = 0;
}

int inL(char c) // finding char in L.
{
    // Simple linear search
    for (int i = 0; i < 4; i++)
    {
        if(c == L[i]) return i;
    }
    return -1;
}

int inL2(char c) // finding char in L2.
{
    //simple linear search.
    for (int i = 0; i < 4; i++)
    {
        if(c == L2[i]) return i;
    }
    return -1;
}
 
void decode (Stack* st, char* word, int debug)
{


    Stack decode; // stack to store decoded chars.
    init (&decode); // important to initialize stack.

    int j = 0;
    while (word[j] != '\0') // loooping till null ptr.
    {   
        char c = word[j];
        
        int indexL = inL(c); // finding the char in L.
        int indexL2 = inL2(c); // Finding the char in L2.

        if(indexL != -1)
        {
          push(st, c, debug); // pushing strictly chars in L 
        }
        
        else if(indexL2 != -1)
        {
            if( top(st) == L[indexL2]) // looking for perfect map.
            { 
                push(&decode, L[indexL2], 0); // using the decoded char in another stack.
                pop(st, debug); //popping if perfect map.
            }
            else // since not a perfect map, we discard the word
            { 
                break;
            }
        }
        j += 1;
        
    }
    //if stack is empty, the word is perfictly mapped. Hence printing it. 
    if(isEmpty(st))
    {
        print_message(&decode);
        printf(" ");
        reset(&decode); // resetting top pointer
    }
}


int main(int argc, char const *argv[])
{
	int debugMode = 0;
	char input[300];

    Stack st1;
    init (&st1);
    
    // looking for debug flag.
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-d") == 0)
        {
            debugMode = 1;
            printf("Debug mode is ON!!");
        }
    } 
	
    while (1)
    {
        // get line of input from standard input
        printf ("\nEnter input to check or q to quit\n");
        fgets(input, 300, stdin);

        // remove the newline character from the input 
        int i = 0;
        while (input[i] != '\n' && input[i] != '\0')
        {
            i++;
        }
        input[i] = '\0';


        // check if user enter q or Q to quit program 
        if ( (strcmp (input, "q") == 0) || (strcmp (input, "Q") == 0) )
            break;
        
        // printf ("%s\n", input);

        //Start tokenizing the input into words separated by space. We use strtok() function from string.h
        //The tokenized words are added to an array of words
        char delim[] = " ";
        char *ptr = strtok(input, delim); // this function splits the string based on delim.
        int j = 0; // this will be our # of words.
        char *wordList[15]; // we are assuming that the message wont be more than 15 words long.

        while (ptr != NULL)
        {	
            wordList[j++] = ptr;
            ptr = strtok(NULL, delim);
        }

        //Decoding starts here.
        //Ittereating through all the words and sending it to decode function.
        for (int i = 0; i < j; i++)
        {
            // printf("%s\n", wordList[i]);
            decode(&st1, wordList[i], debugMode);
            reset(&st1);
        }
        
    }


    printf ("\nGoodbye\n");

	return 0;
}

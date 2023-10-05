/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Kacie Beckett 1452651
  Dated:     03/09/2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency
#define LINELEN 999

#define CRTRNC '\r'                             // carriage return character

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node{                   // a node in a linked list of transitions has
    char*           str;        // ... a transition string
    state_t*        state;      // ... the state reached via the string, and
    node_t*         next;       // ... a link to the next node in the list.
};

typedef struct {                // a linked list consists of
    node_t*         head;       // ... a pointer to the first node and
    node_t*         tail;       // ... a pointer to the last node in the list.
} list_t;

struct state {                  // a state in an automaton is characterized by
    unsigned int    id;         // ... an identifier,
    unsigned int    freq;       // ... frequency of traversal,
    int             visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    unsigned int    nid;        // ... the identifier of the next new state.
} automaton_t;



/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int fileno(FILE *);
int mygetchar(void); 
int read_line(char *line, int maxlen);          // getchar() that skips carriage returns

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/

/*
To-Do List:
Stage 0:
- take the chars from each line and put them into a tree 
> first create a root node
> then check each node whether any node off of root contains the char as data
> if not then add a node off root containing the data and continue adding the nodes until end of line
> if yes increment frequency of that node and go to the next node and check
<repeat these steps>

now generate the information required by iterating through the array in depth first order
Stage 1:

*/

int main(int argc, char *argv[]) {
    char line[LINELEN + 1] = {0};
    int *root = NULL;

    while (read_line(line, LINELEN)) {
        if (strlen(line) > 0 && line[0] != '\0') {
            //printf("boop");
        } else {
            //switch to the next stage
        }
    }

    for(int i = 0; i < argc; i++){
        printf("\ni:%d %s\n", i,argv[i]);
    }




    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}

// Reads a line of input into the array passed as argument,
// returns false if there is no input available.
// All whitespace characters are removed on the way through.
// ATTRIBUTION: ALISTAIR MOFFAT ASSIGNMENT 1 2023
//
int read_line(char *line, int maxlen) {
    int i=0, c;
    while (((c=mygetchar())!=EOF) && (c!='\n')) {
        if (i<maxlen && !isspace(c)) {
            line[i++] = c;
        }
    }
    line[i] = '\0';
    // then, if the input is coming from a file or the output
    // is going to a file, it is helpful to echo the input line
    // and record what the command was 
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
        printf("%s\n", line);
    }
    return ((i>0) || (c!=EOF));
}


void add_node(char current, int index){


}

void get_new_node(int data){


}

// Algorithms are Fun!!!
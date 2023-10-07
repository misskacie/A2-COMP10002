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
#include <stdbool.h>
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

#define ROOTID 0
#define CRTRNC '\r'       // carriage return character
#define NULTRM '\0' // used to mark the end of a string

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node {                   // a node in a linked list of transitions has
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
    bool            visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    unsigned int    nid;        // ... the identifier of the next new state.
} automaton_t;


/*----------------------------------------------------------------------------*/
int fileno(FILE *);
int mygetchar(void);  // getchar() that skips carriage returns
automaton_t *init_automaton(void);     
state_t *create_state(int id); 
list_t *create_list(void);
node_t *create_node(char *str, int id);
state_t *insert_at_tail(automaton_t *automaton, state_t *state, char *str);
state_t *insert_state(automaton_t *automaton, state_t *state, char *str);
int read_line_into_automaton(automaton_t *automaton, int *charcount);
void print_state(state_t *state, char *message);
/*----------------------------------------------------------------------------*/



int 
main(int argc, char *argv[]) {
    automaton_t *automaton;
    automaton = init_automaton();
    int stage0_char_count = 0;

    while (read_line_into_automaton(automaton, &stage0_char_count)) {
        //printf("line read=================================\n");
        //print_state(automaton->ini,"main");
    }
    printf(SDELIM, 1);
    printf(NOSFMT,automaton->ini->freq);
    printf(NOCFMT, stage0_char_count);
    printf(NPSFMT,automaton->nid);
    

    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int 
mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}

automaton_t 
*init_automaton(void){
    automaton_t *automaton;
    automaton = (automaton_t*) malloc(sizeof(*automaton));
    assert(automaton != NULL);
    automaton->ini = create_state(ROOTID); //root state has id 0
    automaton->nid = 1;
    return automaton;
}

state_t 
*create_state(int id) {
    state_t *state;
    state = (state_t*) malloc(sizeof(*state));
    assert(state != NULL);
    state->freq = 0;
    state->id = id;
    state->visited = false;
    state->outputs = create_list();
    
    return state;
}

// Adapted from listops.c to match style
// Credit: Alistair Moffat
list_t 
*create_list(void) {
    list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->tail = NULL;
	return list;
}


node_t 
*create_node(char *str, int id) {
    node_t *node;
    node = (node_t*) malloc(sizeof(*node));
    assert(node != NULL);

    node->str = str;
    node->state = create_state(id);
    node->next = NULL;

    return node;
}

// Adapted from listops.c 
// Credit: Alistair Moffat
state_t 
*insert_at_tail(automaton_t *automaton, state_t *state, char *str) {
    node_t *new_node;
    new_node = create_node(str, automaton->nid);
    automaton->nid += 1;

    if (state->outputs->tail == NULL){
        //first insertion
        state->outputs->head = state->outputs->tail = new_node;
        state->visited = true;
    } else {
        state->outputs->tail->next = new_node;
        state->outputs->tail = new_node;
    }

    state->freq += 1;

    return new_node->state;
}



state_t 
*insert_state(automaton_t *automaton, state_t *state, char *str) {
    char *newstr;
    newstr = (char*) malloc(2*sizeof(*newstr));
    strcpy(newstr,str);

    node_t *current_node; 
    current_node = state->outputs->head;


    
    bool flag = (current_node != NULL) ? true : false;
    while (flag){
        if (!strcmp(current_node->str, str)) {
            state->freq += 1;
            //print_state(current_node->state);
            return current_node->state; 
        }
        // guard clause
        if (current_node->next == NULL) {
            break;
        }
        current_node = current_node->next;
    }
    // node with that string doesnt exist so make one
    
    state_t *newstate;
    //printf("new %s\n",newstr);
    newstate = insert_at_tail(automaton, state, newstr);
    //print_state(state,"insertstate");
    return newstate;

           
}


int
read_line_into_automaton(automaton_t *automaton, int *charcount) {
    char c;
    char str[2];
    state_t *current_state;
    current_state = automaton->ini; 
    bool flag = false;
    
    while (((c=mygetchar()) != EOF) && (c != '\n')) {
        *charcount += 1;
           
            str[0] = c;
            str[1] = NULTRM;
            //printf("%s\n",str);
            //print_state(automaton->ini,"readline");
            current_state = insert_state(automaton, current_state, str);
            //print_state(automaton->ini,"readlineafter");
        
        flag = true;
    }
    return (flag);
}

void 
print_state(state_t *state, char *message){
    printf("\n");
    printf("%s\n",message);
    printf("STATE-----------------\n");
    printf("id: %d\n",state->id);
    printf("freq: %d\n",state->freq);
    printf("visted: %d\n",state->visited);
    node_t *currentnode;
    currentnode = state->outputs->head;
    
    while(state->outputs->head != NULL){
        printf("encoded state: %s\n",currentnode->str);
        if(currentnode->next == NULL){
            break;
        }
        currentnode = currentnode->next;
    }
    printf("--------------------\n");
    printf("\n");

}





// Algorithms are Fun!!!
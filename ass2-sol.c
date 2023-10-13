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

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n" // stage delimiter
#define MDELIM "-------------------------------------\n"  // delimiter of -'s
#define THEEND "==THE END============================\n"  // end message
#define NOSFMT "Number of statements: %d\n"               // no. of statements
#define NOCFMT "Number of characters: %d\n"               // no. of chars
#define NPSFMT "Number of states: %d\n"                   // no. of states
#define TFQFMT "Total frequency: %d\n"                    // total frequency

#define ROOTID 0
#define MAXINTDIGITS 10 // Num digits that int type can hold without overflow
#define CRTRNC '\r'     // carriage return character
#define NULTRM '\0'     // used to mark the end of a string
#define NULTRMSIZE 1
#define TRUNCATELENGTH 37

#define TRUE 1
#define FALSE 0

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t; // a state in an automaton
typedef struct node node_t;   // a node in a linked list
struct node {                 // a node in a linked list of transitions has
    char *str;                // ... a transition string
    state_t *state;           // ... the state reached via the string, and
    node_t *next;             // ... a link to the next node in the list.
};

typedef struct {  // a linked list consists of
    node_t *head; // ... a pointer to the first node and
    node_t *tail; // ... a pointer to the last node in the list.
} list_t;

struct state {         // a state in an automaton is characterized by
    unsigned int id;   // ... an identifier,
    unsigned int freq; // ... frequency of traversal,
    list_t *outputs;   // ... a list of output states.
};

typedef struct {      // an automaton consists of
    state_t *ini;     // ... the initial state, and
    unsigned int nid; // ... the identifier of the next new state.
} automaton_t;

/* FUNCTION PROTOTYPES--------------------------------------------------------*/
void print_stages(automaton_t *automaton, int *char_count, int *state_count,
                  int *freq_count);
int mygetchar(void); // getchar() that skips carriage returns
// MEMORY MANAGEMENT FUNCTIONS
automaton_t *init_automaton(void); // initiliases the memory for automaton type
state_t *create_state(int id); // initiliases the memory for state type
list_t *create_list(void); // initiliases the memory for list type
node_t *create_node(char *str, int id); // initiliases the memory for list type
char *copy_string(char *str); // returns a pointer to a copied string
char *create_string(int length); // returns a pointer to a new string
char *string_concat(char *str1, char *str2); // return pointer to concat string
void free_automaton(automaton_t *automaton); // free memory used by automaton
void recursive_free_automaton(state_t *state); // recursive iter and free memory

// Inserts items to a linked list ASCII-betical order of node string
state_t *sorted_list_insert(automaton_t *automaton, state_t *state, char *str);
state_t *insert_state(automaton_t *automaton, state_t *state, char *str);
node_t *select_next_state(state_t *state);
state_t *find_state_match(state_t *state, char *str);
node_t *find_substring(state_t *state, char *str);
int read_line_into_automaton(automaton_t *automaton, int *charcount);
void print_ellipses(int *char_count);
int replay(automaton_t *automaton, state_t *current_state, state_t *tmp_state,
           int *flag, int *char_count, char *str);
int replay_automaton(automaton_t *automaton);
state_t *find_next_compression(state_t *state, state_t *target);
int compress_automaton(automaton_t *automaton, int *freq_count);
void perform_compressions(automaton_t *automaton, int *freq_count,
                          int *state_count);
void print_state(state_t *state, char *message);

/*----------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    automaton_t *automaton;
    automaton = init_automaton();
    int char_count = 0, state_count = 0, freq_count = 0;


    print_stages(automaton, &char_count, &state_count, &freq_count);
    free_automaton(automaton);
    return EXIT_SUCCESS; // algorithms are fun!!!
}

/*----------------------------------------------------------------------------*/

// Control function to run code for each stage and print the required output
void print_stages(automaton_t *automaton, int *char_count, int *state_count,
                  int *freq_count) {
    // Stage 0
    printf(SDELIM, 0);
    while (read_line_into_automaton(automaton, char_count));

    // freq for uncompressed automaton == number of chars in input
    *freq_count = *char_count;
    *state_count = automaton->nid;
    printf(NOSFMT, automaton->ini->freq);
    printf(NOCFMT, *char_count);
    printf(NPSFMT, *state_count);

    // Stage 1
    printf(SDELIM, 1);
    while (replay_automaton(automaton)) {
        printf("\n");
    }

    // Stage 2
    printf(SDELIM, 2);
    perform_compressions(automaton, freq_count, state_count);
    printf(NPSFMT, *state_count);
    printf(TFQFMT, *freq_count);
    printf(MDELIM);
    while (replay_automaton(automaton)) {
        printf("\n");
    }
    printf(THEEND);
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c = getchar()) == CRTRNC);
    return c;
}

/*MEMORY MANAGEMENT FUNCTIONS-------------------------------------------------*/

// Initialise the memory for the automaton type and set initial values
automaton_t *init_automaton(void) {
    automaton_t *automaton;
    automaton = (automaton_t *)malloc(sizeof(*automaton));
    assert(automaton != NULL);

    automaton->ini = create_state(ROOTID); // root state has id 0
    automaton->nid = 1;
    return automaton;
}

// Initialise the memory for the state type and set initial values
state_t *create_state(int id) {
    state_t *state;
    state = (state_t *)malloc(sizeof(*state));
    assert(state != NULL);

    state->freq = 0;
    state->id = id;
    state->outputs = create_list();
    return state;
}
// Intialise the memory for the list type and set initial values
// Credit: Alistair Moffat, 'listops.c'
list_t *create_list(void) {
    list_t *list;
    list = (list_t *)malloc(sizeof(*list));
    assert(list != NULL);

    list->head = list->tail = NULL;
    return list;
}
// Intialise the memory for the node type and set initial values
// Credit: Alistair Moffat, adapted from 'listops.c'
node_t *create_node(char *str, int id) {
    node_t *node;
    node = (node_t *)malloc(sizeof(*node));
    assert(node != NULL);

    node->str = str;
    node->state = create_state(id);
    node->next = NULL;
    return node;
}

// returns a pointer to the copied string
char *copy_string(char *str) {
    char *newstr;
    newstr = create_string(strlen(str));
    assert(newstr != NULL);

    strcpy(newstr, str);
    return newstr;
}

char *create_string(int length) { 
    return (char *)malloc(sizeof(char) * length + NULTRMSIZE); 
    }

// return pointer to a new string which is a concatanation of the input strings
// only free's str2 to prevent needing to copy str2 in the usage context
char *string_concat(char *str1, char *str2) {
    assert(str1 != NULL && str2 != NULL);
    char *rtr_str;
    rtr_str = create_string(strlen(str1)+strlen(str2));
    assert(rtr_str != NULL);

    strcpy(rtr_str, str1);
    strcat(rtr_str, str2);
    free (str2);
    return rtr_str;
}

// Credit: Alistair Moffat, Adapted from 'treeops.c'
// Wrapper function so the automaton type can also be freed
void free_automaton(automaton_t *automaton) {
    assert(automaton != NULL);
    recursive_free_automaton(automaton->ini);
    free(automaton);
}

// Recursively iterates through the entire data structure to free memory 
// allocated to nodes, node transition string, lists and states
// Credit: Alistair Moffat, Adapted from 'treeops.c'
void recursive_free_automaton(state_t *state) {
    node_t *current_node, *tmp;
    current_node = state->outputs->head;
    if (current_node) {
        while (current_node != NULL) {
            recursive_free_automaton(current_node->state);
            tmp = current_node;
            current_node = current_node->next;
            free(tmp->str);
            free(tmp);
        }
    }
    free(state->outputs);
    free(state);
}

/*----------------------------------------------------------------------------*/

// when inserting ASCII characters to the output list put them in ASCII-betical
// order so that other operations are easier
// REFERENCE: [https://www.geeksforgeeks.org/given-a-linked-list-which-is-sorted
// how-will-you-insert-in-sorted-way/]
state_t *sorted_list_insert(automaton_t *automaton, state_t *state, char *str) {
    node_t *new_node;
    new_node = create_node(str, automaton->nid);
    automaton->nid += 1;
    state->freq += 1;
    if (state->outputs->tail == NULL) {
        // first insertion
        state->outputs->head = state->outputs->tail = new_node;
        return new_node->state;
    }

    // strcmp == 0 is checked before this function is called so dont check it
    if (strcmp(str, state->outputs->head->str) < 0) {
        // check if smaller than head and insert at head
        new_node->next = state->outputs->head;
        state->outputs->head = new_node;
    } else {
        node_t *current_node;
        current_node = state->outputs->head;
        while (current_node->next != NULL &&
               strcmp(str, current_node->next->str) > 0) {
            current_node = current_node->next;
        }
        new_node->next = current_node->next;
        current_node->next = new_node;
        state->outputs->tail = new_node;
    }
    return new_node->state;
}

// find a state corresponding to the input string and increase its frequency or
// add a new node corresponding to the input string
state_t *insert_state(automaton_t *automaton, state_t *state, char *str) {
    node_t *current_node;
    current_node = state->outputs->head;
    // try to find a state corresponding to the input string in the output list
    state_t *newstate;
    newstate = find_state_match(state, str); 
    if (newstate != NULL) {
        state->freq += 1;
        return newstate;
    }
    // node with that string doesnt exist, make one and add to the output list
    char *newstr;
    newstr = copy_string(str);
    
    newstate = sorted_list_insert(automaton, state, newstr);
    return newstate;
}

// Because the output list for each state is sorted ASCII-beticaly, choose the
// rightmost occurance of the highest frequency. 
node_t *select_next_state(state_t *state) {
    assert(state != NULL);

    node_t *largest_node, *current_node;
    current_node = largest_node = state->outputs->head;
    // find the node of highest frequency and highest string ASCII number
    while (current_node != NULL) {
        if (current_node->state->freq >= largest_node->state->freq) {
            largest_node = current_node;
        }
        current_node = current_node->next;
    }
    return largest_node;
}

// Add states to the automaton for a given input line
int read_line_into_automaton(automaton_t *automaton, int *charcount) {
    char c, str[2];
    state_t *current_state;
    current_state = automaton->ini;
    int flag = FALSE;
    while (((c = mygetchar()) != EOF) && (c != '\n')) {
        *charcount += 1;
        str[0] = c;
        str[1] = NULTRM;
        current_state = insert_state(automaton, current_state, str);
        flag = TRUE;
    }
    return flag;
}

// Adapted version from Alistair Moffat's recursive traverse in treeops.c
// Assumes a low->high sorted ASCII-betically connections linked list and
// iterates through each output to find the next valid state to compress
state_t *find_next_compression(state_t *state, state_t *target) {
    node_t *current_node;
    current_node = state->outputs->head;
    if (current_node != NULL && target == NULL) {
        if (state->outputs->head == state->outputs->tail &&
            current_node->state->outputs->head != NULL) {
            target = state;
        }
        while (current_node != NULL && target == NULL) {
            target = find_next_compression(current_node->state, target);
            current_node = current_node->next;
        }
    }
    return target;
}

// the arc to compress is the first item of the output list as it is sorted
// ASCII-betically
int compress_automaton(automaton_t *automaton, int *freq_count) {
    state_t *top_state, *bottom_state;
    top_state = find_next_compression(automaton->ini, NULL);
    if (top_state == NULL) {
        return 0;
    }
    bottom_state = top_state->outputs->head->state;
    *freq_count -= bottom_state->freq; // as bottom_state will be removed
    list_t *tmp;
    tmp = top_state->outputs;
    top_state->outputs = bottom_state->outputs;
    free(bottom_state);

    // iterate through each node and concatinate the string identifiers 
    node_t *current_node;
    current_node = top_state->outputs->head;
    while (current_node != NULL) {
        current_node->str = string_concat(tmp->head->str, current_node->str);
        current_node = current_node->next;
    }
    free(tmp->head->str);
    free(tmp->head);
    free(tmp);
    return 1;
}

void perform_compressions(automaton_t *automaton, int *freq_count,
                          int *state_count) {
    char str[MAXINTDIGITS+NULTRMSIZE], c;
    int i = 0;
    while ((c = mygetchar()) && c != '\n' && i < MAXINTDIGITS) {
        str[0 + i] = c;
        i++;
    }
    str[0 + i] = NULTRM;
    int compression_steps = atoi(str);
    i = 0;
    while (i < compression_steps) {
        if (!compress_automaton(automaton, freq_count)) {
            break;
        }
        i++;
    }
    *state_count -= i;
}

// print the ellipses that follow the inputed line before the generated output
void print_ellipses(int *char_count) {
    int i = 0;
    while (i < 3 && *char_count < TRUNCATELENGTH) {
        *char_count += 1;
        printf(".");
        i++;
    }
}

// returns the state which corresponds to the input string or NULL if it
// can't be found
state_t *find_state_match(state_t *state, char *str) {
    assert(state != NULL);
    node_t *current_node;
    current_node = state->outputs->head;
    while (current_node != NULL) {
        if (strcmp(current_node->str, str) == 0) {
            return current_node->state;
        }
        current_node = current_node->next;
    }
    return NULL;
}

// Find whether the input string is a substring of a string corresponding
// to a node and choose the most ASCII-betically high occurance. 
// Return a Node if found or NULL
node_t *find_substring(state_t *state, char *str) {
    assert(state != NULL);
    node_t *current_node, *found;
    current_node = state->outputs->head;
    found = NULL;
    while (current_node != NULL) {
        if (strstr(current_node->str, str) != NULL) {
            // choose the rightmost occurance of the substring as it is the 
            // highest ASCII-betically
            found = current_node;
        }
        current_node = current_node->next;
    }
    return found;
}

int replay(automaton_t *automaton, state_t *current_state, state_t *tmp_state,
           int *flag, int *char_count, char *str) {
    int output = FALSE;

    if (*char_count < TRUNCATELENGTH && *flag == TRUE &&
        current_state == tmp_state) {
        node_t *current_node;
        current_node = select_next_state(current_state);
        while ((current_node != NULL)) {
            printf("%s", current_node->str);
            output = TRUE;
            current_node = select_next_state(current_node->state);
        }
    }

    if (output == FALSE) {
        return 0;
    }
    return 1;
}

int replay_automaton(automaton_t *automaton) {
    char c, str[TRUNCATELENGTH+NULTRMSIZE];
    // Max line width is TRUNCATELENGTH so the output string for a line can be
    // at most that long as all other chars will be thrown out.
    int char_count = 0, flag = FALSE;
    state_t *current_state, *tmp_state;
    current_state = tmp_state = automaton->ini;

    int i = 0;
    while ((c = mygetchar())) {
        if (c == '\n' || c == EOF) {
            if (tmp_state == NULL) {
                printf("%c", *(str));
                // char_count has increased above TRUNCATE LENGTH because
                // the loop runs to delete excess chars
                char_count -= (strlen(str) - 1);
            }
            if (flag == TRUE && char_count < TRUNCATELENGTH) {
                node_t *found_node;
                if ((found_node = find_substring(current_state, str)) != NULL &&
                    tmp_state == NULL) {
                    printf("%s", str);
                    print_ellipses(&char_count);
                    int max = strlen(found_node->str);
                    int i = strlen(str);
                    while (i < max) {
                        printf("%c", *(found_node->str + i));
                        i++;
                    }
                    tmp_state = current_state = found_node->state;
                    replay(automaton, current_state, tmp_state, &flag,
                           &char_count, str);
                    return flag;
                }
                print_ellipses(&char_count);
                replay(automaton, current_state, tmp_state, &flag, &char_count,
                       str);
            }
            return flag;
        }
        if (char_count < TRUNCATELENGTH) {
            str[0 + i] = c;
            str[1 + i] = NULTRM;
            char_count++;
            i++;
        }
        flag = TRUE;
        tmp_state = find_state_match(current_state, str);
        if (tmp_state != NULL) {
            current_state = tmp_state;
            if (char_count < TRUNCATELENGTH) {
                printf("%s", str);
            }
            i = 0;
        }
    }
    return flag;
}

// Algorithms Are Fun!!!

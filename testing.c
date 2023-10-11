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
#define NULLTERM '\0'
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
char
*copy_string(char *str) {
    char *newstr;
    newstr = (char*) malloc(sizeof(*str));
    strcpy(newstr,str);
    return newstr;
}

char
*change_string_size(char *str, int new_length ) {
    return realloc(str, new_length);
}

char
*create_string(int length) {
    return (char*) malloc(sizeof(char) * length);
}


char
*string_concat(char *str1, char *str2) {
    str1 = realloc(str1, sizeof(char)*(sizeof(*str1)+sizeof(*str2)));
    strcat(str1, str2);
    free(str2);
    return str1;
}


int main(int argc, char *argv[]) {
    char *str, *str2;
    str = create_string(3);
    str2 = create_string(10);

    *str = 'a';
    *(str+1) = 'b';
    *(str+2) = '\0';
    for (int i; i <= 8; i++){
        *(str2+i) = 86;
       // printf("%c %d\n",*(str+i),i);
    }
    *(str2+9) = '\0';

    string_concat(str,str2);
    printf("%s",str);
  
    // printf("hi");
    // printf("%s\n",str);
    // printf("%d\n",strlen(str));
    return EXIT_SUCCESS;        // algorithms are fun!!!
}

int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}


void add_node(char current, int index){


}

void get_new_node(int data){

    
}

// Algorithms are Fun!!!
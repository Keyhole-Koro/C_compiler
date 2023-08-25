#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    NUM,      //number
    ADD,      // +
    SUB,      // -
    MUL,      // *
    DIV,      // /
    L_PARENTHESES, // (
    R_PARENTHESES, // )

    EXPR, // E
    TERM, // T
    FACTOR, // F
    ACCEPTED, //accept
    
    S,  // shift
    R, // reducce
    G, //goto
    ACC, //acc
};

typedef struct {
    int left;
    char *right;
    int numChar;
    int columnTable;
} Production;

typedef struct {
    int column;
    int symbol;
} Columns;

typedef struct {
    int SRGA;
    int Item;
} Action;

Production productions[] = {
    {EXPR, "E$", 2, 8},
    {EXPR, "E+T", 3, 8},
    {EXPR, "E-T", 3, 8},
    {EXPR, "T",1 , 8},
    {TERM, "T*F", 3, 9},
    {TERM, "T/F", 3, 9},
    {TERM, "F", 1, 9},
    {FACTOR, "(E)", 3, 10},
    {FACTOR, "num", 1, 10}
};

Columns columns[] = {
    {0, NUM},
    {1, ADD},
    {2, SUB},
    {3, MUL},
    {4, DIV},
    {5, L_PARENTHESES},
    {6, R_PARENTHESES},
    {7, ACCEPTED},
    {8, EXPR},
    {9, TERM},
    {10, FACTOR},
};

int numSymbols = 7;

int stack[100];
int top = -1;

int result[100];
int size_result = 0;



Action table[][17] = {
//    id   +  -  *  /  (  )  $  E  T  F
    { {S, 5}, {}, {}, {}, {}, {S, 2}, {}, {}, {G, 1}, {G, 3}, {G, 4} },//0
    { {}, {S, 6}, {S, 7}, {}, {}, {}, {}, {ACC, 0}, {}, {}, {} },//1
    { {S, 5}, {}, {}, {}, {}, {}, {}, {}, {G, 8}, {G, 3}, {G, 4} },//2
    { {}, {R, 3}, {R, 3}, {R, 8}, {R, 9}, {}, {R, 3}, {R, 3}, {}, {}, {} },//3
    { {}, {R, 6}, {R, 6}, {R, 6}, {R, 6}, {}, {R, 6}, {R, 6}, {}, {}, {} },//4
    { {}, {R, 8}, {R, 8}, {R, 8}, {R, 8}, {}, {R, 8}, {R, 8}, {}, {}, {} },//5
    { {S, 5}, {}, {}, {}, {}, {S, 2}, {}, {}, {}, {G, 10}, {G, 4} },//6
    { {S, 5}, {}, {}, {}, {}, {S, 2}, {}, {}, {}, {G, 11}, {G, 4} },//7
    { {}, {S, 6}, {S, 8}, {}, {}, {}, {S, 12}, {}, {}, {}, {} },//8
    { {S, 5}, {}, {}, {}, {}, {S, 2}, {}, {}, {}, {}, {G, 13} },//9
    { {S, 5}, {}, {}, {}, {}, {}, {S, 2}, {}, {}, {}, {G, 14} },//10
    { {}, {R, 1}, {R, 1}, {S, 8}, {S, 9}, {}, {R, 1}, {R, 1}, {}, {}, {} },//11
    { {}, {R, 2}, {R, 2}, {S, 8}, {S, 9}, {}, {R, 2}, {R, 2}, {}, {}, {} },//12
    { {}, {R, 7}, {R, 7}, {R, 7}, {R, 7}, {}, {R, 7}, {R, 7}, {}, {}, {} },//13
    { {}, {R, 4}, {R, 4}, {R, 4}, {R, 4}, {}, {R, 4}, {R, 4}, {}, {}, {} },//14
    { {}, {R, 5}, {R, 5}, {R, 5}, {R, 5}, {}, {R, 5}, {R, 5}, {}, {}, {} }//15
    };

int getSymbolIndex(int *symbol) {
    for (int i = 0; i < sizeof(columns) / sizeof(columns[0]); i++) {
        if (columns[i].symbol == *symbol) {
            return columns[i].column;
        }
    }
    return -1;
}

Action readAction(int row, int column){
    Action action = table[row][column];
    return action;
}

void addResult(int ipt) {
    result[size_result++] = ipt;
}

int getLastResult() {
    return result[size_result];
}

int getTop(){
    return stack[top];
}

void push(int state) {
    stack[++top] = state;
}

int pop() {
    return stack[top--];
}

void popAtOnce(int num) {
    for (int i = 0; i < num; i++) {
        top--;
    }
}

void displayStack() {
    printf("[Stack contents: ");
    
    for (int i = 0; i <= top; i++) {
        int state = stack[i];
        printf("%d ", state);
    }
    
    printf("]");
}


int *parse(int *ipt){
    Action action;
    int prefix;
    int state;
    int next_state;
    int length_syntax;//such as E+T is 3
    int sym;
    int non_terminal;
    action = readAction(2, 9);
    push(0);
    //printf("[%d]", *ipt);
    action = readAction(getTop(), getSymbolIndex(ipt));
    for (int i = 0; i < 20; i++) {
        prefix = action.SRGA;
        //printf("[%d]", *ipt);
        
        printf("\n");
        if (prefix == 11) printf("S");
        if (prefix == 12) printf("R");
        if (prefix == 13) printf("G");
        printf("%d", action.Item);
        displayStack();
        
        //printf("[%d]", *ipt);
        if (prefix == S) {
            next_state = action.Item;
            push(next_state);
            
            action = readAction(getTop(), getSymbolIndex(++ipt));
            continue;
        }
        if (prefix == R) {
            state = action.Item;
            addResult(state);
            length_syntax = productions[state].numChar;
            
            popAtOnce(length_syntax);
            non_terminal = productions[state].left;
            
            action = readAction(getTop(), getSymbolIndex(&non_terminal));
            continue;
        }
        if (prefix == G) {
            next_state = action.Item;
            push(next_state);
            
            action = readAction(getTop(), getSymbolIndex(ipt));
            continue;
        }
        if (prefix == ACC) {
            printf("ACCEPTED");
            break;
        }
    }
    return result;
}

int main(){
    int input[] = {L_PARENTHESES, NUM, ADD, NUM, R_PARENTHESES, DIV, NUM};
    int *input_ptr = input;
    int *parsed = parse(input_ptr);
    printf("/n");
    return 0;
}

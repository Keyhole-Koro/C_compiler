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
};

char *productions[] = {
    "E->E$"
    "E->E+T",
    "E->E-T",
    "E->T",
    "T->T*F",
    "T->T/F",
    "T->F",
    "F->(E)",
    "F->id"
};

int numProductions = 9;

int *symbols[] = {NUM, ADD, SUB, MUL, DIV, L_PARENTHESES, R_PARENTHESES};
int numSymbols = 7;

int stack[100];
int top = -1;

int getTop(){
    return top;
}

void push(int state) {
    stack[++top] = state;
}

int pop() {
    return stack[top--];
}

int getSymbolIndex(int symbol) {
    for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); i++) {
        if (symbols[i] == symbol) {
            return i;
        }
    }
    return -1;
}

char *table[][17] = {
//    id   +  -  *  /  (  )  $  E  T  F
    {"s5","","","","","s2","","","1","3","4"},
    {"","s6","s7","","","","","","acc","",""},
    {"s5","","","","","","","","8","3","4"},
    {"","r4","r4","r9","r10","","r4","r4","","",""},
    {"","r7","r7","r7","r7","","r7","r7","","",""},
    {"","r9","r9","r9","r9","","r9","r9","","",""},
    {"s5","","","","","s2","","","","11","4"},
    {"s5","","","","","s2","","","","12","4"},
    {"","s6","s8","","","","s13","","","",""},
    {"s5","","","","","s2","","","","","14"},
    {"s5","","","","","","s2","","","","15"},
    {"","r2","r2","s9","s10","","r2","r2","","",""},
    {"","r3","r3","s9","s10","","r3","r3","","",""},
    {"","r8","r8","r8","r8","","r8","r8","","",""},
    {"","r5","r5","r5","r5","","r5","r5","","",""},
    {"","r6","r6","r6","r6","","r6","r6","","",""}
};

char *readAction(char *ipt){
    int row = getTop();//state
    int column = getSymbolIndex(ipt);//symbol
    char *action = table[row][column];
    ipt++;
    return action;
}

int *parse(int *ipt){
    char *action;
    int nextState;
    push(0);
    while(*ipt) {
        action = readAction(ipt++);
        if (action[0] == "s") {
            nextState = action[1];
            push(nextState);
            continue;
        } else if (action[0] == "r") {

        } else if (action[0] == "a") {

        }
    }
}

int main(){
    int *input[] = {L_PARENTHESES, NUM, ADD, NUM, R_PARENTHESES, DIV, NUM};
    int parsed = parse(input);
    return 0;
}
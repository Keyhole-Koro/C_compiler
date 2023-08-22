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
};

typedef struct {
    char *left;
    char *right;
    int numChar;
    int columnTable;
} Production;

typedef struct {
    int *column;
    int symbol;
}

Production productions[] = {
    {"E", "E$", 2, 8},
    {"E", "E+T", 3, 8},
    {"E", "E-T", 3, 8},
    {"E", "T",1 , 8},
    {"T", "T*F", 3, 9},
    {"T", "T/F", 3, 9},
    {"T", "F", 1, 9},
    {"F", "(E)", 3, 10},
    {"F", "num", 1, 10}
    };



int *symbols[] = {NUM, ADD, SUB, MUL, DIV, L_PARENTHESES, R_PARENTHESES};
int numSymbols = 7;

int stack[100];
int top = -1;

int result[] = {};
int size_result = 0;



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

bool isNum(char *letter) {
	if (*letter < '0' || *letter > '9') return true;
	return false;
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

int getSymbolIndex(int symbol) {
    for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); i++) {
        if (symbols[i] == symbol) {
            return i;
        }
    }
    return -1;
}

char *readAction(char *ipt){
    int row = getTop();//state
    int column = getSymbolIndex(ipt);//symbol
    char *action = table[row][column];
    ipt++;
    return action;
}

char *removePrefix(char *ipt) {
    int length = strlen(ipt);
    char *removed_char = ipt[1, length-1];//maybe it starts with 1
    return removed_char;
}

void addResult(int *ipt) {
    result[size_result++] = ipt;
}

int getLateResult() {
    return result[size_result];
}

void popAtOnce(int num) {
    for (int i = 0; i < num; i++) {
        top--;
    }
}

int *parse(int *ipt){
    char *action;
    char *prefix;
    int next_state;
    int nlp;
    char *non_terminal;
    push(0);
    while(*ipt) {
        action = readAction(ipt);
        prefix = action[0];//head of action like r s , goto is gonna be number
        if (*prefix == "s") {
            next_state = removePrefix(action) - '0';
            push(next_state);
            ipt++;
            continue;
        } else if (*prefix == "r") {
            addResult(removePrefix(action) - '0');
            nlp = productions[getLateResult()].numChar;
            popAtOnce(nlp);
            action = table[getTop()][productions[getLateResult()].columnTable];
        } else if (isNum(prefix)) {
            next_state = removePrefix(action) - '0';
            push(next_state);
        }else if (*prefix == "a") {

        }
    }
}

int main(){
    int *input[] = {L_PARENTHESES, NUM, ADD, NUM, R_PARENTHESES, DIV, NUM};
    int parsed = parse(input);
    return 0;
}
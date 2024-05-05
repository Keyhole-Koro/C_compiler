#ifndef ASM_LABEL_H
#define ASM_LABEL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug.h"

typedef enum {
    LABEL_BRANCH,
    LABEL_TRUE_BRANCH,
    LABEL_FALSE_BRANCH,
    LABEL_RELAY,
    LABEL_CONT,
    LABEL_STRING
} LabelKind;

typedef struct {
    LabelKind kind;
    char *name;
} Label;


Label *makeBranchLabel();
Label *makeTrueBranchLabel();
Label *makeFalseBranchLabel();
Label *makeRelayLabel();
Label *makeContLabel();

void startLabel(Label *label);
void jumpTo(Label *label);

#endif
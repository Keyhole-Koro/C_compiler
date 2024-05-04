#ifndef ASM_LABEL_H
#define ASM_LABEL_H

typedef enum {
    LABEL_BRANCH,
    LABEL_STRING
} LabelKind;

typedef struct {
    LabelKind kind;
    char *name;
} Label;


Label *makeBranchLabel();
void startLabel(Label *label);
void jumpTo(Label *label);

#endif
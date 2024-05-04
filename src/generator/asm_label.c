#include "asm_label.h"

char *label_branch = ".BRANCH.";
int cur_num_branch = 0;

char *label_str = ".STR.";
int cur_num_str = 0;

Label *makeBranchLabel() {
    return makeLabel(LABEL_BRANCH);
}

Label *makeLabel(LabelKind kind) {
    Label *newLabel = malloc(sizeof(Label));
    newLabel->kind = kind;
    int id = -1;
    char *label_prefix = NULL;
    switch (kind) {
        case LABEL_BRANCH:
            id = cur_num_branch++;
            label_prefix = label_branch;
        case LABEL_STR:
            id = cur_num_str++;
            label_prefix = label_str;
        default:
            DEBUG_PRINT("Unsupported label kind\n");
            exit(1);
    }
    char *name = strcat(label_prefix, id + '0'); 
    newLabel->name = name;

    return newLabel;
}

void startLabel(Label *label) {
    printf("%s:\n", label->name);
}

void jumpTo(Label *label) {
    printf("   jmp %s\n", label->name);
}


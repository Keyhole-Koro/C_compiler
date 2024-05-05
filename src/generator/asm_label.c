#include "asm_label.h"

Label *makeLabel(LabelKind kind);

char *label_branch = ".BRANCH.";
int cur_num_branch = 0;

char *label_true_branch = ".BRANCH.TRUE.";
int cur_num_true_branch = 0;

char *label_false_branch = ".BRANCH.FALSE.";
int cur_num_false_branch = 0;

char *label_relay = ".RELAY.";
int cur_num_relay = 0;

char *label_cont = ".CONT.";
int cur_num_cont = 0;

char *label_str = ".STR.";
int cur_num_str = 0;

Label *makeBranchLabel() {
    return makeLabel(LABEL_BRANCH);
}

Label *makeTrueBranchLabel() {
    return makeLabel(LABEL_TRUE_BRANCH);
}

Label *makeFalseBranchLabel() {
    return makeLabel(LABEL_FALSE_BRANCH);
}

Label *makeRelayLabel() {
    return makeLabel(LABEL_RELAY);
}

Label *makeContLabel() {
    return makeLabel(LABEL_CONT);
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
            break;
        case LABEL_TRUE_BRANCH:
            id = cur_num_true_branch++;
            label_prefix = label_true_branch;
            break;
        case LABEL_FALSE_BRANCH:
            id = cur_num_false_branch++;
            label_prefix = label_false_branch;
            break;
        case LABEL_RELAY:
            id = cur_num_relay++;
            label_prefix = label_relay;
            break;
        case LABEL_CONT:
            id = cur_num_cont++;
            label_prefix = label_cont;
            break;
        case LABEL_STRING:
            id = cur_num_str++;
            label_prefix = label_str;
            break;
        default:
            DEBUG_PRINT("Unsupported label kind\n");
            exit(1);
    }

    char *name = malloc(strlen(label_prefix) + 9 + 1);
    sprintf(name, "%s%d", label_prefix, id);

    newLabel->name = name;

    return newLabel;
}

void startLabel(Label *label) {
    printf("\n");
    printf("%s:\n", label->name);
}

void jumpTo(Label *label) {
    printf("    jmp %s\n", label->name);
}


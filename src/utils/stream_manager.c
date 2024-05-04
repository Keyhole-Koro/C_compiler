#include "stream_manager.h"

void switchToPrintf() {
    write = printf;
}

void switchToLabel() {
    write = print_label;
}

int print_branch_label(const char *fmt, ...) {
    va_list args;    
    sprintf(, fmt, args);
    // no meaning
    return 1;
}

int print_loop_label(const char *fmt, ...) {
    va_list args;    
    registerLabel(&branch_label, instruction_buf);
    // no meaning
    return 1;
}

int print_str_label(const char *fmt, ...) {
    va_list args;
    char *buffer = malloc(strlen(instruction_buf));
    sprintf(buffer, fmt, args);

    
    // no meaning
    return 1;
}

Label *registerLabel(Label **target, char *buffer) {
    Label *newLabel = malloc(sizeof(Label));
    newLabel->id = cur_num_condition++;
    newLabel->buf = buffer;
    newLabel->next = NULL;
    (*target)->next = newLabel;
    (*target) = newLabel;
    return newLabel;
}
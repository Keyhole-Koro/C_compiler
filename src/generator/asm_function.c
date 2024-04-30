#include "function.h"

void funcGen(Node *func) {
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, %d\n", getStackFrameSize(func));

    Node *func_details = func->left;
    Node *params = func_details->right;
    int nTh_param = 1;
    for (Node *prm = params; prm; prm = prm->right) {
        (prm->left);
        num_param++;
    }
}

int getStackFrameSize(Node *func) {
    Node *func_details = func->left;
    Node *result = func_details->left;
    Node *frame_size = result->left;
    return frame_size->value.natural;
}

void declarePrm(Node *var, int nth_param) {
    printf("    mov [rbp - %d], %s",getVarOffset(var), getParamRegister(nth_param, getVarSize(var)));
}
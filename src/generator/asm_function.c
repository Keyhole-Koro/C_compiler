#include "asm_function.h"

void returnGen(Node *func);
int getStackFrameSize(Node *func);
Node *getReturnNode(Node *func);

void paramGen(Node *params) ;
void paramGen_(Node *params, int nth_arg);

void callFuncGen(Node *callFunc);
void callFuncGen_(Node *callFunc, int nth_arg);
void argGen(Node *arg, int nth_arg);

char *main_start = "_start";

void returnSuccess();

int findNearestMod16(int num);

void funcGen(Node *func) {
    expectNode(func, AST_FUNCTION);

    int frame_size = findNearestMod16(getStackFrameSize(func)) + 8;

    char *functionName = func->value.str;
    printf("%s:\n", (strcmp(functionName, "main") == 0) ? main_start : functionName);
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, %d\n", frame_size);

    Node *func_details = func->left;
    Node *params = func_details->right;
    Node *stmt = func->right;
    
    paramGen(params);

    stmtGen(stmt);

    if (strcmp(functionName, "main") == 0) {
        printf("    xor rax, rax\n");
        printf("    mov rdi, fmt\n");
        printf("    mov esi, dword [rbp - 36]\n");
        printf("    call printf\n");
        printf("\n");
    }

    returnGen(func);
   
    printf("    add rsp, %d\n", frame_size);
    printf("    pop rbp\n");
    if (strcmp(functionName, "main") == 0) returnSuccess();
    else printf("    ret\n");

    printf("\n");
}

void returnGen(Node *func) {
    Node *return_node = getReturnNode(func);
    exprGen(return_node->left);
}

int getStackFrameSize(Node *func) {
    Node *func_details = func->left;
    Node *result = func_details->left;
    Node *frame_size = result->left;
    return frame_size->value.natural;
}

Node *getReturnNode(Node *func) {
    Node *func_details = func->left;
    Node *result = func_details->left;
    return result->right;
}

int getReturnSize(Node *func) {
    Node *return_node = getReturnNode(func);
    Node *type = return_node->right;
    return type->left->value.natural;
}

void paramGen(Node *params) {
    paramGen_(params, 1);
}

void paramGen_(Node *param, int nth_arg) {
    if (!param) return;

    Node *declare = param->left;
    Node *var = declare->left;
    int size = getVarSize(var);
    printf("    mov %s [rbp - %d], %s\n",
        getWord(size), getVarOffset(var), getParamRegister(nth_arg, size));
    paramGen_(param->right, ++nth_arg);
}
/*
typedef struct {
    Node *func;
    int nth_arg;
    struct CallFunc *next;
    struct CallFunc *prev;
} CallFunc;

CallFunc *exploreCallFunc(Node *callFunc, CallFunc *latest);

void callFuncGen(Node *callFunc) {
    expectNode(callFunc, AST_CALL_FUNC);

    CallFunc head = {callFunc, -1, NULL, NULL};

    exploreCallFunc(callFunc, &head);

    CallFunc *cur_cf = &head;
    while (cur_cf->next) {
        cur_cf = cur_cf->next;
    }

    for (CallFunc *cf = cur_cf; cf; cf = cf->prev) {
        callFuncGen_(cf->func, cf->nth_arg);
    }
}

CallFunc *exploreCallFunc(Node *callFunc, CallFunc *prev) {
    if (!callFunc) return prev;

    expectNode(callFunc, AST_CALL_FUNC);

    CallFunc *head = NULL;
    CallFunc *cur_latest = NULL;
    int num_cf = 0;
    int cur_arg = 1;
    for (Node *arg = callFunc->right; arg; arg = arg->right) {
        if (arg->left->type == AST_CALL_FUNC) {
            CallFunc *new_cf = malloc(sizeof(CallFunc));
            new_cf->func = arg->left;
            new_cf->nth_arg = cur_arg;

            new_cf->prev = prev;
            prev->next = new_cf;

            prev = new_cf;
            if (!head) head = new_cf;

            num_cf++;
        }
        cur_arg++;
    }

    CallFunc *despatch_cf = head;
    for (int i = 0; i < num_cf; i++) {
        cur_latest = exploreCallFunc(despatch_cf->func, cur_latest);
        despatch_cf = despatch_cf->next;
    }

    return prev;
}
*/

void callFuncGen(Node *callFunc) {
    callFuncGen_(callFunc, 1);
}

void callFuncGen_(Node *callFunc, int nth_arg) {
    if (!callFunc) return;
    expectNode(callFunc, AST_CALL_FUNC);

    Node *arg = callFunc->right;

    argGen(arg, 1);

    char *functionName = callFunc->value.str;
    if (strcmp(functionName, "main") == 0) {
        DEBUG_PRINT("calling main()\n");
        exit(1);
    }

    printf("    call %s\n", functionName);

    /*
    if (nth_arg == -1) return;
    Node *type = NULL;
    if ((type = getFuncType(callFunc->left))) {
        int size = type->left->value.natural;
        // eax temporary
        printf("    mov %s, eax\n", getParamRegister(nth_arg, size));
    }
    */
}

void argGen(Node *arg, int nth_arg) {
    if (!arg) return;

    expectNode(arg, AST_ARG);

    if (arg->left->type == AST_VARIABLE || arg->left->type == AST_NUMBER) {

        Node *var = arg->left;
        int size = getVarSize(var);
        printf("    mov %s, %s [rbp - %d]\n",
                getParamRegister(nth_arg, size), getWord(size), getVarOffset(var));
    
    }
    argGen(arg->right, nth_arg + 1);
}


void returnSuccess() {
    printf("    mov rax, 60\n");
    printf("    xor rdi, rdi\n");
    printf("    syscall\n");
    printf("\n");
}

int isMultipleOf16(int num) {
    return (num % 16) == 0;
}

int adjustToMod16(int num) {
    return num + (16 - (num % 16)) % 16;
}

int findNearestMod16(int num) {
    if (isMultipleOf16(num + 1)) {
        return num + 1;
    } else {
        int adjusted_num = adjustToMod16(num + 1);
        int adjusted_num_prev = adjustToMod16(num);
        return (adjusted_num - num) < (num - adjusted_num_prev) ? adjusted_num : adjusted_num_prev;
    }
}
#include "function.h"

Func func = {NULL, NULL, NULL, NULL};
Func *head_functions = &func;

Node *parameterNode(Token **cur, int *cur_offset, Var *vars);
Node *returnNode(Token **cur, Var *vars, Type *type);

Func *registerFunc(char *name, Type *type, Node *func_details);
Func *findFunc(char *expectedName);
Node *callFunctionNode(Token **cur, Var *vars);
Node *argNode(Token **cur, Var *vars);

Node *functionNode(Token **cur) {
    Var *localVars = calloc(1, sizeof(Var));
    int offset = 0;

    Type *type = NULL;
    if (!(type = isType(*cur))) {
        DEBUG_PRINT("not type %s\n", revertToken(*cur));
        exit(1);
    }
    consume(cur);

    if ((*cur)->kind != IDENTIFIER) {
        DEBUG_PRINT("not identifier\n");
        exit(1);
    }

    Token *func_idtfr = consume(cur);
    char *functionName = func_idtfr->value;

    Node *function = createStringNode(AST_FUNCTION, functionName);
    Node *func_details = createNode(AST_FUNCTION_DETAILS);

    registerFunc(functionName, type, func_details);
    
    expect(*cur, L_PARENTHESES);
    consume(cur);

    Node *param = parameterNode(cur, &offset, localVars);
    func_details->right = param;

    function->left = func_details;

    expect(*cur, R_PARENTHESES);
    consume(cur);

    expect(*cur, L_BRACE);
    consume(cur);

    // statementNode doesn't consume RETURN
    function->right = statementNode(cur, &offset, localVars); 

    Node *result = createNode(AST_FUNCTION_RESULT);

    func_details->left = result;

    if (strcmp(type->name, "void") != 0) {
        result->right = returnNode(cur, localVars, type);
    }

    result->left = createNaturalNode(AST_STACK_FRAME_SIZE, offset);

    expect(*cur, R_BRACE);
    consume(cur);

    return function;
}

Node *parameterNode(Token **cur, int *cur_offset, Var *vars) {
    if ((*cur)->kind == R_PARENTHESES) return NULL;

    Type *type = NULL;
    if (!(type = isType(*cur))) {
        DEBUG_PRINT("not type\n");
        exit(1);
    }
    //consume(cur);

    //expect(*cur, IDENTIFIER);
    Token *variable = (*cur)->next;

    char *variableName = variable->value;

    Node *param = createStringNode(AST_PARAMETER, variableName);
    param->left = declareVariableNode(cur, type, vars, cur_offset);
    
    if ((*cur)->kind == COMMA) {
        consume(cur);
        param->right = parameterNode(cur, cur_offset, vars);
    }
    return param;
}

Node *returnNode(Token **cur, Var *vars, Type *type) {
    Node *return_node = createNode(AST_RETURN);
    expect(*cur, RETURN);
    consume(cur);
    return_node->left = exprNode(cur, vars);
    return_node->right = typeNode(type);
    expect(*cur, SEMICOLON);
    consume(cur);
    return return_node;
}

Func *registerFunc(char *name, Type *type, Node *func_details) {
    Func *newFunc = malloc(sizeof(Func));
    newFunc->name = name;
    newFunc->type = type;
    newFunc->func_details = func_details;
    newFunc->next = NULL;

    Func *func = head_functions;

    while (func->next) {
        func = func->next;
    }
    func->next = newFunc;

    return newFunc;
}

Func *findFunc(char *expectedName) {

    for (Func *cur_func = head_functions; cur_func; cur_func = cur_func->next) {
        if (!cur_func->name) continue;
        if (strcmp(cur_func->name, expectedName) == 0) return cur_func;
    }
    return NULL;
}

Node *callFunctionNode(Token **cur, Var *vars) {
    expect(*cur, IDENTIFIER);
    char *calledfuncName = (*cur)->value;

    Func *calledFunc;
    if (!(calledFunc = findFunc(calledfuncName))) {
        DEBUG_PRINT("the target function not found [%s]\n", calledfuncName);
        exit(1);
    }

    Node *callFunc = createStringNode(AST_CALL_FUNC, calledfuncName);
    
    consume(cur);

    // take this line carefully when you add syntax error handler
    // if calledFunc was not found, pass this part otherwise, segmentation fault
    // calledFunc->type
    callFunc->left = calledFunc->func_details;
    expect(*cur, L_PARENTHESES);
    consume(cur);
    callFunc->right = argNode(cur, vars);

    expect(*cur, R_PARENTHESES);
    consume(cur);
    
    return callFunc;
}

Node *argNode(Token **cur, Var *vars) {
    if ((*cur)->kind == R_PARENTHESES) return NULL;
    expect(*cur, IDENTIFIER);
    char *variableName = (*cur)->value;

    Node *param = createStringNode(AST_ARG, variableName);

    if ((*cur)->next->kind == L_PARENTHESES) {
        param->left = callFunctionNode(cur, vars);
    } else {
        param->left = variableNode(cur, vars);
    }

    if ((*cur)->kind == R_PARENTHESES) return param;
     
    expect(*cur, COMMA);
    consume(cur);
    param->right = argNode(cur, vars);
    
    return param;
}
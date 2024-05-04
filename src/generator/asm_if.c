#include "asm_if.h"

/**
    https://www.philadelphia.edu.jo/academics/qhamarsheh/uploads/Lecture%2018%20Conditional%20Jumps%20Instructions.pdf
*/


void ifGen(Node *if_node, Var *vars) {
    Label *trueBranch = makeBranchLabel();
    Label *falseBranch = makeBranchLabel();

    Node *next = cur->right;

    if (next->type == AST_ELSE_IF || next->type == AST_ELSE) {
        startLabel(trueBranch);
        conditional_stmtGen(cur->left, trueBranch);
        startLabel(falseBranch);
        ifGen(next, vars);

    } else {
        /**
            @brief when branch closes just after `if` or `else if`
        */
        conditional_stmtGen(cur->left, trueBranch);
        jumpTo(outOfBlock);
    }

    startLabel(trueBranch);
    stmtGen(cur->left->right);
}

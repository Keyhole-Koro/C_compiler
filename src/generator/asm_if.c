#include "asm_if.h"

// defined in statement.c
void stmtGen(Node *root);

/**
    https://www.philadelphia.edu.jo/academics/qhamarsheh/uploads/Lecture%2018%20Conditional%20Jumps%20Instructions.pdf
*/

void ifGen_(Node *if_node, Label *ifBranch, Label *outOfIfBlock);


void ifGen(Node *if_node) {
    Label *ifBranch = makeBranchLabel();
    Label *outOfIfBlock = makeContLabel();
    jumpTo(ifBranch);
    ifGen_(if_node, ifBranch, outOfIfBlock);
}

void ifGen_(Node *if_node, Label *ifBranch, Label *outOfIfBlock) {

    Label *trueBranch = makeTrueBranchLabel();
    Label *falseBranch = makeFalseBranchLabel();

    Node *next = if_node->right;

    if (if_node->type == AST_ELSE) {
        /**
            @brief when branch closes just after `if` or `else if`
        */
        //conditionGen(if_node->left, trueBranch, falseBranch);
        startLabel(ifBranch);
        stmtGen(if_node->left->right);
        jumpTo(outOfIfBlock);

        startLabel(outOfIfBlock);

    } else if (next->type == AST_ELSE_IF || next->type == AST_ELSE) {
        
        startLabel(ifBranch);
        conditionGen(if_node->left, trueBranch, falseBranch);
        jumpTo(falseBranch);
        
        startLabel(trueBranch);
        stmtGen(if_node->left->right);
        jumpTo(outOfIfBlock);

        ifGen_(next, falseBranch, outOfIfBlock);
    }    

}

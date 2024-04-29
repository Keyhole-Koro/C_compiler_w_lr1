#include "statement.h"

Node *statementNode_(Token **cur, int cur_offset, Var *vars);

Node *statementNode(Token **cur) {
    Var *localVars = calloc(1, sizeof(Var));
    int offset = 0;
    return statementNode_(cur, offset, localVars);
}

Node *statementNode_(Token **cur, int cur_offset, Var *vars) {
    if ((*cur)->kind == EOT) return NULL;

    DEBUG_PRINT("11\n");
    Node *stmt = createNode(AST_STATEMENT);

    //if ((Type *type = isModifier(&cur))) {

    Type *type = NULL;
    if ((type = isType(*cur))) {
        // int i or int i = 0
        stmt->left = declareVariableNode(cur, type, &vars, &cur_offset);
        DEBUG_PRINT("12\n");

    } else if ((*cur)->kind == IDENTIFIER) {
        // i = 0
        DEBUG_PRINT("var %s\n", vars->name);
        if ((*cur)->next->kind == ASSIGN) stmt->left = assignNode(cur, variableNode(cur, vars));
     
    }
    DEBUG_PRINT("13 %s\n", revertToken((*cur)->next->next));

    expect(*cur, SEMICOLON);
    consume(cur); // SEMICOLON

    stmt->right = statementNode_(cur, cur_offset, vars);
    DEBUG_PRINT("14\n");

    return stmt;
}
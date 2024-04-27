#include "expr.h"

#include <stdbool>

bool isOperator(AST_Type type) {
    switch (type){
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
            return true;
        default:
            return false;
    }
}

Node *functor(Token **cur) {
    Node *new_node = NULL;
    if ((*cur)->kind == NUMBER) {
        new_node = createNode(AST_NUMBER, (*cur)->value);
        *cur = (*cur)->next;
    } else if ((*cur)->kind == L_PARENTHESES) {
        *cur = (*cur)->next;
        new_node = expr(cur);
        if (new_node && isOperator(new_node->type)
                && (*cur) && (*cur)->kind == R_PARENTHESES) {
            *cur = (*cur)->next;
            return new_node;
        } else {
            fprintf(stderr, "Nothing inside ()\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Unexpected token\n");
        exit(1);
    }
    return new_node;
}

Node *term(Token **cur) {
    if (!(*cur)) return NULL;

    Node *new_node = NULL;

    new_node = functor(cur);

    while (*cur && ((*cur)->kind == MUL || (*cur)->kind == DIV)) {
        Token *op = *cur;
        *cur = (*cur)->next;
        Node *op_node = createNode((op->kind == MUL) ? AST_MUL : AST_DIV, NULL);
        op_node->left = new_node;
        op_node->right = functor(cur);
        new_node = op_node;
    }

    return new_node;
}

Node *expr(Token **cur) {
    if (!(*cur)) return NULL;

    Node *new_node = term(cur);

    while (*cur && ((*cur)->kind == ADD || (*cur)->kind == SUB)) {
        Token *op = *cur;
        *cur = (*cur)->next;
        Node *op_node = createNode((op->kind == ADD) ? AST_ADD : AST_SUB, NULL);
        op_node->left = new_node;
        op_node->right = term(cur);
        new_node = op_node;
    }

    return new_node;
}
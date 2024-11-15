// Node.h
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "token.h"  // Include the Token header if it's a separate file

// Forward declarations for the Node-based structures
struct TermNode;
struct ExprPrimeNode;
struct TermPrimeNode;
struct FactorNode;

// Abstract base class for all nodes in the parse tree
struct Node {
    virtual void print() const = 0;  // Pure virtual function for printing
    virtual ~Node() = default;       // Virtual destructor to ensure proper cleanup
};

// ExprNode Class
struct ExprNode : public Node {
    TermNode* term;
    ExprPrimeNode* expr_prime;

    ExprNode(TermNode* term, ExprPrimeNode* expr_prime);

    void print() const override;
};

// ExprPrimeNode Class
struct ExprPrimeNode : public Node {
    Token op;
    TermNode* term;
    ExprPrimeNode* expr_prime;
    bool end_of_file;

    ExprPrimeNode(Token op, TermNode* term, ExprPrimeNode* expr_prime, bool eof);

    void print() const override;
};

// TermNode Class
struct TermNode : public Node {
    FactorNode* factor;
    TermPrimeNode* term_prime;

    TermNode(FactorNode* factor, TermPrimeNode* term_prime);

    void print() const override;
};

// TermPrimeNode Class
struct TermPrimeNode : public Node {
    Token op;
    FactorNode* factor;
    TermPrimeNode* term_prime;
    bool end_of_file;

    TermPrimeNode(Token op, FactorNode* factor, TermPrimeNode* term_prime, bool eof);

    void print() const override;
};

// FactorNode Class
struct FactorNode : public Node {
    Token number;
    ExprNode* expr;
    bool is_number;

    FactorNode(Token number);
    FactorNode(ExprNode* expr);

    void print() const override;
};

#endif // NODE_H

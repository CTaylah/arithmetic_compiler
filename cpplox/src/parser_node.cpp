// Node.cpp
#include "parser_node.h"

// Helper function to print with indentation for tree structure
void printIndented(int level) {
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";  // Indentation for each level
    }
}

// ExprNode Class
ExprNode::ExprNode(TermNode* term, ExprPrimeNode* expr_prime)
    : term(term), expr_prime(expr_prime) {}

void ExprNode::print() const {
    printIndented(0);
    std::cout << "ExprNode(";
    term->print();
    if (expr_prime) {
        std::cout << " ";
        expr_prime->print();
    }
    std::cout << ")";
}

// ExprPrimeNode Class
ExprPrimeNode::ExprPrimeNode(Token op, TermNode* term, ExprPrimeNode* expr_prime, bool eof)
    : op(op), term(term), expr_prime(expr_prime), end_of_file(eof) {}

void ExprPrimeNode::print() const {
    if (end_of_file) {
        return;  // Do not print anything if it's end of file (ε)
    }

    printIndented(1);
    std::cout << "ExprPrimeNode(" << op.toString() << " ";
    term->print();
    if (expr_prime) {
        std::cout << " ";
        expr_prime->print();
    }
    std::cout << ")";
}

// TermNode Class
TermNode::TermNode(FactorNode* factor, TermPrimeNode* term_prime)
    : factor(factor), term_prime(term_prime) {}

void TermNode::print() const {
    printIndented(1);
    std::cout << "TermNode(";
    factor->print();
    if (term_prime) {
        std::cout << " ";
        term_prime->print();
    }
    std::cout << ")";
}

// TermPrimeNode Class
TermPrimeNode::TermPrimeNode(Token op, FactorNode* factor, TermPrimeNode* term_prime, bool eof)
    : op(op), factor(factor), term_prime(term_prime), end_of_file(eof) {}

void TermPrimeNode::print() const {
    if (end_of_file) {
        return;  // Do not print anything if it's end of file (ε)
    }

    printIndented(2);
    std::cout << "TermPrimeNode(" << op.toString() << " ";
    factor->print();
    if (term_prime) {
        std::cout << " ";
        term_prime->print();
    }
    std::cout << ")";
}

// FactorNode Class
FactorNode::FactorNode(Token number)
    : number(number), is_number(true) {}

FactorNode::FactorNode(ExprNode* expr)
    : expr(expr), is_number(false) {}

void FactorNode::print() const {
    printIndented(2);
    std::cout << "FactorNode(";
    if (is_number) {
        std::cout << "Number: " << number.toString();
    } else {
        std::cout << "Expr: ";
        expr->print();
    }
    std::cout << ")";
}

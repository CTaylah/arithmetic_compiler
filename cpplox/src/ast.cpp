#include "parser_node.h"
#include "ast.h"
#include <iostream>

// Helper function to print indentation
void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
}

ScalarNode::ScalarNode(Token number) : number(number) {}

void ScalarNode::print(int indent) const {
    print_indent(indent);
    std::cout << "ScalarNode: " << number.toString() << " ";
}

BinaryOpNode::BinaryOpNode(ASTNodeType lhs, ASTNodeType rhs, Token op)
    : lhs(std::move(lhs)), op(op), rhs(std::move(rhs)) {}

void BinaryOpNode::print(int indent) const {
    print_indent(indent);
    std::cout << "(BinaryOpNode: " << op.toString() << " ";
    std::visit([indent](auto&& arg) { arg->print(indent + 1); }, lhs);
    std::cout << "";
    std::visit([indent](auto&& arg) { arg->print(indent + 1); }, rhs);
    std::cout << ")";
}


ASTNodeType node_from_expr(ExprNode* expr){
    auto lhs = node_from_term(expr->term);
    if(expr != nullptr && expr->expr_prime == nullptr){
        return lhs;
    }

    Token op = expr->expr_prime->op;
    auto rhs = node_from_expr_prime(expr->expr_prime);
    return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);

}

ASTNodeType node_from_expr_prime(ExprPrimeNode* expr_prime){
    ASTNodeType lhs = node_from_term(expr_prime->term);
    if(expr_prime->expr_prime == nullptr){
        return lhs;
    }

    Token op = expr_prime->expr_prime->op;
    auto rhs = node_from_expr_prime(expr_prime->expr_prime);
    return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);

}

ASTNodeType node_from_term(TermNode* term){
    auto lhs = node_from_factor(term->factor);

    if(term->term_prime == nullptr){
        return lhs;
    }
    Token op = term->term_prime->op;
    auto rhs = node_from_term_prime(term->term_prime);
    return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);

}

ASTNodeType node_from_term_prime(TermPrimeNode* term_prime){
    ASTNodeType lhs = node_from_factor(term_prime->factor);
    if(term_prime->term_prime == nullptr){
        return lhs;
    }

    Token op = term_prime->term_prime->op;
    ASTNodeType rhs = node_from_term_prime(term_prime->term_prime);
    return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);
}

ASTNodeType node_from_factor(FactorNode* factor){
    if(factor->is_number){
        return std::make_unique<ScalarNode>(factor->number);
    }
    else return node_from_expr(factor->expr);
}

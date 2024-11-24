#pragma once

#include "parser_node.h"
#include <memory>
#include <variant>

struct ScalarNode;
struct BinaryOpNode;


using ASTNodeType = std::variant<std::unique_ptr<BinaryOpNode>, std::unique_ptr<ScalarNode>>;
// union ASTNodeType{
//     std::unique_ptr<BinaryOpNode> binary_op;
//     std::unique_ptr<ScalarNode> scalar;
//     ASTNodeType(std::unique_ptr<BinaryOpNode> binary_op):binary_op(std::move(binary_op)){}
//     ASTNodeType(std::unique_ptr<ScalarNode> scalar):scalar(std::move(scalar)){}
// };


struct ScalarNode{
    Token number;

    void print(int indent) const;
    ScalarNode() = default;
    ~ScalarNode() = default;
    ScalarNode(Token number);
};

struct BinaryOpNode{
    Token op;
    std::variant<std::unique_ptr<BinaryOpNode>, std::unique_ptr<ScalarNode>> lhs;
    std::variant<std::unique_ptr<BinaryOpNode>, std::unique_ptr<ScalarNode>> rhs;

    void print(int indent) const;
    BinaryOpNode() = default;
    ~BinaryOpNode() = default;
    BinaryOpNode(ASTNodeType lhs, ASTNodeType rhs, Token op);
};



ASTNodeType node_from_expr(ExprNode* expr);
ASTNodeType node_from_expr_prime(ExprPrimeNode* expr_prime);
ASTNodeType node_from_term(TermNode* term);
ASTNodeType node_from_term_prime(TermPrimeNode* term_prime);
ASTNodeType node_from_factor(FactorNode* factor);

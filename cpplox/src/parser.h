#include <iostream>
#include "scanner.h"
#include "parser_node.h"
#include "ast.h"
#include "three_address_code.h"
#include "register_alloc.h"
#include "assembly.h"

/*
expr_node
- term_node
- expr_prime_node

expr_prime_node
- operator (+ or -)
- term_node
- expr_prime_node

term_node
- factor_node
- term_prime_node

term_prime_node
- operator (* or /)
- factor_node
- term_prime_node

factor_node
- number_node or expr_node
*/

class Parser{
private:
    std::vector<Token> tokens;
    Token current_word;
    int index;

    inline void advance(){
        if (index + 1 < tokens.size()){
            index += 1;
            current_word = tokens[index];
        }
        else{throw std::runtime_error("PARSE_ADVANCE ERROR: End of file");}
    }

    inline Token peek(){
        if (index + 1 < tokens.size()){
            return tokens[index + 1];
        }
        else{throw std::runtime_error("PARSE_PEEK ERROR: End of file");}
    }

    inline void Fail(int label){
        std::cout << "Error on token:" << current_word.toString() << "\n";
        std::cout << label;
        exit(1);
         
    }

    inline ExprNode* Expr(){
        TermNode* term = Term();
        ExprPrimeNode* expr_prime = ExprPrime();
        return new ExprNode(term, expr_prime);
    }

    inline ExprPrimeNode* ExprPrime(){
        if(current_word.getType() == TokenType::PLUS || current_word.getType() == TokenType::MINUS){
            Token word_copy = current_word;
            advance();
            TermNode* term = Term();
            ExprPrimeNode* expr_prime = ExprPrime();
            return new ExprPrimeNode(word_copy, term, expr_prime, false);
        }
        else if (current_word.getType() == TokenType::END_OF_FILE){
            //This seems fishy
            return nullptr;
        }
        else{
            return nullptr;
        }
    }

    inline TermNode* Term(){
        FactorNode* factor = Factor();
        TermPrimeNode* term_prime = TermPrime();
        return new TermNode(factor, term_prime);
    }

    inline TermPrimeNode* TermPrime(){
        if(current_word.getType() == TokenType::STAR || current_word.getType() == TokenType::SLASH){
            Token word_copy = current_word;
            advance();
            FactorNode* factor = Factor();
            TermPrimeNode* term_prime = TermPrime();
            return new TermPrimeNode(word_copy, factor, term_prime, false);
            
        }
        else if(current_word.getType() == TokenType::END_OF_FILE){
            return nullptr;
        }
        else{
            return nullptr;
        }
    }

    inline FactorNode* Factor(){
        if(current_word.getType() == TokenType::NUMBER){
            FactorNode* factor = new FactorNode(current_word);
            advance();
            return factor;
        }
        else if(current_word.getType() == TokenType::LEFT_PAREN){
            advance();
            ExprNode* expr = Expr();
            if(current_word.getType() == TokenType::RIGHT_PAREN){
                FactorNode* factor = new FactorNode(expr);
                advance();
                return factor;
            }
            else{
                Fail(1);
            }
        }
        else{
            Fail(2);
        }
    }

public:
    inline Parser(std::vector<Token> tokens){
        this->tokens = tokens; 
        this->index=0;
        current_word = tokens[0];
        }
    inline void parse(){
        ExprNode* expr = Expr();
        expr->print();
        std::cout << "\n";
        ASTNodeType node = node_from_expr(expr);
        std::visit([](auto&& arg) { arg->print(0); }, node);
        std::cout << "\n";

        TACInstructions tac(std::move(node));
        create_file(tac);
    }
};

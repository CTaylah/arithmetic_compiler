#include <iostream>
#include "scanner.h"

class ExprNode{
public:
    virtual ~ExprNode(){}
};

class BinaryExprNode : public ExprNode{
    public:
        ExprNode left;
        Token op;
        ExprNode right;

        BinaryExprNode(ExprNode left, Token op, ExprNode right):
            left(left), op(op), right(right){}
};

class LiteralExpr : public ExprNode{
    public:
        Token value;

        LiteralExpr(Token value):
            value(value){}
};

class Parser{
private:
    std::vector<Token> tokens;
    int index;

    void Fail(Token t, int line){
        std::cout << "Error on token:" << t.toString() << "\n";
        std::cout << line;
        exit(1);
         
    }

    inline bool Expr(){
        if (Term()){
            return ExprPrime();
        }
        else{
            Fail(tokens[index], 1);
            return false;
        }

    }

    inline bool ExprPrime(){
        if(tokens[index].getType() == TokenType::PLUS || tokens[index].getType() == TokenType::MINUS){
            index += 1;
            if(Term()){
                return ExprPrime();
            }
            else{
                Fail(tokens[index], 2);
            }
        }
        else if(tokens[index].getType() == TokenType::END_OF_FILE || tokens[index].getType() == TokenType::RIGHT_PAREN){
            return true;
        }
        else Fail(tokens[index], 3);
        return false;
    }

    inline bool Term(){
        // literal1 = Factor()
        if(Factor()){
            return TermPrime();
        }
        else Fail(tokens[index], 4);
        return false;
    }

    inline bool TermPrime(){
        if(tokens[index].getType() == TokenType::STAR || tokens[index].getType() == TokenType::SLASH){
            index += 1;
            if(Factor()){
                return TermPrime();
            }
            else{
                Fail(tokens[index], 5);
                return false;
            }
        }

        else if(tokens[index].getType() == TokenType::END_OF_FILE || tokens[index].getType() == TokenType::PLUS 
        || tokens[index].getType() == TokenType::MINUS || tokens[index].getType() == TokenType::RIGHT_PAREN){
            return true;
        }
        Fail(tokens[index], 6);
        return false;
    }

    inline bool Factor(){
        if(tokens[index].getType() == TokenType::LEFT_PAREN){
            index += 1;

            if(!Expr()) 
               Fail(tokens[index], 7);
            if (tokens[index].getType() != TokenType::RIGHT_PAREN)
                Fail(tokens[index], 8);

            index += 1;
            return true;
        }
        else if (tokens[index].getType() == TokenType::IDENTIFIER || tokens[index].getType() == TokenType::NUMBER){
            index += 1;
            return true;
        }

        else Fail(tokens[index], 9);
        return false;
    }

public:
    inline Parser(std::vector<Token> tokens){this->tokens = tokens;}
    inline void parse(){
        index = 0;
        if (Expr()){
            if (tokens[index].getType() == TokenType::END_OF_FILE){
                std::cout << "success \n";
                return;
            }
            else{
                Fail(tokens[index], 10);
            }
        }
        else{
            Fail(tokens[index], 11);
        }
    }

};

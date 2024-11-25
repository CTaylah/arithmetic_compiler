#include "ast.h"
#include <string>
#include <optional>
#include <vector>

struct TAC{
    std::string arg1;
    std::optional<std::string> arg2;
    std::string result;
    std::optional<std::string> op;

    TAC(std::string arg1, int index)
        :op(std::nullopt), arg1(arg1), arg2(std::nullopt), result(result)
            {result = "t" + std::to_string(index);}

    TAC(std::string arg1, std::string arg2, int index, std::string op)
        :op(op), arg1(arg1), arg2(arg2) 
        {result = "t" + std::to_string(index);}

    void print() const{
        std::cout << result << " = " << arg1 << " " << op.value_or("") << " " << arg2.value_or("") << "\n";
    }

    std::string toString() const{
        return result + " = " + arg1 + " " + op.value_or("") + " " + arg2.value_or("") + "\n";
    }

};

class TACInstructions{
    std::vector<TAC> instructions;
    int index = 0;

public:
    TACInstructions(ASTNodeType head){
        tac_from_ast(std::move(head));
        for(TAC tac : instructions){
            tac.print();
        }
    }

    inline void tac_from_ast(ASTNodeType head){
        if (std::holds_alternative<std::unique_ptr<ScalarNode>>(head)){
            auto scalar_node = std::get_if<std::unique_ptr<ScalarNode>>(&head);
            std::string arg1 = scalar_node->get()->number.getLiteralString();
            add_instruction(TAC(arg1, index));
            return;
        }
        else{
            auto binop_node = std::get_if<std::unique_ptr<BinaryOpNode>>(&head);
            if(binop_node == nullptr){
                throw std::runtime_error("Line 37 three_address_code. nullptr.");
            }
            TAC _ = tac_from_binop(std::move(*binop_node));
        }
    }

private:
    inline void add_instruction(TAC tac){
        instructions.push_back(tac);
        index++;
    }

    inline TAC tac_from_binop(std::unique_ptr<BinaryOpNode> node){
        std::string arg1 = "Unknown";
        std::string arg2 = "Unknown";
        std::string op = node.get()->op.getLexeme();

        //Check lhs
        {
            ASTNodeType lhs = std::move(node.get()->lhs);
            //Check if lhs is a Scalar or BinOp

            if(std::holds_alternative<std::unique_ptr<ScalarNode>>(lhs)){
                auto scalar_node = std::get_if<std::unique_ptr<ScalarNode>>(&lhs);
                arg1 = scalar_node->get()->number.getLiteralString();
            }
            else{
                auto binary_op_node = std::move(std::get<std::unique_ptr<BinaryOpNode>>(lhs));
                arg1 = tac_from_binop(std::move(binary_op_node)).result;
            }
        }

        //Check rhs
        {
            ASTNodeType rhs = std::move(node.get()->rhs);
            //Check if lhs is a Scalar or BinOp

            if(std::holds_alternative<std::unique_ptr<ScalarNode>>(rhs)){
                auto scalar_node = std::get_if<std::unique_ptr<ScalarNode>>(&rhs);
                arg2 = scalar_node->get()->number.getLiteralString();
            }
            else{
                auto binary_op_node = std::move(std::get<std::unique_ptr<BinaryOpNode>>(rhs));
                arg2 = tac_from_binop(std::move(binary_op_node)).result;
            }
        }
        //build TAC and push to instructions

        TAC tac(arg1, arg2, index, op);
        add_instruction(tac);
        return tac;
        //increment index
    }
};

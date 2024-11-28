#pragma once

#include "three_address_code.h"
#include <iostream>
#include <optional>
#include <fstream>

inline std::string op_to_instruction(std::optional<std::string> op){
    std::string operation = "";

    if(op.has_value()){
       operation = op.value();
    }
    else {
        operation = "=";
        return "mov";
    }

    if(operation == "+") return "add";
    else if(operation == "-") return "sub";
    // else if(operation == "*") return "imul";
    // else if(operation == "/") return "idiv";
    // else if(operation == "x") return "subl";
    else throw(1);

    return "";
}

inline void generate_assembly(TACInstructions tac_instructions, std::ofstream& file){
    std::string reg("esi");
    while(tac_instructions.is_empty() == false){
        TAC tac = tac_instructions.pop();
        std::string asm_instruction = op_to_instruction(tac.op);

    }
}

inline void create_file(){
    std::ofstream out_file("assembly.s");

    if(!out_file){
        std::cerr << "Couldn't open file" << std::endl;
        return;
    }

    // Write boiler plate
    out_file << ".section .data \n \n";
    out_file << ".section .text \n \n";
    out_file << ".globl _start \n \n";
    out_file << "_start: \n \n";

}

#pragma once

#include "register_alloc.h"
#include <iostream>
#include <optional>
#include <fstream>
#include <map>

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
    else throw(1);

    return "";
}

inline void generate_assembly(TACInstructions tac_instructions, std::ofstream& file){

    std::map<std::string, int> variable_color_map = get_register_map(tac_instructions, 4);
    std::map<std::string, std::string> register_map;

    for(const auto& [variable, color] : variable_color_map){
        int reg_number = color + 7;
        std::string reg = "r" + std::to_string(reg_number);
        register_map[variable] = reg;
    }

    while(!tac_instructions.is_empty()){
        TAC tac = tac_instructions.pop();

        if(is_variable(tac.arg1)){
            file << "mov " << register_map[tac.arg1] << ", " << register_map[tac.result] << "\n \n";
        }
        else{
            file << "mov " << tac.arg1 << ", " << register_map[tac.result] << "\n \n";
        }
                
        if(tac.arg2.has_value()){
            std::string asm_instruction = op_to_instruction(tac.op);
            if(is_variable(tac.arg2)){
                file << asm_instruction << " " << register_map[tac.arg2.value()] << ", " << register_map[tac.result] << "\n \n";
            }
            else{
                file << asm_instruction << " " << tac.arg2.value() << ", " << register_map[tac.result] << "\n \n";
            }
                
        }
    }
}

inline void create_file(TACInstructions& tac_instructions){
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

    generate_assembly(tac_instructions, out_file);

}

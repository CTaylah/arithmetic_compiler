#include <iostream>
#include "scanner.h"
#include "parser.h"

int main(int argc, char** argv) {
	std::string lox_source = "3 + ( (2) + 3 + (((3)))) * 4";
	Scanner scanner(lox_source);
	std::vector<Token> tokens = scanner.scanTokens();

	Parser parser(tokens);

	for(Token token : tokens){
		std::cout << token.toString() << std::endl;
	}

	parser.parse();

}


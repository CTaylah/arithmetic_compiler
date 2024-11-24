#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "ast.h"

int main(int argc, char** argv) {
	std::string lox_source = "5 * (3 + 2)";
	Scanner scanner(lox_source);
	std::vector<Token> tokens = scanner.scanTokens();

	Parser parser(tokens);

	for(Token token : tokens){
		std::cout << token.toString() << std::endl;
	}

	parser.parse();

}


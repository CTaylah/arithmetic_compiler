#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "assembly.h"
#include "ast.h"


int main(int argc, char** argv) {
	std::string lox_source = "(3 * 3) + (23 + (32 + 23)) - 523";
	Scanner scanner(lox_source);
	std::vector<Token> tokens = scanner.scanTokens();

	Parser parser(tokens);

	for(Token token : tokens){
		std::cout << token.toString() << std::endl;
	}

	parser.parse();

	create_file();

}

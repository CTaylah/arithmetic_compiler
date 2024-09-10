#include <iostream>
#include "scanner.h"

int main(int argc, char** argv) {
	std::string lox_source = "var x = 5;";
	Scanner scanner(lox_source);
	std::vector<Token> tokens = scanner.scanTokens();

	for(Token token : tokens){
		std::cout << token.toString() << std::endl;
	}
}
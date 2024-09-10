#pragma once

#include "token_type.h"
#include <variant>
#include <string>

class Token{
private:
	TokenType type;
	std::string lexeme;
	std::variant<int, float, std::string> literal;
	int line;
	std::string toString(std::variant<int, float, std::string> literal);
public:

	Token(TokenType type, std::string lexeme,
		std::variant<int, float, std::string> literal, int line);

	std::string toString();
	
};



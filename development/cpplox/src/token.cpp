
#include "token.h"
#include "token_type.h"

#include <variant>
//When we bundle a lexeme with other useful data, we call it a token.
Token::Token(TokenType type, std::string lexeme, std::variant<int, float, std::string> literal, int line) {
	this->type = type;
	// lexemes are raw substrings of the source code
	this->lexeme = lexeme;
	this->literal = literal;
	this->line = line;
}

std::string Token::toString(std::variant<int, float, std::string> literal) const {
	if (std::holds_alternative<int>(literal)) {
		return std::to_string(std::get<int>(literal));
	} else if (std::holds_alternative<float>(literal)) {
		return std::to_string(std::get<float>(literal));
	} else {
		return std::get<std::string>(literal);
	}
}

std::string Token::toString() const {
	return token_type_to_string(type) + " " + lexeme + " " + toString(literal);
}

std::variant<int, float, std::string> Token::getLiteral(){
	return literal;
}

std::string Token::getLiteralString(){
	if(std::holds_alternative<int>(literal)){
		return std::to_string(std::get<int>(literal));
	}
	else if(std::holds_alternative<float>(literal)){
		return std::to_string(std::get<float>(literal));
	}
	else{
		return std::get<std::string>(literal);
	}
}

std::string Token::getLexeme(){
	return lexeme;
}

TokenType Token::getType(){
	return type;
}
#pragma once

#include <vector>
#include <string>
#include <map>

#include "token_type.h"
#include "token.h"

class Scanner{
private:
	std::string source;
	std::vector<Token> tokens;

    int start = 0;
    int current = 0;
    int line = 1;

	void scanToken();
	bool isAtEnd();
	char advance();
	void addToken(TokenType type);
	void addToken(TokenType type, std::variant<int, float, std::string> literal);

	bool match(char expected);
	char peek();
	char peekNext();

	void string();
	void number();
	void identifier();

	std::map<std::string, TokenType> keywords = {
		{"and", TokenType::AND},
		{"class", TokenType::CLASS},
		{"else", TokenType::ELSE},
		{"false", TokenType::FALSE},
		{"for", TokenType::FOR},
		{"fun", TokenType::FUN},
		{"if", TokenType::IF},
		{"nil", TokenType::NIL},
		{"or", TokenType::OR},
		{"print", TokenType::PRINT},
		{"return", TokenType::RETURN},
		{"super", TokenType::SUPER},
		{"this", TokenType::THIS},
		{"true", TokenType::TRUE},
		{"var", TokenType::VAR},
		{"while", TokenType::WHILE}
	};



public:
	Scanner(std::string source): source(source){}
	std::vector<Token> scanTokens();
};

#include "scanner.h"

#include <iostream>

std::vector<Token> Scanner::scanTokens() {
    while (current < source.length()) {
        start = current;
        scanToken();
    }
    tokens.push_back(Token(TokenType::END_OF_FILE, "", "", line));
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

char Scanner::advance() {
    current++;
    return source[current - 1];
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, std::variant<int, float, std::string> literal) {
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}
char Scanner::peekNext(){
    if(current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Scanner::string(){
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        std::cout << "Unterminated string." << std::endl;
        return;
    }

    // The closing "
    advance();

    //HMM
    std::string value = source.substr(start + 1, current - 1);
    addToken(TokenType::STRING, value);
}

void Scanner::number(){

    while (isdigit(peek()) && !isAtEnd()){
        advance();
    }

    if(peek() == '.' && isdigit(peekNext())){
        advance();
        while (isdigit(peek()) && !isAtEnd()){
            advance();
        }
    }
    std::string number_string = source.substr(start, current);
    float value = std::stof(number_string);
    addToken(TokenType::NUMBER, value);
}

void Scanner::identifier(){
    while(isalnum(peek()) && !isAtEnd()){
        advance();
    }

    std::string text = source.substr(start, current);
    if (keywords.count(text) > 0){
        addToken(keywords[text]);
    } else {
        addToken(TokenType::IDENTIFIER);
    }
    
}

void Scanner::scanToken(){
    char c = advance();

    switch (c){
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        // The division symbol needs extra handling because '/' is also used for comments
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            string();
            break;
        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c)) {
                identifier();
            } else {
                std::cout << "Unexpected character: " << c << std::endl;
            }
            break;
    }
}
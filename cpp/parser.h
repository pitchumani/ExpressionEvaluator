#ifndef PARSER_H_
#define PARSER_H_

#include<string>
#include<sstream>
#include<iostream>
#include<vector>
#include<cctype>

#include "expression.h"
using namespace std;

struct Token {
    string value;
    enum class Type { NUM, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, ID,END };
    Type type;
    Token() : value(""), type(Type::NUM) {}
    Token(string s, Type t) : value(s), type(t) {}
};

class Tokenizer {
    string input;
    size_t position = 0;
public:
    Tokenizer(string input) : input(std::move(input)) {}
    Token next();
    Token peek();
    bool hasNext() const {
        return !input.empty();
    }
    void advance(size_t count = 1);
private:
    void skipWhitespace();
    Token parseNumber();
    Token parseOperator();
    Token parseParentheses();
    Token parseString();
};

class Parser {
    Tokenizer *tokenizer;
    Token currentToken;
    int getPrecedence(const Token &token);
    IExpression *parseExpression(int precedence = 0);
    IExpression *parsePrimary();
    IExpression *parseParentheses();
    IExpression *parseNumber(const Token &token);
    IExpression *parseOperator(const Token &opToken, IExpression *left, IExpression *right);
    void advance();
public:
    Parser() {
        currentToken = Token("", Token::Type::END);
    }

    IExpression *parse(std::string input);
};

#endif  //  PARSER_H_

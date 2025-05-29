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
    enum class Type { NUM, OPR, FOPR, BOPEN, BCLOSE };
    Type type;
    Token(string s, Type t) : value(s), type(t) {}
};

class Parser {
    vector<Token> tokenize(string input);

public:
    Parser() {}
    IExpression *parse(string input);
};

#endif  //  PARSER_H_

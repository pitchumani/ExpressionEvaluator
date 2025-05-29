#ifndef TEST_PARSER_H_
#define TEST_PARSER_H_

#include <cassert>
#include <iostream>
#include "parser.h"
#include "expression.h"

// Helper to evaluate an expression tree (assuming IExpression has evaluate())
double eval(IExpression* expr) {
    if (!expr) throw std::runtime_error("Null expression");
    double result = expr->evaluate();
    delete expr;
    return result;
}

void test_tokenizer_basic() {
    Tokenizer tokenizer("12 + 34");
    Token t1 = tokenizer.next();
    assert(t1.type == Token::Type::NUM && t1.value == "12");
    Token t2 = tokenizer.next();
    assert(t2.type == Token::Type::PLUS && t2.value == "+");
    Token t3 = tokenizer.next();
    assert(t3.type == Token::Type::NUM && t3.value == "34");
}

void test_tokenizer_operators() {
    Tokenizer tokenizer("1-2*3/4");
    assert(tokenizer.next().type == Token::Type::NUM);
    assert(tokenizer.next().type == Token::Type::MINUS);
    assert(tokenizer.next().type == Token::Type::NUM);
    assert(tokenizer.next().type == Token::Type::MUL);
    assert(tokenizer.next().type == Token::Type::NUM);
    assert(tokenizer.next().type == Token::Type::DIV);
    assert(tokenizer.next().type == Token::Type::NUM);
}

void test_tokenizer_parentheses() {
    Tokenizer tokenizer("(1+2)");
    assert(tokenizer.next().type == Token::Type::LPAREN);
    assert(tokenizer.next().type == Token::Type::NUM);
    assert(tokenizer.next().type == Token::Type::PLUS);
    assert(tokenizer.next().type == Token::Type::NUM);
    assert(tokenizer.next().type == Token::Type::RPAREN);
}

void test_parser_simple_add() {
    Parser parser;
    IExpression* expr = parser.parse("2+3");
    assert(eval(expr) == 5.0);
}

void test_parser_precedence() {
    Parser parser;
    IExpression* expr = parser.parse("2+3*4");
    assert(eval(expr) == 14.0);
}

void test_parser_parentheses() {
    Parser parser;
    IExpression* expr = parser.parse("(2+3)*4");
    assert(eval(expr) == 20.0);
}

void test_parser_subtract_divide() {
    Parser parser;
    IExpression* expr = parser.parse("10-6/2");
    assert(eval(expr) == 7.0);
}

void test_parser_complex() {
    Parser parser;
    IExpression* expr = parser.parse("1+2*(3+4)-5/5");
    assert(eval(expr) == 1+2*(3+4)-1);
}

void run_all_parser_tests() {
    test_tokenizer_basic();
    test_tokenizer_operators();
    test_tokenizer_parentheses();
    test_parser_simple_add();
    test_parser_precedence();
    test_parser_parentheses();
    test_parser_subtract_divide();
    test_parser_complex();
    std::cout << "All parser tests passed!" << std::endl;
}

#endif // TEST_PARSER_H_
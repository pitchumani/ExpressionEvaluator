#include <gtest/gtest.h>
#include <sstream>
#include "parser.h"

// Helper to evaluate and print expression tree as string
std::string printExpression(IExpression* expr) {
    std::ostringstream oss;
    if (expr) expr->print(oss);
    return oss.str();
}

TEST(TokenizerTest, ParseNumber) {
    Tokenizer tokenizer("12345");
    Token token = tokenizer.next();
    EXPECT_EQ(token.type, Token::Type::NUM);
    EXPECT_EQ(token.value, "12345");
}

TEST(TokenizerTest, ParseOperators) {
    Tokenizer tokenizer("+ - * /");
    EXPECT_EQ(tokenizer.next().type, Token::Type::PLUS);
    EXPECT_EQ(tokenizer.next().type, Token::Type::MINUS);
    EXPECT_EQ(tokenizer.next().type, Token::Type::MUL);
    EXPECT_EQ(tokenizer.next().type, Token::Type::DIV);
}

TEST(TokenizerTest, ParseParentheses) {
    Tokenizer tokenizer("( )");
    EXPECT_EQ(tokenizer.next().type, Token::Type::LPAREN);
    EXPECT_EQ(tokenizer.next().type, Token::Type::RPAREN);
}

TEST(TokenizerTest, SkipWhitespace) {
    Tokenizer tokenizer("   42   ");
    Token token = tokenizer.next();
    EXPECT_EQ(token.type, Token::Type::NUM);
    EXPECT_EQ(token.value, "42");
}

TEST(TokenizerTest, InvalidCharacter) {
    Tokenizer tokenizer("42$");
    Token token1 = tokenizer.next();
    Token token2 = tokenizer.next();
    EXPECT_EQ(token1.type, Token::Type::NUM);
    EXPECT_EQ(token2.type, Token::Type::END);
}

TEST(ParserTest, SimpleAddition) {
    Parser parser;
    IExpression* expr = parser.parse("2+3");
    ASSERT_NE(expr, nullptr);
    EXPECT_EQ(printExpression(expr), "(2+3)");
    delete expr;
}

TEST(ParserTest, OperatorPrecedence) {
    Parser parser;
    IExpression* expr = parser.parse("2+3*4");
    ASSERT_NE(expr, nullptr);
    EXPECT_EQ(printExpression(expr), "(2+(3*4))");
    delete expr;
}

TEST(ParserTest, Parentheses) {
    Parser parser;
    IExpression* expr = parser.parse("(2+3)*4");
    ASSERT_NE(expr, nullptr);
    EXPECT_EQ(printExpression(expr), "((2+3)*4)");
    delete expr;
}

TEST(ParserTest, NestedParentheses) {
    Parser parser;
    IExpression* expr = parser.parse("((1+2)*(3+4))");
    ASSERT_NE(expr, nullptr);
    EXPECT_EQ(printExpression(expr), "((1+2)*(3+4))");
    delete expr;
}

TEST(ParserTest, InvalidInput) {
    Parser parser;
    IExpression* expr = parser.parse("2+*3");
    EXPECT_EQ(expr, nullptr);
}

TEST(ParserTest, EmptyInput) {
    Parser parser;
    IExpression* expr = parser.parse("");
    EXPECT_EQ(expr, nullptr);
}
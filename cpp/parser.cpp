#include "parser.h"

vector<Token> Parser::tokenize(string input) {
    vector<Token> tokens;
    size_t idx = 0;
    while (idx < input.size()) {
        if (std::isspace(input[idx])) {
            ++idx;
            continue;
        } else if (isdigit(input[idx])) {
            string numstr;
            while (std::isdigit(input[idx]) && (idx < input.length())) {
                numstr += input[idx];
                ++idx;
            };
            tokens.push_back({numstr, Token::Type::NUM});
        } else if ((input[idx] == '+') || (input[idx] == '-') ||
                   (input[idx] == '*') || (input[idx] == '/')) {
            string opr;
            opr += input[idx];
            tokens.push_back({opr, Token::Type::OPR});
            ++idx;
        } else if (input[idx] == '(') {
            string opr;
            opr += input[idx];
            tokens.push_back({opr, Token::Type::BOPEN});
            ++idx;
        } else if (input[idx] == ')') {
            string opr;
            opr += input[idx];
            tokens.push_back({opr, Token::Type::BCLOSE});
            ++idx;
        } else {
            cerr << "Invalid character '" << input[idx] << "' at index "
                 << idx << "\n";
            ++idx;
        }
    }
    return tokens;
}

IExpression *Parser::parse(string input) {
    auto tokens = tokenize(input);
    cout << "Found " << tokens.size() << " tokens!\n";
    stack<std::unique_ptr<IExpression>> exprStack;
    for (size_t idx = 0; idx < tokens.size(); ++idx) {
        auto token = tokens[idx];
        switch (token.type) {
            case Token::Type::NUM:
            {
                auto exprNum = ExpressionFactory::createNumber(std::stoi(token.value));
                exprStack.push(std::move(exprNum));
            }
            break;
            case Token::Type::OPR:
            {
                if (exprStack.empty()) {
                    // TODO: add input index to the error message
                    cerr << "Invalid token '" << token.value << "'.\n";
                    break;
                }
                auto left = std::move(exprStack.top());
                exprStack.pop();
                auto ptr = dynamic_cast<NumberNode*>(left.get());
                if (!ptr) {
                    // TODO: add input index to the error message
                    cerr << "Invalid token '" << left->getTypeName() << "' before '"
                         << token.value << "'.\n";
                    break;
                }
                ++idx;
                if (idx >= tokens.size()) {
                    cerr << "Unexpected end of expression. Expected a numberic "
                         << "token after the operator '" << token.value << "'\n";
                    break;
                }
                auto nextToken = tokens[idx];
                if (nextToken.type != Token::Type::NUM) {
                    // TODO: add input index to the error message
                    cerr << "Invalid token '" << nextToken.value << "' after '"
                         << token.value << "'.\n";
                    break;
                }
                auto right = ExpressionFactory::createNumber(std::stoi(nextToken.value));
                auto exprBop = ExpressionFactory::createBinary(BinaryNode::get(token.value[0]),
                                              std::move(left),
                                              std::move(right));
                exprStack.push(std::move(exprBop));
            }
            break;
            default:
                cerr << "Unhandled token: " << token.value << " !!\n";
            break;
        }
    }
    if (!exprStack.empty()) {
        auto res = std::move(exprStack.top());
        exprStack.pop();
        auto rptr = res.get();
        res.release();
        return rptr;
    }
    return new NumberNode(tokens.size());
}

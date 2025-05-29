#include "parser.h"

void Tokenizer::skipWhitespace() {
    while (position < input.size() && std::isspace(input[position])) {
        ++position;
    }
}
Token Tokenizer::parseNumber() {
    size_t start = position;
    while (position < input.size() && std::isdigit(input[position])) {
        ++position;
    }
    return Token(input.substr(start, position - start), Token::Type::NUM);
}
Token Tokenizer::parseString() {
    size_t start = position;
    while (position < input.size() && std::isalnum(input[position])) {
        ++position;
    }
    return Token(input.substr(start, position - start), Token::Type::ID);
}

Token Tokenizer::parseOperator() {
    char op = input[position++];
    switch (op) {
        case '+': return Token("+", Token::Type::PLUS);
        case '-': return Token("-", Token::Type::MINUS);
        case '*': return Token("*", Token::Type::MUL);
        case '/': return Token("/", Token::Type::DIV);
        default:
            cerr << "Invalid operator '" << op << "' at position " << position - 1 << "\n";
            return Token("", Token::Type::END); // Return a default token
    }
}
Token Tokenizer::parseParentheses() {
    char paren = input[position++];
    if (paren == '(') {
        return Token("(", Token::Type::LPAREN);
    } else if (paren == ')') {
        return Token(")", Token::Type::RPAREN);
    } else {
        cerr << "Invalid parentheses '" << paren << "' at position " << position - 1 << "\n";
        return Token("", Token::Type::END); // Return a default token
    }
}

void Tokenizer::advance(size_t count) {
    position += count;
    skipWhitespace();
}

Token Tokenizer::peek() {
    size_t originalPosition = position;
    Token token = next();
    position = originalPosition; // Reset position after peeking
    return token;
}

Token Tokenizer::next() {
    skipWhitespace();
    if (position >= input.size()) {
        return Token("", Token::Type::END); // Return a default token
    }
    if (std::isdigit(input[position])) {
        return parseNumber();
    } else if (input[position] == '+' || input[position] == '-' ||
               input[position] == '*' || input[position] == '/') {
        return parseOperator();
    } else if (input[position] == '(' || input[position] == ')') {
        return parseParentheses();
    } else if (std::isalpha(input[position])) {
        // TODO: Handle identifiers or keywords if needed
        parseString();
        // For now, treat identifiers as NUM for simplicity
        cerr << "Identifiers are not supported yet. Treating it as number.\n";
        return Token("0", Token::Type::NUM);
    } else {
        cerr << "Invalid character '" << input[position] << "' at position " << position << "\n";
        advance(); // Skip invalid character
        return next(); // Try to get the next token
    }
}

int Parser::getPrecedence(const Token &token) {
    switch (token.type) {
        case Token::Type::PLUS:
        case Token::Type::MINUS:
            return 1;
        case Token::Type::MUL:
        case Token::Type::DIV:
            return 2;
        default:
            return 0; // For NUM, LPAREN, RPAREN
    }
}

IExpression *Parser::parseNumber(const Token &token) {
    try {
        return ExpressionFactory::createNumber(std::stoi(token.value));
    } catch (const std::invalid_argument &e) {
        cerr << "Invalid number: " << token.value << "\n";
        return nullptr;
    }
}
IExpression *Parser::parseParentheses() {
    if (currentToken.type != Token::Type::LPAREN) {
        cerr << "Expected '(', but found '" << currentToken.value << "'\n";
        return nullptr;
    }
    advance(); // Skip the '('
    auto expr = parseExpression(0); // Parse the expression inside parentheses
    if (!expr) {
        return nullptr; // Error already reported in parseExpression
    }
    if (currentToken.type != Token::Type::RPAREN) {
        cerr << "Expected ')', but found '" << currentToken.value << "'\n";
        return nullptr;
    }
    advance(); // Skip the ')'
    return expr;
}
IExpression *Parser::parsePrimary() {
    if (currentToken.type == Token::Type::NUM) {
        return parseNumber(currentToken);
    } else if (currentToken.type == Token::Type::LPAREN) {
        return parseParentheses();
    } else if (currentToken.type == Token::Type::END) {
        return nullptr; // No more tokens to parse
    } else {
        cerr << "Unexpected token '" << currentToken.value << "' while parsing primary expression.\n";
        return nullptr;
    }
}

void Parser::advance() {
    if (tokenizer->hasNext()) {
        currentToken = tokenizer->next();
    } else {
        currentToken = Token("", Token::Type::END); // Set to END if no more tokens
        cerr << "No more tokens available.\n";
    }
}

IExpression *Parser::parseOperator(const Token &opToken, IExpression *left, IExpression *right) {
    if (!left || !right) {
        cerr << "Invalid operands for operator '" << opToken.value << "'.\n";
        return nullptr;
    }
    auto op = BinaryNode::get(opToken.value[0]);
    return ExpressionFactory::createBinary(op, left, right);
}

IExpression *Parser::parseExpression(int precedence) {
    if (!tokenizer->hasNext()) {
        cerr << "No tokens available to parse.\n";
        return nullptr;
    }
    currentToken = tokenizer->next();
    auto left = parsePrimary();
    if (!left) {
        return nullptr; // Error already reported in parsePrimary
    }
    while (tokenizer->hasNext()) {
        auto opToken = tokenizer->peek(); // peek, don't consume the token
        int currentPrecedence = getPrecedence(opToken);
        if (currentPrecedence < precedence) {
            break; // Stop if the operator has lower precedence
        }
        if (opToken.type == Token::Type::RPAREN || opToken.type == Token::Type::END) {
            break; // End of expression
        }
        tokenizer->advance(); // Consume the operator token
        auto right = parseExpression(currentPrecedence + 1);
        if (!right) {
            cerr << "Error parsing right operand after operator '" << opToken.value << "'.\n";
            return nullptr; // Error already reported in parseExpression
        }
        left = parseOperator(opToken, left, right);
        if (!left) {
            cerr << "Error creating binary expression for operator '" << opToken.value << "'.\n";
            return nullptr; // Error already reported in parseOperator
        }
    }
    return left;
}

IExpression *Parser::parse(std::string input) {
    if (input.empty()) {
        cerr << "Input is empty.\n";
        return nullptr;
    }
    tokenizer = new Tokenizer(std::move(input));
    // currentToken = tokenizer->next();
    auto res = parseExpression(0);
    if (res) {
        cout << endl;
        res->print(cout);
        cout << endl;
    }
    delete tokenizer;
    return res;
}

#if 0
    cout << "Found " << lexicalTokens.size() << " tokens!\n";
    stack<IExpression*> optrStack;
    for (size_t idx = 0; idx < lexicalTokens.size(); ++idx) {
        auto token = lexicalTokens[idx];
        switch (token.type) {
            case Token::Type::NUM:
            {
                // Create a NumberNode and push it onto the stack
                auto exprNum = parseNumber(token);
                if (exprNum) {
                    optrStack.push(exprNum);
                } else {
                    // Error already reported in parseNumber
                }
            }
            break;
            case Token::Type::LPAREN:
            {
                auto exprParen = parseParentheses(tokens, idx);
                if (exprParen) {
                    optrStack.push(exprParen);
                } else {
                    // Error already reported in parseParentheses
                }
            }
            break;
            case Token::Type::PLUS:
            case Token::Type::MINUS:
            case Token::Type::MUL:
            case Token::Type::DIV:
            {
                if (optrStack.empty()) {
                    cerr << "Invalid operator '" << token.value << "' at index "
                         << idx << ". No left operand available.\n";
                    continue;
                }
                auto right = std::move(optrStack.top());
                optrStack.pop();
                auto left = parsePrimary(tokens, idx);
                if (!left) {
                    // Error already reported in parsePrimary
                    continue;
                }
                auto exprBop = parseOperator(token, left, right);
                if (optrStack.empty()) {
                    cerr << "Invalid expression after operator '" << token.value << "'.\n";
                    continue;
                }
                optrStack.push(std::move(exprBop));
            }
            break;
            default:
                cerr << "Unhandled token: " << token.value << " !!\n";
                break;
        }
    }
    // If we have any remaining operators, we should have a valid expression
    if (optrStack.empty()) {
        cerr << "No valid expression found after parsing all tokens.\n";
        return nullptr;
    }
    // If we have a valid expression, return it
    return finalizeExpression(optrStack, tokens);
#endif

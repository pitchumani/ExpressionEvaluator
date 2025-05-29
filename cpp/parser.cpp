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
        auto id = parseString();
        // For now, treat identifiers as NUM for simplicity
        //cerr << "Identifiers are not supported yet: " << id.value << "\n";
        return Token(id.value, Token::Type::ID);
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
    return expr;
}

IExpression *Parser::parsePrimary() {
    //cout << "Parsing primary expression: " << endl;
    //cout << "Current token: " << currentToken.value << endl;    
    if (currentToken.type == Token::Type::NUM) {
        return parseNumber(currentToken);
    } else if (currentToken.type == Token::Type::LPAREN) {
        return parseParentheses();
    } else if (currentToken.type == Token::Type::ID) {
        cerr << "Identifiers are not supported yet: " << currentToken.value << "\n";
        advance(); // Skip the identifier
        return nullptr; // Return nullptr for unsupported identifiers
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
    //cout << "Parsing expression with precedence: " << precedence << endl;
    auto left = parsePrimary();
    if (!left) return nullptr;

    // fetch the next token
    advance();
    while (true) {
        int currentPrecedence = getPrecedence(currentToken);
        if (currentPrecedence <= 0 || currentPrecedence < precedence) {
            break;
        }

        Token opToken = currentToken;
        advance(); // consume operator

        auto right = parseExpression(currentPrecedence); // use same precedence for left-associativity
        if (!right) {
            cerr << "Error parsing right operand after operator '" << opToken.value << "'.\n";
            return nullptr;
        }

        left = parseOperator(opToken, left, right);
        if (!left) {
            cerr << "Error creating binary expression for operator '" << opToken.value << "'.\n";
            return nullptr;
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
    advance(); // Initialize the first token
    //cout << "first token: " << currentToken.value << endl;
    auto res = parseExpression(0);
    if (res) {
        res->print(cout);
        cout << endl;
    }
    delete tokenizer;
    return res;
}

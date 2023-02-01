#include "./parser.hpp"

namespace Blaze {

Parser::Parser(Lexer &l)
    : lexer(l) {
    nextToken();
    nextToken();
    precedences[TokenType::PLUS] = Precedence::SUM;
    precedences[TokenType::MINUS] = Precedence::SUM;
    precedences[TokenType::STAR] = Precedence::PRODUCT;
    precedences[TokenType::SLASH] = Precedence::PRODUCT;
    precedences[TokenType::CARET] = Precedence::REPEAT;
    prefixHooks[TokenType::INTEGER] = &Parser::parseInteger;
    prefixHooks[TokenType::FLOAT] = &Parser::parseFloat;
    prefixHooks[TokenType::DICE] = &Parser::parseDice;
    prefixHooks[TokenType::LPAREN] = &Parser::parseGroupedExpr;
    prefixHooks[TokenType::PLUS] = &Parser::parsePrefixExpr;
    prefixHooks[TokenType::MINUS] = &Parser::parsePrefixExpr;
    infixHooks[TokenType::PLUS] = &Parser::parseInfixExpr;
    infixHooks[TokenType::MINUS] = &Parser::parseInfixExpr;
    infixHooks[TokenType::STAR] = &Parser::parseInfixExpr;
    infixHooks[TokenType::SLASH] = &Parser::parseInfixExpr;
    infixHooks[TokenType::CARET] = &Parser::parseInfixExpr;
}

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = lexer.NextToken();
}

bool Parser::curTokenIs(TokenType t) {
    return curToken.Type == t;
}

bool Parser::peekTokenIs(TokenType t) {
    return peekToken.Type == t;
}

bool Parser::expectPeek(TokenType t) {
    if (peekTokenIs(t)) {
        nextToken();
        return true;
    }
    throw Exception("Expected %1%, Found %2%", t, peekToken);
}

Precedence Parser::curPrecedence() {
    if (precedences.find(curToken.Type) != precedences.end()) return precedences.at(curToken.Type);
    return Precedence::LOWEST;
}

Precedence Parser::peekPrecedence() {
    if (precedences.find(peekToken.Type) != precedences.end()) return precedences.at(peekToken.Type);
    return Precedence::LOWEST;
}

std::unique_ptr<AST::Root> Parser::ParseRoot() {
    auto root = std::make_unique<AST::Root>();
    auto event = parseEvent();
    if (event != nullptr) root->Event = std::move(event);
    expectPeek(TokenType::EOF_);
    return root;
}

std::unique_ptr<AST::Event_> Parser::parseEvent() {
    return parseRoll();
}

std::unique_ptr<AST::Roll> Parser::parseRoll() {
    auto roll = std::make_unique<AST::Roll>();
    roll->Token = curToken;
    roll->Expr = parseExpr(Precedence::LOWEST);
    return roll;
}

std::unique_ptr<AST::Expr_> Parser::parseExpr(Precedence p) {
    if (prefixHooks.find(curToken.Type) == prefixHooks.end()) {
        throw Exception("No prefix parse fn for %1%", curToken);
    }
    auto prefix = prefixHooks[curToken.Type];
    std::unique_ptr<AST::Expr_> leftx = std::invoke(prefix, this);
    while (!peekTokenIs(TokenType::EOF_) && p < peekPrecedence()) {
        if (infixHooks.find(peekToken.Type) == infixHooks.end()) return leftx;
        auto infix = infixHooks[peekToken.Type];
        nextToken();
        leftx = std::invoke(infix, this, leftx);
    }
    return leftx;
}

std::unique_ptr<AST::Expr_> Parser::parseInteger() {
    auto intl = std::make_unique<AST::Integer>();
    intl->Token = curToken;
    intl->Value = stoll(curToken.Literal);
    return intl;
}

std::unique_ptr<AST::Expr_> Parser::parseFloat() {
    auto floatl = std::make_unique<AST::Float>();
    floatl->Token = curToken;
    floatl->Value = stod(curToken.Literal);
    return floatl;
}

std::unique_ptr<AST::Expr_> Parser::parseDice() {
    auto dicel = std::make_unique<AST::Dice>();
    dicel->Token = curToken;
    auto d = curToken.Literal.find_first_of('d');
    const std::string nDices = curToken.Literal.substr(0, d);
    if (nDices == "0") throw Exception("Declared 0 Dices %1%", curToken);
    const std::string nFaces = curToken.Literal.substr(d + 1, curToken.Literal.size());
    if (nFaces == "0") throw Exception("Declared 0 faces Dice %1%", curToken);
    dicel->nDices = nDices.empty() ? 1 : stoull(nDices);
    dicel->nFaces = stoull(nFaces);
    return dicel;
}

std::unique_ptr<AST::Expr_> Parser::parseInfixExpr(std::unique_ptr<AST::Expr_> &l) {
    auto expr = std::make_unique<AST::Infix>();
    expr->Token = curToken;
    expr->Operator = curToken.Literal;
    expr->Left = std::move(l);
    auto preced = curPrecedence();
    nextToken();
    expr->Right = parseExpr(preced);
    return expr;
}

std::unique_ptr<AST::Expr_> Parser::parseGroupedExpr() {
    nextToken();
    auto expr = parseExpr(Precedence::LOWEST);
    if (!expectPeek(TokenType::RPAREN)) return nullptr;
    expr->expl = true;
    return expr;
}

std::unique_ptr<AST::Expr_> Parser::parsePrefixExpr() {
    auto expr = std::make_unique<AST::Prefix>();
    expr->Token = curToken;
    expr->Operator = curToken.Literal;
    nextToken();
    expr->Right = parseExpr(Precedence::PREFIX);
    return expr;
}

} // namespace Blaze

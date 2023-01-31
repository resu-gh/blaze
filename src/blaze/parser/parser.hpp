#pragma once
#include "../ast/ast.hpp"
#include "../except/except.hpp"
#include "./precedence.hpp"
#include <map>

namespace Blaze {

class Parser final {
    using prefixHookFn = std::unique_ptr<AST::Expr_> (Parser::*)();
    using infixHookFn = std::unique_ptr<AST::Expr_> (Parser::*)(std::unique_ptr<AST::Expr_> &);

  private:
    Lexer &lexer;
    Token curToken;
    Token peekToken;
    std::map<TokenType, prefixHookFn> prefixHooks;
    std::map<TokenType, infixHookFn> infixHooks;
    std::map<TokenType, Precedence> precedences;

    void nextToken();
    bool curTokenIs(TokenType);
    bool peekTokenIs(TokenType);
    bool expectPeek(TokenType);
    Precedence curPrecedence();
    Precedence peekPrecedence();
    std::unique_ptr<AST::Event_> parseEvent();
    std::unique_ptr<AST::Roll> parseRoll();
    std::unique_ptr<AST::Expr_> parseExpr(Precedence);
    std::unique_ptr<AST::Expr_> parseGroupedExpr();
    std::unique_ptr<AST::Expr_> parseInfixExpr(std::unique_ptr<AST::Expr_> &);
    std::unique_ptr<AST::Expr_> parsePrefixExpr();
    std::unique_ptr<AST::Expr_> parseInteger();
    std::unique_ptr<AST::Expr_> parseFloat();
    std::unique_ptr<AST::Expr_> parseDice();

  public:
    explicit Parser(Lexer &);

    std::unique_ptr<AST::Root> ParseRoot();
};

} // namespace Blaze

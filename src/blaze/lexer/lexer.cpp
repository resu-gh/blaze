#include "./lexer.hpp"

namespace Blaze {

Lexer::Lexer(const std::string &i)
    : input(i)
    , pos(0)
    , rpos(0)
    , curr(0)
    , blanks{' ', '\t', '\r', '\n'} {
    readChar();
}

void Lexer::readChar() {
    curr = rpos >= input.size() ? static_cast<char>(0) : input[rpos];
    pos = rpos;
    rpos++;
}

Token Lexer::NextToken() {
    Token token;
    skipBlanks();
    switch (curr) {
    case '\0': token = make(TokenType::EOF_, ""); break;
    case '+': token = make(TokenType::PLUS, "+"); break;
    case '-': token = make(TokenType::MINUS, "-"); break;
    case '*': token = make(TokenType::STAR, "*"); break;
    case '/': token = make(TokenType::SLASH, "/"); break;
    case '(': token = make(TokenType::LPAREN, "("); break;
    case ')': token = make(TokenType::RPAREN, ")"); break;
    case '^': token = make(TokenType::CARET, "^"); break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': return makeDiceOrNumber(); break;
    case 'd': return makeDice(); break;
    default: token = make(TokenType::ILLEGAL_, std::string{curr}); break;
    }
    readChar();
    return token;
}

void Lexer::skipBlanks() {
    while (find(blanks.begin(), blanks.end(), curr) != blanks.end()) readChar();
}

bool Lexer::isDigit(char c) {
    return '0' <= c && c <= '9';
}

Token Lexer::make(TokenType t, const std::string &l) {
    Token token;
    token.Type = t;
    token.Literal = l;
    return token;
}

Token Lexer::makeDice() {
    uint64_t pre = pos;
    readChar(); // skip `d`
    if (!isDigit(curr)) return make(TokenType::ILLEGAL_, extractSubstr(pre, pos));
    while (isDigit(curr)) readChar();
    return make(TokenType::DICE, extractSubstr(pre, pos));
}

Token Lexer::makeDiceOrNumber() {
    uint64_t pre = pos;
    while (isDigit(curr)) readChar();
    switch (curr) {
    case 'd':
        readChar(); // skip `d`
        if (!isDigit(curr)) return make(TokenType::ILLEGAL_, extractSubstr(pre, pos));
        while (isDigit(curr)) readChar();
        return make(TokenType::DICE, extractSubstr(pre, pos));
        break;
    case '.':
        readChar(); // skip `.`
        if (!isDigit(curr)) return make(TokenType::ILLEGAL_, extractSubstr(pre, pos));
        while (isDigit(curr)) readChar();
        return make(TokenType::FLOAT, extractSubstr(pre, pos));
        break;
    default: return make(TokenType::INTEGER, extractSubstr(pre, pos)); break;
    }
}

std::string Lexer::extractSubstr(uint32_t li, uint32_t ri) {
    return std::string(input.begin() + li, input.begin() + ri);
}

} // namespace Blaze

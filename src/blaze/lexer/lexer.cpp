#include "./lexer.hpp"

namespace blaze {

lexer::lexer(const std::string &i)
    : input(i)
    , pos(0)
    , rpos(0)
    , curr(0)
    , blanks{' ', '\t', '\r', '\n'} {
    read_char();
}

void lexer::read_char() {
    curr = rpos >= input.size() ? static_cast<char>(0) : input[rpos];
    pos = rpos;
    rpos++;
}

tok::token lexer::next_token() {
    tok::token token;
    skip_blanks();
    switch (curr) {
    case '\0': token = make(tok::type::EOF_, ""); break;
    case '+': token = make(tok::type::PLUS, "+"); break;
    case '-': token = make(tok::type::MINUS, "-"); break;
    case '*': token = make(tok::type::STAR, "*"); break;
    case '/': token = make(tok::type::SLASH, "/"); break;
    case '(': token = make(tok::type::LPAREN, "("); break;
    case ')': token = make(tok::type::RPAREN, ")"); break;
    case '^': token = make(tok::type::CARET, "^"); break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': return make_dice_or_num(); break;
    case 'd': return make_dice(); break;
    default: token = make(tok::type::ILLEGAL_, std::string{curr}); break;
    }
    read_char();
    return token;
}

void lexer::skip_blanks() {
    while (find(blanks.begin(), blanks.end(), curr) != blanks.end()) read_char();
}

bool lexer::is_digit(char c) {
    return '0' <= c && c <= '9';
}

tok::token lexer::make(tok::type t, const std::string &l) {
    tok::token token;
    token.type = t;
    token.literal = l;
    return token;
}

tok::token lexer::make_dice() {
    const auto pre = pos;
    read_char(); // skip `d`
    if (!is_digit(curr)) return make(tok::type::ILLEGAL_, extract_substr(pre, pos));
    while (is_digit(curr)) read_char();
    return make(tok::type::DICE, extract_substr(pre, pos));
}

tok::token lexer::make_dice_or_num() {
    const auto pre = pos;
    while (is_digit(curr)) read_char();
    switch (curr) {
    case 'd':
        read_char(); // skip `d`
        if (!is_digit(curr)) return make(tok::type::ILLEGAL_, extract_substr(pre, pos));
        while (is_digit(curr)) read_char();
        return make(tok::type::DICE, extract_substr(pre, pos));
        break;
    case '.':
        read_char(); // skip `.`
        if (!is_digit(curr)) return make(tok::type::ILLEGAL_, extract_substr(pre, pos));
        while (is_digit(curr)) read_char();
        return make(tok::type::FLOAT, extract_substr(pre, pos));
        break;
    default: return make(tok::type::INTEGER, extract_substr(pre, pos)); break;
    }
}

std::string lexer::extract_substr(uint32_t li, uint32_t ri) {
    return std::string(input.begin() + li, input.begin() + ri);
}

} // namespace blaze

#pragma once
#include "./token.hpp"
#include <algorithm>
#include <vector>

namespace Blaze {

class Lexer final {
  private:
    std::string input;
    uint64_t pos;
    uint64_t rpos;
    char curr;
    std::vector<char> blanks;

    void readChar();
    void skipBlanks();
    bool isDigit(char);
    Token make(TokenType, const std::string &);
    Token makeDice();
    Token makeDiceOrNumber();
    std::string extractSubstr(uint32_t, uint32_t);

  public:
    explicit Lexer(const std::string &);

    Token NextToken();
};

} // namespace Blaze

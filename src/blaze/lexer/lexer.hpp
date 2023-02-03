#pragma once // NOLINT(llvm-header-guard)
#include "./token.hpp"
#include <algorithm>
#include <vector>

namespace blaze {

class lexer final {
  private:
    std::string input;
    uint32_t pos;
    uint32_t rpos;
    char curr;
    std::vector<char> blanks;

    void read_char();
    void skip_blanks();
    bool is_digit(char);
    tok::token make(tok::type, const std::string &);
    tok::token make_dice();
    tok::token make_dice_or_num();
    std::string extract_substr(uint32_t, uint32_t);

  public:
    explicit lexer(const std::string &);

    tok::token next_token();
};

} // namespace blaze

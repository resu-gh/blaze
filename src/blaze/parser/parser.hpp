#pragma once
#include "../ast/ast.hpp"
#include "../except/except.hpp"
#include "./precedence.hpp"
#include <map>

namespace blaze {

class parser final {
    using prefix_hook_fn = std::unique_ptr<ast::expr_> (parser::*)();
    using infix_hook_fn = std::unique_ptr<ast::expr_> (parser::*)(std::unique_ptr<ast::expr_> &);

  private:
    blaze::lexer &lexer;
    tok::token cur_token;
    tok::token peek_token;
    std::map<tok::type, prefix_hook_fn> prefix_hooks;
    std::map<tok::type, infix_hook_fn> infix_hooks;
    std::map<tok::type, precedence> precedences;

    void next_token();
    bool cur_token_is(tok::type);
    bool peek_token_is(tok::type);
    bool peek_token_expect(tok::type);
    precedence cur_token_preced();
    precedence peek_token_preced();
    std::unique_ptr<ast::event_> parse_event();
    std::unique_ptr<ast::roll> parse_roll();
    std::unique_ptr<ast::expr_> parse_expr(precedence);
    std::unique_ptr<ast::expr_> parse_expr_group();
    std::unique_ptr<ast::expr_> parse_expr_infix(std::unique_ptr<ast::expr_> &);
    std::unique_ptr<ast::expr_> parse_expr_prefix();
    std::unique_ptr<ast::expr_> parse_integer();
    std::unique_ptr<ast::expr_> parse_floating();
    std::unique_ptr<ast::expr_> parse_dice();

  public:
    explicit parser(blaze::lexer &);

    std::unique_ptr<ast::root> parse_root();
};

} // namespace blaze

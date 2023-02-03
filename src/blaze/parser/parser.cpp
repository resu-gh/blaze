#include "./parser.hpp"

namespace blaze {

parser::parser(blaze::lexer &l)
    : lexer(l) {
    next_token();
    next_token();
    precedences[tok::type::PLUS] = precedence::SUM;
    precedences[tok::type::MINUS] = precedence::SUM;
    precedences[tok::type::STAR] = precedence::PRODUCT;
    precedences[tok::type::SLASH] = precedence::PRODUCT;
    precedences[tok::type::CARET] = precedence::REPEAT;
    prefix_hooks[tok::type::INTEGER] = &parser::parse_integer;
    prefix_hooks[tok::type::FLOAT] = &parser::parse_floating;
    prefix_hooks[tok::type::DICE] = &parser::parse_dice;
    prefix_hooks[tok::type::LPAREN] = &parser::parse_expr_group;
    prefix_hooks[tok::type::PLUS] = &parser::parse_expr_prefix;
    prefix_hooks[tok::type::MINUS] = &parser::parse_expr_prefix;
    infix_hooks[tok::type::PLUS] = &parser::parse_expr_infix;
    infix_hooks[tok::type::MINUS] = &parser::parse_expr_infix;
    infix_hooks[tok::type::STAR] = &parser::parse_expr_infix;
    infix_hooks[tok::type::SLASH] = &parser::parse_expr_infix;
    infix_hooks[tok::type::CARET] = &parser::parse_expr_infix;
}

void parser::next_token() {
    cur_token = peek_token;
    peek_token = lexer.next_token();
}

bool parser::cur_token_is(tok::type t) {
    return cur_token.type == t;
}

bool parser::peek_token_is(tok::type t) {
    return peek_token.type == t;
}

bool parser::peek_token_expect(tok::type t) {
    if (peek_token_is(t)) {
        next_token();
        return true;
    }
    throw exception("expected %1%, found %2%", t, peek_token);
}

precedence parser::cur_token_preced() {
    if (precedences.find(cur_token.type) != precedences.end()) return precedences.at(cur_token.type);
    return precedence::LOWEST;
}

precedence parser::peek_token_preced() {
    if (precedences.find(peek_token.type) != precedences.end()) return precedences.at(peek_token.type);
    return precedence::LOWEST;
}

std::unique_ptr<ast::root> parser::parse_root() {
    auto root = std::make_unique<ast::root>();
    auto event = parse_event();
    if (event != nullptr) root->event = std::move(event);
    peek_token_expect(tok::type::EOF_);
    return root;
}

std::unique_ptr<ast::event_> parser::parse_event() {
    return parse_roll();
}

std::unique_ptr<ast::roll> parser::parse_roll() {
    auto roll = std::make_unique<ast::roll>();
    roll->token = cur_token;
    roll->expr = parse_expr(precedence::LOWEST);
    return roll;
}

std::unique_ptr<ast::expr_> parser::parse_expr(precedence p) {
    if (prefix_hooks.find(cur_token.type) == prefix_hooks.end()) {
        throw exception("no prefix parse fn for %1%", cur_token);
    }
    auto prefix = prefix_hooks[cur_token.type];
    std::unique_ptr<ast::expr_> leftx = std::invoke(prefix, this);
    while (!peek_token_is(tok::type::EOF_) && p < peek_token_preced()) {
        if (infix_hooks.find(peek_token.type) == infix_hooks.end()) return leftx;
        auto infix = infix_hooks[peek_token.type];
        next_token();
        leftx = std::invoke(infix, this, leftx);
    }
    return leftx;
}

std::unique_ptr<ast::expr_> parser::parse_integer() {
    auto intl = std::make_unique<ast::integer>();
    intl->token = cur_token;
    intl->value = stoll(cur_token.literal);
    return intl;
}

std::unique_ptr<ast::expr_> parser::parse_floating() {
    auto floatl = std::make_unique<ast::floating>();
    floatl->token = cur_token;
    floatl->value = stod(cur_token.literal);
    return floatl;
}

std::unique_ptr<ast::expr_> parser::parse_dice() {
    auto dicel = std::make_unique<ast::dice>();
    dicel->token = cur_token;
    auto d = cur_token.literal.find_first_of('d');
    const std::string nd = cur_token.literal.substr(0, d);
    if (nd == "0") throw exception("declared 0 dices %1%", cur_token);
    const std::string nf = cur_token.literal.substr(d + 1, cur_token.literal.size());
    if (nf == "0") throw exception("declared 0 faces dice %1%", cur_token);
    dicel->n_dices = nd.empty() ? 1 : stoull(nd);
    dicel->n_faces = stoull(nf);
    return dicel;
}

std::unique_ptr<ast::expr_> parser::parse_expr_infix(std::unique_ptr<ast::expr_> &l) {
    auto expr = std::make_unique<ast::infix>();
    expr->token = cur_token;
    expr->oper = cur_token.literal;
    expr->left = std::move(l);
    auto preced = cur_token_preced();
    next_token();
    expr->right = parse_expr(preced);
    return expr;
}

std::unique_ptr<ast::expr_> parser::parse_expr_group() {
    next_token();
    auto expr = parse_expr(precedence::LOWEST);
    if (!peek_token_expect(tok::type::RPAREN)) return nullptr;
    expr->expl = true;
    return expr;
}

std::unique_ptr<ast::expr_> parser::parse_expr_prefix() {
    auto expr = std::make_unique<ast::prefix>();
    expr->token = cur_token;
    expr->oper = cur_token.literal;
    next_token();
    expr->right = parse_expr(precedence::PREFIX);
    return expr;
}

} // namespace blaze

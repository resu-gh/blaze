#pragma once
#include "../lexer/lexer.hpp"
#include <memory>
#include <sstream>

namespace blaze {
namespace ast {

class node_ {
  public:
    node_() = default;
    node_(const node_ &) = default;
    node_(node_ &&) = delete;
    node_ &operator=(const node_ &) = default;
    node_ &operator=(node_ &&) = delete;
    virtual ~node_() = default;
    virtual std::string token_lit() const = 0;
    virtual std::string stringify() const = 0;
};

class event_ : public node_ {
  public:
    event_() = default;
    event_(const event_ &) = default;
    event_(event_ &&) = delete;
    event_ &operator=(const event_ &) = default;
    event_ &operator=(event_ &&) = delete;
    ~event_() override = default;
    virtual void _event() const = 0;
};

class expr_ : public node_ {
  public:
    expr_() = default;
    expr_(const expr_ &) = default;
    expr_(expr_ &&) = delete;
    expr_ &operator=(const expr_ &) = default;
    expr_ &operator=(expr_ &&) = delete;
    bool expl = false;
    ~expr_() override = default;
    virtual void _expr() const = 0;
};

class root final : public node_ {
  public:
    std::unique_ptr<event_> event;
    root();
    std::string token_lit() const override;
    std::string stringify() const override;
};

class roll final : public event_ {
  public:
    tok::token token;
    std::unique_ptr<expr_> expr;
    roll();
    void _event() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

class prefix final : public expr_ {
  public:
    tok::token token;
    std::string oper;
    std::unique_ptr<expr_> right;
    prefix();
    void _expr() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

class infix final : public expr_ {
  public:
    tok::token token;
    std::string oper;
    std::unique_ptr<expr_> left;
    std::unique_ptr<expr_> right;
    infix();
    void _expr() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

class integer final : public expr_ {
  public:
    tok::token token;
    int64_t value;
    integer();
    void _expr() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

class floating final : public expr_ {
  public:
    tok::token token;
    double value;
    floating();
    void _expr() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

class dice final : public expr_ {
  public:
    tok::token token;
    uint64_t n_dices;
    uint64_t n_faces;
    dice();
    void _expr() const override;
    std::string token_lit() const override;
    std::string stringify() const override;
};

} // namespace ast
} // namespace blaze

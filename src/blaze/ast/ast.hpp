#pragma once
#include "../lexer/lexer.hpp"
#include <memory>
#include <sstream>

namespace Blaze {
namespace AST {

class Node_ {
  public:
    Node_() = default;
    Node_(const Node_ &) = default;
    Node_(Node_ &&) = delete;
    Node_ &operator=(const Node_ &) = default;
    Node_ &operator=(Node_ &&) = delete;
    virtual ~Node_() = default;
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
};

class Event_ : public Node_ {
  public:
    Event_() = default;
    Event_(const Event_ &) = default;
    Event_(Event_ &&) = delete;
    Event_ &operator=(const Event_ &) = default;
    Event_ &operator=(Event_ &&) = delete;
    ~Event_() override = default;
    virtual void event_() const = 0;
};

class Expr_ : public Node_ {
  public:
    Expr_() = default;
    Expr_(const Expr_ &) = default;
    Expr_(Expr_ &&) = delete;
    Expr_ &operator=(const Expr_ &) = default;
    Expr_ &operator=(Expr_ &&) = delete;
    bool expl = false;
    ~Expr_() override = default;
    virtual void _expr() const = 0;
};

class Root final : public Node_ {
  public:
    std::unique_ptr<Event_> Event;
    Root();
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Roll final : public Event_ {
  public:
    Blaze::Token Token;
    std::unique_ptr<Expr_> Expr;
    Roll();
    void event_() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Prefix final : public Expr_ {
  public:
    Blaze::Token Token;
    std::string Operator;
    std::unique_ptr<Expr_> Right;
    Prefix();
    void _expr() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Infix final : public Expr_ {
  public:
    Blaze::Token Token;
    std::string Operator;
    std::unique_ptr<Expr_> Left;
    std::unique_ptr<Expr_> Right;
    Infix();
    void _expr() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Integer final : public Expr_ {
  public:
    Blaze::Token Token;
    int64_t Value;
    Integer();
    void _expr() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Float final : public Expr_ {
  public:
    Blaze::Token Token;
    double Value;
    Float();
    void _expr() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

class Dice final : public Expr_ {
  public:
    Blaze::Token Token;
    uint64_t nDices;
    uint64_t nFaces;
    Dice();
    void _expr() const override;
    std::string TokenLiteral() const override;
    std::string String() const override;
};

} // namespace AST
} // namespace Blaze

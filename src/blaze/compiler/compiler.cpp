#include "./compiler.hpp"

namespace Blaze {

Compiler::Compiler(const std::string &s) {
    Rand::CSPRNG r = Rand::CSPRNG();
    Lexer l = Lexer(s);
    Parser p = Parser(l);
    std::unique_ptr<AST::Root> a;
    try {
        a = p.ParseRoot();
    } catch (Exception &px) {
        throw &px;
    }
    std::cout << a->String() << std::endl << std::endl;
    Evaluator e = Evaluator(r, std::cout);
    std::unique_ptr<OBJ::Numeric_> o;
    try {
        o = e.Eval(*a);
    } catch (Exception &ax) {
        throw &ax;
    }
    std::cout << (o->Type() == OBJ::ObjectType::SEQUENCE ? "** " : " = ** ");
    std::cout << o->Inspect() << " **" << std::endl;
}

} // namespace Blaze

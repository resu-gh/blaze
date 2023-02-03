#include "./compiler.hpp"

namespace blaze {

compiler::compiler(const std::string &s) {
    rand::csprng r = rand::csprng();
    lexer l = lexer(s);
    parser p = parser(l);
    std::unique_ptr<ast::root> a;
    try {
        a = p.parse_root();
    } catch (blaze::exception &px) {
        throw px;
    }
    std::cout << a->stringify() << std::endl << std::endl;
    evaluator e = evaluator(r, std::cout);
    std::unique_ptr<obj::numeric_> o;
    try {
        o = e.eval(*a);
    } catch (blaze::exception &ax) {
        throw ax;
    }
    if (o->type() == obj::type::ARRAY) {
        std::cout << std::endl << "** ";
    } else {
        std::cout << " = ** ";
    }
    std::cout << o->inspect() << " **" << std::endl;
}

} // namespace blaze

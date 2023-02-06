#include "../blaze/compiler/compiler.hpp"

const std::string help = "USAGE: blaze {EXPRESSION}\n\n"
                         "EXPRESSION: \n\n"
                         " d#                      - declare 1 dice with # faces\n"
                         " #d#                     - declare # dices with # faces\n"
                         " +                       - operator (addition)\n"
                         " -                       - operator (subtract)\n"
                         " *                       - operator (multiplication)\n"
                         " /                       - operator (division)\n"
                         " ^                       - operator (repetition)\n"
                         " ()                      - parenthesis (precedence)\n\n"
                         "EXAMPLES: blaze ...\n\n"
                         " 1d6                     - roll a single dice d6\n"
                         " 2d20                    - roll 2 times 2 d20 dices\n";

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << help;
        return 0;
    }
    try {
        blaze::compiler{argv[1]};
    } catch (blaze::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    };
    return 0;
}

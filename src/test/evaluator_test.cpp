#define GLOG_CUSTOM_PREFIX_SUPPORT
#include "../blaze/evaluator/evaluator.hpp"
#include "../blaze/parser/parser.hpp"
#include <gtest/gtest.h>
#include <glog/logging.h>

void glogPrefix(std::ostream &s, const google::LogMessageInfo &l, void *) {
    s << l.severity[0] << " ";
    s << l.time.hour() << ":";
    s << l.time.min() << ":";
    s << l.time.sec() << " ";
    s << l.thread_id << " ";
    s << l.filename << ":";
    s << l.line_number << "]";
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    fLB::FLAGS_logtostderr = 1;
    fLB::FLAGS_colorlogtostderr = 1;
    ::google::InitGoogleLogging(argv[0], &glogPrefix);
    LOG(INFO) << "Running tests\n";
    auto r = RUN_ALL_TESTS();
    r ? LOG(ERROR) << "Failed" << std::endl : LOG(INFO) << "Successstd::" << std::endl;
    return 0;
}

TEST(EvaluatorT, expr) {
    struct Test {
        std::string input;
        Blaze::OBJ::ObjectType expectedType;
        std::string expectedValue;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"(10 * 2) + 10 / 2 - 5", Blaze::OBJ::ObjectType::INTEGER, "20"},
        {"33 + 67", Blaze::OBJ::ObjectType::INTEGER, "100"},
        {"1024 - 256", Blaze::OBJ::ObjectType::INTEGER, "768"},
        {"12 * +12", Blaze::OBJ::ObjectType::INTEGER, "144"},
        {"144 / 12", Blaze::OBJ::ObjectType::INTEGER, "12"},
        {"-1 + 2", Blaze::OBJ::ObjectType::INTEGER, "1"},
        {"2 + -1.2", Blaze::OBJ::ObjectType::FLOAT, "0.8"},
        {"(2 + -1.2) * 2", Blaze::OBJ::ObjectType::FLOAT, "1.6"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto a = parser.ParseRoot();
        const int64_t seed = 1;
        Blaze::Rand::PRNG g(seed);
        std::stringstream s;
        Blaze::Evaluator e(g, s);
        auto o = e.Eval(*a);
        // test: OBJ::Object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        EXPECT_EQ(o->Type(), test.expectedType);
        EXPECT_EQ(o->Inspect(), test.expectedValue);
    }
}

TEST(EvaluatorT, exprWithDice) {
    struct Test {
        std::string input;
        Blaze::OBJ::ObjectType expectedType;
        std::string expectedValue;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"d6", Blaze::OBJ::ObjectType::DICE, "1"},
        {"+10d6", Blaze::OBJ::ObjectType::INTEGER, "27"},
        {"-d6", Blaze::OBJ::ObjectType::INTEGER, "-1"},
        {"2d6 + 2", Blaze::OBJ::ObjectType::INTEGER, "4"},
        {"2d6 + 2.0", Blaze::OBJ::ObjectType::INTEGER, "4"},
        {"2 + 20d20", Blaze::OBJ::ObjectType::INTEGER, "171"},
        {"2.0 + 20d20", Blaze::OBJ::ObjectType::INTEGER, "171"},
        {"10d6 * 20d20", Blaze::OBJ::ObjectType::INTEGER, "4536"},
        {"1d6 * (2 + d1)", Blaze::OBJ::ObjectType::INTEGER, "3"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto a = parser.ParseRoot();
        const int64_t seed = 1;
        Blaze::Rand::PRNG g(seed);
        std::stringstream s;
        Blaze::Evaluator e(g, s);
        auto o = e.Eval(*a);
        // test: OBJ::Object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        EXPECT_EQ(o->Type(), test.expectedType);
        EXPECT_EQ(o->Inspect(), test.expectedValue);
    }
}

TEST(EvaluatorT, print) {
    struct Test {
        std::string input;
        std::string expectedString;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"1", "1 = 1"},
        {"(1)", "(1) = 1"},
        {"(1.0)", "(1) = 1"},
        {"(1.5)", "(1.5) = 1.5"},
        {"+1", "+1 = 1"},
        {"(+1)", "(+1) = 1"},
        {"-1", "-1 = -1"},
        {"(-1)", "(-1) = -1"},
        {"d6", "[1](1) = 1"},
        {"+d6", "+[1](1) = 1"},
        {"-d6", "-[1](1) = -1"},
        {"(-d6)", "(-[1](1)) = -1"},
        {"(+d6)", "(+[1](1)) = 1"},
        {"2d6", "[1,1](2) = 2"},
        {"2d6 + 3", "[1,1](2) + 3 = 5"},
        {"(2d6 + 3)", "([1,1](2) + 3) = 5"},
        {"1 + 2", "1 + 2 = 3"},
        {"1 + 2 * 3", "1 + 2 * 3 = 7"},
        {"(1 + 2) * 3", "(1 + 2) * 3 = 9"},
        {"(1 + 2) * -3", "(1 + 2) * -3 = -9"},
        {"(1 + (2)) * (-3)", "(1 + (2)) * (-3) = -9"},
        {"(-2d6) + +5d20", "(-[1,1](2)) + +[10,1,8,19,10](48) = 46"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto a = parser.ParseRoot();
        const int64_t seed = 1;
        Blaze::Rand::PRNG g(seed);
        std::stringstream s;
        Blaze::Evaluator e(g, s);
        auto o = e.Eval(*a);
        // test: OBJ::Object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        const std::string r = s.str() + " = " + o->Inspect();
        EXPECT_EQ(r, test.expectedString);
    }
}

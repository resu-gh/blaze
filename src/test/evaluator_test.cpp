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
    LOG(INFO) << "running tests\n";
    auto r = RUN_ALL_TESTS();
    r ? LOG(ERROR) << "failed" << std::endl : LOG(INFO) << "success" << std::endl;
    return 0;
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(EvaluatorTest, expr) {
    struct test_case {
        std::string input;
        blaze::obj::type expected_type;
        std::string expected_value;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"(10 * 2) + 10 / 2 - 5", blaze::obj::type::INTEGER, "20"},
        {"33 + 67", blaze::obj::type::INTEGER, "100"},
        {"1024 - 256", blaze::obj::type::INTEGER, "768"},
        {"12 * +12", blaze::obj::type::INTEGER, "144"},
        {"144 / 12", blaze::obj::type::INTEGER, "12"},
        {"-1 + 2", blaze::obj::type::INTEGER, "1"},
        {"2 + -1.2", blaze::obj::type::FLOAT, "0.8"},
        {"(2 + -1.2) * 2", blaze::obj::type::FLOAT, "1.6"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto a = parser.parse_root();
        const int64_t seed = 1;
        blaze::rand::prng g(seed);
        std::stringstream s;
        blaze::evaluator e(g, s);
        auto o = e.eval(*a);
        // test: obj::object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        EXPECT_EQ(o->type(), test.expected_type);
        EXPECT_EQ(o->inspect(), test.expected_value);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(EvaluatorTest, exprWithDices) {
    struct test_case {
        std::string input;
        blaze::obj::type expected_type;
        std::string expected_value;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"d6", blaze::obj::type::DICE, "1"},
        {"+10d6", blaze::obj::type::INTEGER, "27"},
        {"-d6", blaze::obj::type::INTEGER, "-1"},
        {"2d6 + 2", blaze::obj::type::INTEGER, "4"},
        {"2d6 + 2.0", blaze::obj::type::INTEGER, "4"},
        {"2 + 20d20", blaze::obj::type::INTEGER, "171"},
        {"2.0 + 20d20", blaze::obj::type::INTEGER, "171"},
        {"10d6 * 20d20", blaze::obj::type::INTEGER, "4536"},
        {"1d6 * (2 + d1)", blaze::obj::type::INTEGER, "3"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto a = parser.parse_root();
        const int64_t seed = 1;
        blaze::rand::prng g(seed);
        std::stringstream s;
        blaze::evaluator e(g, s);
        auto o = e.eval(*a);
        // test: obj::object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        EXPECT_EQ(o->type(), test.expected_type);
        EXPECT_EQ(o->inspect(), test.expected_value);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(EvaluatorTest, print) {
    struct test_case {
        std::string input;
        std::string expected_string;
    };
    const std::vector<test_case> tests = {
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
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto a = parser.parse_root();
        const int64_t seed = 1;
        blaze::rand::prng g(seed);
        std::stringstream s;
        blaze::evaluator e(g, s);
        auto o = e.eval(*a);
        // test: obj::object must be != nullptr
        ASSERT_NE(o, nullptr);
        // expectations
        const std::string r = s.str() + " = " + o->inspect();
        EXPECT_EQ(r, test.expected_string);
    }
}

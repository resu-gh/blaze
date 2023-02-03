#define GLOG_CUSTOM_PREFIX_SUPPORT
#include "../blaze/parser/parser.hpp"
#include <gtest/gtest.h>
#include <climits>
#include <glog/logging.h>

void glogPrefix(std::ostream &s, const google::LogMessageInfo &l, void *) {
    s << l.severity[0] << " "; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
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
    ::google::InitGoogleLogging(argv[0], &glogPrefix); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    LOG(INFO) << "running tests\n";
    auto r = RUN_ALL_TESTS();
    r ? LOG(ERROR) << "failed" << std::endl : LOG(INFO) << "success" << std::endl;
    return 0;
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, integer) {
    struct test_case {
        std::string input;
        int64_t expected_value;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"01", 1},
        {"0012", 12},
        {"10", 10},
        {"21", 21},
        {"2308480000", 2308480000},
        {std::to_string(INT_MAX), INT_MAX},
        {" 2308      ", 2308},
        {"\n 239\t      ", 239},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an integer
        auto *intl = dynamic_cast<blaze::ast::integer *>(expr);
        ASSERT_NE(intl, nullptr);
        // expectations
        EXPECT_EQ(intl->token.type, blaze::tok::type::INTEGER);
        EXPECT_EQ(intl->value, test.expected_value);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, float) {
    struct test_case {
        std::string input;
        double expected_value;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"01.0", 1.0},
        {"0012.4", 12.4},
        {"10.2", 10.2},
        {"2398.10804", 2398.10804},
        {"88.2323084720348", 88.2323084720348},
        {std::to_string(DBL_MAX), DBL_MAX},
        {" 23.405      ", 23.405},
        {"\n 103.453\t      ", 103.453},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an float
        auto *fltl = dynamic_cast<blaze::ast::floating *>(expr);
        ASSERT_NE(fltl, nullptr);
        // expectations
        EXPECT_EQ(fltl->token.type, blaze::tok::type::FLOAT);
        EXPECT_EQ(fltl->value, test.expected_value);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, dice) {
    struct test_case {
        std::string input;
        uint64_t expected_n_dices;
        uint64_t expected_n_faces;
        std::string expected_token_lit;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"10d2", 10, 2, "10d2"},
        {"2398d10804", 2398, 10804, "2398d10804"},
        {"88d2323084720348", 88, 2323084720348, "88d2323084720348"},
        {"d12309", 1, 12309, "d12309"},
        {" 1d20      ", 1, 20, "1d20"},
        {"\n 103d453\t      ", 103, 453, "103d453"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an dice
        auto *dicel = dynamic_cast<blaze::ast::dice *>(expr);
        ASSERT_NE(dicel, nullptr);
        // expectations
        EXPECT_EQ(dicel->token.type, blaze::tok::type::DICE);
        EXPECT_EQ(dicel->n_dices, test.expected_n_dices);
        EXPECT_EQ(dicel->n_faces, test.expected_n_faces);
        EXPECT_EQ(dicel->token.literal, test.expected_token_lit);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, prefixInteger) {
    struct test_case {
        std::string input;
        blaze::tok::type expected_oper_type;
        std::string expected_oper;
        int64_t expected_value;
        std::string expected_token_lit;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"-3", blaze::tok::type::MINUS, "-", 3, "-3"},
        {"-2398", blaze::tok::type::MINUS, "-", 2398, "-2398"},
        {"+18000", blaze::tok::type::PLUS, "+", 18000, "+18000"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an prefix
        auto *prefix = dynamic_cast<blaze::ast::prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced prefix.right must be an integer
        auto *right = dynamic_cast<blaze::ast::integer *>(prefix->right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->token.type, test.expected_oper_type);
        EXPECT_EQ(prefix->oper, test.expected_oper);
        EXPECT_EQ(right->value, test.expected_value);
        EXPECT_EQ(prefix->token.literal + right->token.literal, test.expected_token_lit);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, prefixFloat) {
    struct test_case {
        std::string input;
        blaze::tok::type expected_open_type;
        std::string expected_oper;
        double expected_value;
        std::string expected_token_lit;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"-3.3", blaze::tok::type::MINUS, "-", 3.3, "-3.3"},
        {"-23.98", blaze::tok::type::MINUS, "-", 23.98, "-23.98"},
        {"+230.98", blaze::tok::type::PLUS, "+", 230.98, "+230.98"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an prefix
        auto *prefix = dynamic_cast<blaze::ast::prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced prefix.right must be an float
        auto *right = dynamic_cast<blaze::ast::floating *>(prefix->right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->token.type, test.expected_open_type);
        EXPECT_EQ(prefix->oper, test.expected_oper);
        EXPECT_EQ(right->value, test.expected_value);
        EXPECT_EQ(prefix->token.literal + right->token.literal, test.expected_token_lit);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(ParserTest, prefixDice) {
    struct test_case {
        std::string input;
        blaze::tok::type expected_oper_type;
        std::string expected_oper;
        uint64_t expected_n_dices;
        uint64_t expected_n_faces;
        std::string expected_token_lit;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {"-1d6", blaze::tok::type::MINUS, "-", 1, 6, "-1d6"},
        {"-d6", blaze::tok::type::MINUS, "-", 1, 6, "-d6"},
        {"+100d6", blaze::tok::type::PLUS, "+", 100, 6, "+100d6"},
        // clang-format on
    };
    for (auto &test : tests) {
        blaze::lexer lexer(test.input);
        blaze::parser parser(lexer);
        auto root = parser.parse_root();
        // test: (cast) the produced event_ must be an roll
        auto *roll = dynamic_cast<blaze::ast::roll *>(root->event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced roll.expr must be an expr_
        auto *expr = dynamic_cast<blaze::ast::expr_ *>(roll->expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced expr_ must be an prefix
        auto *prefix = dynamic_cast<blaze::ast::prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced prefix.right must be an float
        auto *right = dynamic_cast<blaze::ast::dice *>(prefix->right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->token.type, test.expected_oper_type);
        EXPECT_EQ(prefix->oper, test.expected_oper);
        EXPECT_EQ(right->n_dices, test.expected_n_dices);
        EXPECT_EQ(right->n_faces, test.expected_n_faces);
        EXPECT_EQ(prefix->token.literal + right->token.literal, test.expected_token_lit);
    }
}

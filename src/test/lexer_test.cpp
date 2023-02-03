#define GLOG_CUSTOM_PREFIX_SUPPORT
#include "../blaze/lexer/lexer.hpp"
#include <gtest/gtest.h>
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
TEST(LexerTest, diagnostics) {
    const std::string input = "s";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::ILLEGAL_, "s"},
        {blaze::tok::type::EOF_, ""},
        // clang-format on
    };
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(LexerTest, token) {
    const std::string input = "s + - * / 12 1.2 d10 10d10 ( ) ^";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::ILLEGAL_, "s"},
        {blaze::tok::type::PLUS, "+"},
        {blaze::tok::type::MINUS, "-"},
        {blaze::tok::type::STAR, "*"},
        {blaze::tok::type::SLASH, "/"},
        {blaze::tok::type::INTEGER, "12"},
        {blaze::tok::type::FLOAT, "1.2"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::DICE, "10d10"},
        {blaze::tok::type::LPAREN, "("},
        {blaze::tok::type::RPAREN, ")"},
        {blaze::tok::type::CARET, "^"},
        {blaze::tok::type::EOF_, ""},
        // clang-format on
    };
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(LexerTest, dice) {
    const std::string input = "d10 d100 d1d d308s dd10 73dd10 "
                              "10d10 1.0d10 10d.200 10d1.4 "
                              "408d1s 393d10d 1d1rand";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::DICE, "d100"},
        {blaze::tok::type::DICE, "d1"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::DICE, "d308"},
        {blaze::tok::type::ILLEGAL_, "s"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::ILLEGAL_, "73d"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::DICE, "10d10"},
        {blaze::tok::type::FLOAT, "1.0"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::ILLEGAL_, "10d"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "200"},
        {blaze::tok::type::DICE, "10d1"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "4"},
        {blaze::tok::type::DICE, "408d1"},
        {blaze::tok::type::ILLEGAL_, "s"},
        {blaze::tok::type::DICE, "393d10"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::DICE, "1d1"},
        {blaze::tok::type::ILLEGAL_, "r"},
        {blaze::tok::type::ILLEGAL_, "a"},
        {blaze::tok::type::ILLEGAL_, "n"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::EOF_, ""},
    };
    // clang-format on
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(LexerTest, interger) {
    const std::string input = "1 10 100 100s 100d 3948d8293 -2930 -29.30";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::INTEGER, "1"},
        {blaze::tok::type::INTEGER, "10"},
        {blaze::tok::type::INTEGER, "100"},
        {blaze::tok::type::INTEGER, "100"},
        {blaze::tok::type::ILLEGAL_, "s"},
        {blaze::tok::type::ILLEGAL_, "100d"},
        {blaze::tok::type::DICE, "3948d8293"},
        {blaze::tok::type::MINUS, "-"},
        {blaze::tok::type::INTEGER, "2930"},
        {blaze::tok::type::MINUS, "-"},
        {blaze::tok::type::FLOAT, "29.30"},
        {blaze::tok::type::EOF_, ""},
        // clang-format on
    };
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(LexerTest, float) {
    const std::string input = "1. .1 23.4 234 . 28.29389 2.3d "
                              "408.2398d d2398.13 2398d.239 23d2.3";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::ILLEGAL_, "1."},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "1"},
        {blaze::tok::type::FLOAT, "23.4"},
        {blaze::tok::type::INTEGER, "234"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::FLOAT, "28.29389"},
        {blaze::tok::type::FLOAT, "2.3"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::FLOAT, "408.2398"},
        {blaze::tok::type::ILLEGAL_, "d"},
        {blaze::tok::type::DICE, "d2398"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "13"},
        {blaze::tok::type::ILLEGAL_, "2398d"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "239"},
        {blaze::tok::type::DICE, "23d2"},
        {blaze::tok::type::ILLEGAL_, "."},
        {blaze::tok::type::INTEGER, "3"},
        {blaze::tok::type::EOF_, ""},
        // clang-format on
    };
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,cppcoreguidelines-owning-memory)
TEST(LexerTest, scan) {
    const std::string input = "+ + d10 10d6 10d10d10\t\n"
                              "d20 + 2d12 - 12\r "
                              "3d20\t * 1.234     / (20\r )\n";
    struct test_case {
        blaze::tok::type expected_type;
        std::string expected_literal;
    };
    const std::vector<test_case> tests = {
        // clang-format off
        {blaze::tok::type::PLUS, "+"},
        {blaze::tok::type::PLUS, "+"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::DICE, "10d6"},
        {blaze::tok::type::DICE, "10d10"},
        {blaze::tok::type::DICE, "d10"},
        {blaze::tok::type::DICE, "d20"},
        {blaze::tok::type::PLUS, "+"},
        {blaze::tok::type::DICE, "2d12"},
        {blaze::tok::type::MINUS, "-"},
        {blaze::tok::type::INTEGER, "12"},
        {blaze::tok::type::DICE, "3d20"},
        {blaze::tok::type::STAR, "*"},
        {blaze::tok::type::FLOAT, "1.234"},
        {blaze::tok::type::SLASH, "/"},
        {blaze::tok::type::LPAREN, "("},
        {blaze::tok::type::INTEGER, "20"},
        {blaze::tok::type::RPAREN, ")"},
        {blaze::tok::type::EOF_, ""},
        // clang-format on
    };
    blaze::lexer lexer(input);
    for (auto &test : tests) {
        const blaze::tok::token tok = lexer.next_token();
        // expectations
        EXPECT_EQ(tok.type, test.expected_type);
        EXPECT_EQ(tok.literal, test.expected_literal);
    }
}

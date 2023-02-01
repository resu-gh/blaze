#define GLOG_CUSTOM_PREFIX_SUPPORT
#include "../blaze/lexer/lexer.hpp"
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
    r ? LOG(ERROR) << "Failed" << std::endl : LOG(INFO) << "Success" << std::endl;
    return 0;
}

TEST(LexerT, diagnostics) {
    const std::string input = "s";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::ILLEGAL_, "s"},
        {Blaze::TokenType::EOF_, ""},
        // clang-format on
    };
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

TEST(LexerT, token) {
    const std::string input = "s + - * / 12 1.2 d10 10d10 ( ) ^";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::ILLEGAL_, "s"},
        {Blaze::TokenType::PLUS, "+"},
        {Blaze::TokenType::MINUS, "-"},
        {Blaze::TokenType::STAR, "*"},
        {Blaze::TokenType::SLASH, "/"},
        {Blaze::TokenType::INTEGER, "12"},
        {Blaze::TokenType::FLOAT, "1.2"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::DICE, "10d10"},
        {Blaze::TokenType::LPAREN, "("},
        {Blaze::TokenType::RPAREN, ")"},
        {Blaze::TokenType::CARET, "^"},
        {Blaze::TokenType::EOF_, ""},
        // clang-format on
    };
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

TEST(LexerT, dice) {
    const std::string input = "d10 d100 d1d d308s dd10 73dd10 "
                              "10d10 1.0d10 10d.200 10d1.4 "
                              "408d1s 393d10d 1d1rand";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::DICE, "d100"},
        {Blaze::TokenType::DICE, "d1"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::DICE, "d308"},
        {Blaze::TokenType::ILLEGAL_, "s"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::ILLEGAL_, "73d"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::DICE, "10d10"},
        {Blaze::TokenType::FLOAT, "1.0"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::ILLEGAL_, "10d"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "200"},
        {Blaze::TokenType::DICE, "10d1"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "4"},
        {Blaze::TokenType::DICE, "408d1"},
        {Blaze::TokenType::ILLEGAL_, "s"},
        {Blaze::TokenType::DICE, "393d10"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::DICE, "1d1"},
        {Blaze::TokenType::ILLEGAL_, "r"},
        {Blaze::TokenType::ILLEGAL_, "a"},
        {Blaze::TokenType::ILLEGAL_, "n"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::EOF_, ""},
    };
    // clang-format on
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

TEST(LexerT, interger) {
    const std::string input = "1 10 100 100s 100d 3948d8293 -2930 -29.30";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::INTEGER, "1"},
        {Blaze::TokenType::INTEGER, "10"},
        {Blaze::TokenType::INTEGER, "100"},
        {Blaze::TokenType::INTEGER, "100"},
        {Blaze::TokenType::ILLEGAL_, "s"},
        {Blaze::TokenType::ILLEGAL_, "100d"},
        {Blaze::TokenType::DICE, "3948d8293"},
        {Blaze::TokenType::MINUS, "-"},
        {Blaze::TokenType::INTEGER, "2930"},
        {Blaze::TokenType::MINUS, "-"},
        {Blaze::TokenType::FLOAT, "29.30"},
        {Blaze::TokenType::EOF_, ""},
        // clang-format on
    };
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

TEST(LexerT, float) {
    const std::string input = "1. .1 23.4 234 . 28.29389 2.3d "
                              "408.2398d d2398.13 2398d.239 23d2.3";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::ILLEGAL_, "1."},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "1"},
        {Blaze::TokenType::FLOAT, "23.4"},
        {Blaze::TokenType::INTEGER, "234"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::FLOAT, "28.29389"},
        {Blaze::TokenType::FLOAT, "2.3"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::FLOAT, "408.2398"},
        {Blaze::TokenType::ILLEGAL_, "d"},
        {Blaze::TokenType::DICE, "d2398"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "13"},
        {Blaze::TokenType::ILLEGAL_, "2398d"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "239"},
        {Blaze::TokenType::DICE, "23d2"},
        {Blaze::TokenType::ILLEGAL_, "."},
        {Blaze::TokenType::INTEGER, "3"},
        {Blaze::TokenType::EOF_, ""},
        // clang-format on
    };
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

TEST(LexerT, scan) {
    const std::string input = "+ + d10 10d6 10d10d10\t\n"
                              "d20 + 2d12 - 12\r "
                              "3d20\t * 1.234     / (20\r )\n";
    struct Test {
        Blaze::TokenType expectedType;
        std::string expectedLiteral;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {Blaze::TokenType::PLUS, "+"},
        {Blaze::TokenType::PLUS, "+"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::DICE, "10d6"},
        {Blaze::TokenType::DICE, "10d10"},
        {Blaze::TokenType::DICE, "d10"},
        {Blaze::TokenType::DICE, "d20"},
        {Blaze::TokenType::PLUS, "+"},
        {Blaze::TokenType::DICE, "2d12"},
        {Blaze::TokenType::MINUS, "-"},
        {Blaze::TokenType::INTEGER, "12"},
        {Blaze::TokenType::DICE, "3d20"},
        {Blaze::TokenType::STAR, "*"},
        {Blaze::TokenType::FLOAT, "1.234"},
        {Blaze::TokenType::SLASH, "/"},
        {Blaze::TokenType::LPAREN, "("},
        {Blaze::TokenType::INTEGER, "20"},
        {Blaze::TokenType::RPAREN, ")"},
        {Blaze::TokenType::EOF_, ""},
        // clang-format on
    };
    Blaze::Lexer lexer(input);
    for (auto &test : tests) {
        const Blaze::Token tok = lexer.NextToken();
        // expectations
        EXPECT_EQ(tok.Type, test.expectedType);
        EXPECT_EQ(tok.Literal, test.expectedLiteral);
    }
}

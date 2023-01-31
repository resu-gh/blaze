#define GLOG_CUSTOM_PREFIX_SUPPORT
#include "../blaze/parser/parser.hpp"
#include <gtest/gtest.h>
#include <climits>
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

TEST(ParserT, Integer) {
    struct Test {
        std::string input;
        int64_t expectedValue;
    };
    const std::vector<Test> tests = {
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
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Integer
        auto *intl = dynamic_cast<Blaze::AST::Integer *>(expr);
        ASSERT_NE(intl, nullptr);
        // expectations
        EXPECT_EQ(intl->Token.Type, Blaze::TokenType::INTEGER);
        EXPECT_EQ(intl->Value, test.expectedValue);
    }
}

TEST(ParserT, Float) {
    struct Test {
        std::string input;
        double expectedValue;
    };
    const std::vector<Test> tests = {
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
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Float
        auto *fltl = dynamic_cast<Blaze::AST::Float *>(expr);
        ASSERT_NE(fltl, nullptr);
        // expectations
        EXPECT_EQ(fltl->Token.Type, Blaze::TokenType::FLOAT);
        EXPECT_EQ(fltl->Value, test.expectedValue);
    }
}

TEST(ParserT, Dice) {
    struct Test {
        std::string input;
        int64_t expectednDices;
        int64_t expectednFaces;
        std::string expectedTokenLit;
    };
    const std::vector<Test> tests = {
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
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Dice
        auto *dicel = dynamic_cast<Blaze::AST::Dice *>(expr);
        ASSERT_NE(dicel, nullptr);
        // expectations
        EXPECT_EQ(dicel->Token.Type, Blaze::TokenType::DICE);
        EXPECT_EQ(dicel->nDices, test.expectednDices);
        EXPECT_EQ(dicel->nFaces, test.expectednFaces);
        EXPECT_EQ(dicel->Token.Literal, test.expectedTokenLit);
    }
}

TEST(ParserT, prefixInteger) {
    struct Test {
        std::string input;
        Blaze::TokenType expectedOperatorType;
        std::string expectedOperator;
        int64_t expectedValue;
        std::string expectedTokenLit;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"-3", Blaze::TokenType::MINUS, "-", 3, "-3"},
        {"-2398", Blaze::TokenType::MINUS, "-", 2398, "-2398"},
        {"+18000", Blaze::TokenType::PLUS, "+", 18000, "+18000"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Prefix
        auto *prefix = dynamic_cast<Blaze::AST::Prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced Prefix.Right must be an Integer
        auto *right = dynamic_cast<Blaze::AST::Integer *>(prefix->Right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->Token.Type, test.expectedOperatorType);
        EXPECT_EQ(prefix->Operator, test.expectedOperator);
        EXPECT_EQ(right->Value, test.expectedValue);
        EXPECT_EQ(prefix->Token.Literal + right->Token.Literal, test.expectedTokenLit);
    }
}

TEST(ParserT, prefixFloat) {
    struct Test {
        std::string input;
        Blaze::TokenType expectedOperatorType;
        std::string expectedOperator;
        double expectedValue;
        std::string expectedTokenLit;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"-3.3", Blaze::TokenType::MINUS, "-", 3.3, "-3.3"},
        {"-23.98", Blaze::TokenType::MINUS, "-", 23.98, "-23.98"},
        {"+230.98", Blaze::TokenType::PLUS, "+", 230.98, "+230.98"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Prefix
        auto *prefix = dynamic_cast<Blaze::AST::Prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced Prefix.Right must be an Float
        auto *right = dynamic_cast<Blaze::AST::Float *>(prefix->Right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->Token.Type, test.expectedOperatorType);
        EXPECT_EQ(prefix->Operator, test.expectedOperator);
        EXPECT_EQ(right->Value, test.expectedValue);
        EXPECT_EQ(prefix->Token.Literal + right->Token.Literal, test.expectedTokenLit);
    }
}

TEST(ParserT, prefixDice) {
    struct Test {
        std::string input;
        Blaze::TokenType expectedOperatorType;
        std::string expectedOperator;
        uint64_t expectednDices;
        uint64_t expectednFaces;
        std::string expectedTokenLit;
    };
    const std::vector<Test> tests = {
        // clang-format off
        {"-1d6", Blaze::TokenType::MINUS, "-", 1, 6, "-1d6"},
        {"-d6", Blaze::TokenType::MINUS, "-", 1, 6, "-d6"},
        {"+100d6", Blaze::TokenType::PLUS, "+", 100, 6, "+100d6"},
        // clang-format on
    };
    for (auto &test : tests) {
        Blaze::Lexer lexer(test.input);
        Blaze::Parser parser(lexer);
        auto root = parser.ParseRoot();
        // test: (cast) the produced Event_ must be an Roll
        auto *roll = dynamic_cast<Blaze::AST::Roll *>(root->Event.get());
        ASSERT_NE(roll, nullptr);
        // test: (cast) the produced Roll.Expr must be an Expr_
        auto *expr = dynamic_cast<Blaze::AST::Expr_ *>(roll->Expr.get());
        ASSERT_NE(expr, nullptr);
        // test: (cast) the produced Expr_ must be an Prefix
        auto *prefix = dynamic_cast<Blaze::AST::Prefix *>(expr);
        ASSERT_NE(prefix, nullptr);
        // test: (cast) the produced Prefix.Right must be an Float
        auto *right = dynamic_cast<Blaze::AST::Dice *>(prefix->Right.get());
        ASSERT_NE(right, nullptr);
        // expectations
        EXPECT_EQ(prefix->Token.Type, test.expectedOperatorType);
        EXPECT_EQ(prefix->Operator, test.expectedOperator);
        EXPECT_EQ(right->nDices, test.expectednDices);
        EXPECT_EQ(right->nFaces, test.expectednFaces);
        EXPECT_EQ(prefix->Token.Literal + right->Token.Literal, test.expectedTokenLit);
    }
}

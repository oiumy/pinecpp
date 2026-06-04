#include "../src/lexer/lexer.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>

using namespace pinecpp;

void test_basic_assignment() {
    Lexer lexer;
    auto tokens = lexer.tokenize("sma = ta.sma(close, 14)");

    assert(tokens.size() >= 10);
    assert(tokens[0].type == TokenType::IDENTIFIER);
    assert(tokens[0].lexeme == "sma");
    assert(tokens[1].type == TokenType::EQUAL);
    assert(tokens[2].type == TokenType::IDENTIFIER);
    assert(tokens[2].lexeme == "ta");
    assert(tokens[3].type == TokenType::DOT);
    assert(tokens[4].type == TokenType::IDENTIFIER);
    assert(tokens[4].lexeme == "sma");
    assert(tokens[5].type == TokenType::LPAREN);
    assert(tokens[6].type == TokenType::IDENTIFIER);
    assert(tokens[6].lexeme == "close");
    assert(tokens[7].type == TokenType::COMMA);
    assert(tokens[8].type == TokenType::NUMBER);
    assert(tokens[8].lexeme == "14");
    assert(tokens[9].type == TokenType::RPAREN);
    std::cout << "[PASS] test_basic_assignment\n";
}

void test_decimal_number() {
    Lexer lexer;
    auto tokens = lexer.tokenize("3.14");
    assert(tokens.size() == 2);
    assert(tokens[0].type == TokenType::NUMBER);
    assert(tokens[0].lexeme == "3.14");
    std::cout << "[PASS] test_decimal_number\n";
}

void test_comment_ignore() {
    Lexer lexer;
    auto tokens = lexer.tokenize("sma # this is a comment");
    assert(tokens.size() == 2);
    assert(tokens[0].type == TokenType::IDENTIFIER);
    assert(tokens[0].lexeme == "sma");
    std::cout << "[PASS] test_comment_ignore\n";
}

void test_keyword_if() {
    Lexer lexer;
    auto tokens = lexer.tokenize("if a > b");
    assert(tokens.size() == 5);
    assert(tokens[0].type == TokenType::IF);
    assert(tokens[1].type == TokenType::IDENTIFIER && tokens[1].lexeme == "a");
    assert(tokens[2].type == TokenType::GREATER);
    assert(tokens[3].type == TokenType::IDENTIFIER && tokens[3].lexeme == "b");
    std::cout << "[PASS] test_keyword_if\n";
}

void test_double_char_op() {
    Lexer lexer;
    auto tokens = lexer.tokenize("a >= b");
    assert(tokens.size() == 4);
    assert(tokens[0].type == TokenType::IDENTIFIER && tokens[0].lexeme == "a");
    assert(tokens[1].type == TokenType::GREATER_EQ);
    assert(tokens[2].type == TokenType::IDENTIFIER && tokens[2].lexeme == "b");
    std::cout << "[PASS] test_double_char_op\n";
}

void test_illegal_char_error() {
    Lexer lexer;
    bool caught = false;
    try {
        auto tokens = lexer.tokenize("a $ b");
    } catch (const std::runtime_error& e) {
        caught = true;
        std::cout << "[PASS] test_illegal_char_error\n";
    }
    assert(caught);
}

int main() {
    test_basic_assignment();
    test_decimal_number();
    test_comment_ignore();
    test_keyword_if();
    test_double_char_op();
    test_illegal_char_error();
    return 0;
}
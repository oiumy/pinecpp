#include "lexer.hpp"
#include <stdexcept>
#include <string>

namespace pinecpp {
//====字符操作====
char Lexer::peek() const {
    if (is_at_end()) return '\0';
    return (*source_)[pos_];
}

char Lexer::advance() {
    col_++;
    return (*source_)[pos_++];
}

bool Lexer::is_at_end() const {
    return pos_ >= source_->size();
}

//====空白和注释====
void Lexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();  // 跳过空格、tab、换行符
        } else if (c == '\n') {
            advance();
            line_++;
            col_ = 1;  // 换行：行号+1，列号重置为1
        } else {
            break;
        }
    }
}

void Lexer::skip_comment() {
    while (!is_at_end() && peek() != '\n') {
        advance();
    }
}

//====Token识别====
Token Lexer::read_number() {
    int start_col = col_;
    std::string num;

    while (!is_at_end()) {
        char c = peek();
        if (c >= '0' && c <= '9') {
            num += advance();
        } else if (c == '.' && num.find('.') == std::string::npos) {
            num += advance(); //只允许1个小数点
        } else {
            break;
        }
    }

    return Token(TokenType::NUMBER, num, line_, start_col);
}

Token Lexer::read_identifier() {
    int start_col = col_;
    std::string id;

    while (!is_at_end()) {
        char c = peek();
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
            id += advance();
        } else {
            break;
        }
    }

    if (id == "if") return Token(TokenType::IF, id, line_, start_col);
    if (id == "else") return Token(TokenType::ELSE, id, line_, start_col);
    if (id == "end") return Token(TokenType::END, id, line_, start_col);

    return Token(TokenType::IDENTIFIER, id, line_, start_col);
}

Token Lexer::read_string() {
    int start_col = col_;
    advance();

    std::string str;
    while (!is_at_end() && peek() != '"') {
        if (peek() == '\n') {
            throw std::runtime_error(
                "Lexer error at line " + std::to_string(line_) +
                ":unterminated string literal");
        }
        str += advance();
        
    }

    if (is_at_end()) {
        throw std::runtime_error(
            "Lexer error at line " + std::to_string(line_) +
            ":unterminated string literal");
    }

    advance();
    return Token(TokenType::STRING, str, line_, start_col);
}

//====对外接口====
std::vector<Token> Lexer::tokenize(const std::string& source) {
    source_ = &source;
    pos_ = 0;
    line_ = 1;
    col_ = 1;

    std::vector<Token> tokens;

    while (!is_at_end()) {
        skip_whitespace();
        if (is_at_end()) break;

        char c = peek();

        // 注释:#到行尾
        if (c == '#') {
            skip_comment();
            continue;
        }

        //数字:0-9开头
        if (c >= '0' && c <= '9') {
            tokens.push_back(read_number());
            continue;
        }

        //标识符和关键字:字母或下划线开头
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            tokens.push_back(read_identifier());
            continue;
        }

        //字符串:双引号开头
        if (c == '"') {
            tokens.push_back(read_string());
            continue;
        }

        //单字符/双字符符号
        int start_line = line_;
        int start_col = col_;

        switch(c) {
            case '+': advance(); tokens.emplace_back(TokenType::PLUS, "+", start_line, start_col); break;
            case '-': advance(); tokens.emplace_back(TokenType::MINUS, "-", start_line, start_col); break;
            case '*': advance(); tokens.emplace_back(TokenType::STAR, "*", start_line, start_col); break;
            case '/': advance(); tokens.emplace_back(TokenType::SLASH, "/", start_line, start_col); break;
            case '(': advance(); tokens.emplace_back(TokenType::LPAREN, "(", start_line, start_col); break;
            case ')': advance(); tokens.emplace_back(TokenType::RPAREN, ")", start_line, start_col); break;
            case ',': advance(); tokens.emplace_back(TokenType::COMMA, ",", start_line, start_col); break;
            case '.': advance(); tokens.emplace_back(TokenType::DOT, ".", start_line, start_col); break;
            
            case '=':
                advance();
                if (!is_at_end() && peek() == '=') {
                    advance();
                    tokens.emplace_back(TokenType::EQ_EQ, "==", start_line, start_col);
                } else {
                    tokens.emplace_back(TokenType::EQUAL, "=", start_line, start_col);
                }
                break;

            case '>':
                advance();
                if (!is_at_end() && peek() == '=') {
                    advance();
                    tokens.emplace_back(TokenType::GREATER_EQ, ">=", start_line, start_col);
                } else {
                    tokens.emplace_back(TokenType::GREATER, ">", start_line, start_col);
                }
                break;

            case '<':
                advance();
                if (!is_at_end() && peek() == '=') {
                    advance();
                    tokens.emplace_back(TokenType::LESS_EQ, "<=", start_line, start_col);
                } else {
                    tokens.emplace_back(TokenType::LESS, "<", start_line, start_col);
                }
                break;

            case '!':
                advance();
                if (!is_at_end() && peek() == '=') {
                    advance();
                    tokens.emplace_back(TokenType::NOT_EQ, "!=", start_line, start_col);
                } else {
                    throw std::runtime_error(
                        "Lexer error at line " + std::to_string(line_) +
                        ": unexpected '!' (did you mean '!='?)");
                    break;
                }
                break;

            default:
                throw std::runtime_error(
                    "Lexer error at line " + std::to_string(line_) +
                    ", col " + std::to_string(col_) +
                    ": unexpected character '" + std::string(1, c) + "'");
                break;
        }
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", line_, col_);
    return tokens;
}

} // namespace pinecpp
#pragma once              // 防止头文件被重复包含
#include <string>
#include <iostream>

namespace pinecpp {

// 每种 Token 类型对应 PineScript 语言里的一种语法元素
enum class TokenType {
    // ----- 关键字 -----
    IF,          // if
    ELSE,        // else
    END,         // end（结束 if 块）

    // ----- 字面量 -----
    IDENTIFIER,  // 变量名或函数名，比如 sma、close、length
    NUMBER,      // 数字，比如 14、3.14
    STRING,      // 字符串，"long"

    // ----- 算术运算符 -----
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /

    // ----- 比较运算符 -----
    GREATER,     // >
    GREATER_EQ,  // >=
    LESS,        // <
    LESS_EQ,     // <=
    EQ_EQ,       // ==
    NOT_EQ,      // !=

    // ----- 赋值 -----
    EQUAL,       // =

    // ----- 分隔符 -----
    LPAREN,      // (
    RPAREN,      // )
    COMMA,       // ,
    DOT,         // .

    // ----- 特殊 -----
    END_OF_FILE, // 读到文件末尾时发出的信号
    ERROR,       // 非法字符
};

// 一个 Token = 类型 + 原始文本 + 在源码中的位置
// 保存行列号是为了报错时能精确定位——"第 3 行第 5 列：期望 ')'"
struct Token {
    TokenType type;
    std::string lexeme;   // 原始文本，"sma"、"14"、"+" 等
    int line;             // 第几行
    int column;           // 第几列

    Token(TokenType t, std::string lex, int l, int c)
        : type(t), lexeme(std::move(lex)), line(l), column(c) {}
};

// 让 Token 可以直接用 cout 打印（调试用）
inline std::ostream& operator<<(std::ostream& os, const Token& tok) {
    os << "Token{type=" << static_cast<int>(tok.type)
       << ", lexeme='" << tok.lexeme << "'"
       << ", line=" << tok.line
       << ", col=" << tok.column << "}";
    return os;
}

} // namespace mini_pine
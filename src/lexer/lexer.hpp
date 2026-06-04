#pragma once
#include "token.hpp"
#include <vector>
#include <string>

namespace pinecpp {

class Lexer {
public:
    //唯一对外接口：输入源码字符串，返回Token列表
    std::vector<Token> tokenize(const std::string& source);

private:
    const std::string* source_; // 源码字符串的指针
    int pos_; // 当前读取到的位置
    int line_; // 当前行号
    int col_; // 当前列列号

    //字符级操作
    char peek() const;  // 查看当前位置的字符,不移动指针
    char advance();  // 返回当前字符,指针+1,列号+1
    bool is_at_end() const; // 是否到达文件末尾
    
    //跳过空格和注释
    void skip_whitespace(); // 跳过空格、tab、换行符
    void skip_comment(); // 跳过#到行尾

    //读取Token
    Token read_number(); // 读取数字(含小数点)
    Token read_identifier(); // 读取标识符->判断是不是关键字
    Token read_string(); // 读取双引号字符串
};
    
} // namespace pinecpp
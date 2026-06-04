#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr <<  "Usage: pinecpp <script.pines>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr <<  "Error: Failed to open file " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    pinecpp::Lexer lexer;
    auto tokens = lexer.tokenize(source);

    for (const auto& tok : tokens) {
        std::cout << tok << "\n";
    }
    
    return 0;
}
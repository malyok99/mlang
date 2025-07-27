#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

#include "./tokenization.hpp"

std::string TokensToAsm(const std::vector<Token>& tokens) {
  std::stringstream output;
  output << "global _start\n";
  output << "_start:\n";

  for (int i = 0; i < tokens.size(); ++i) {
    const Token& token = tokens.at(i);

    if (token.type == TokenType::kExit) {
      if (i + 1 < tokens.size() &&
          tokens.at(i + 1).type == TokenType::kIntLiteral &&
          i + 2 < tokens.size() &&
          tokens.at(i + 2).type == TokenType::kSemicolon) {
        output << "  mov rax, 60\n";
        output << "  mov rdi, " << tokens.at(i + 1).value.value() << "\n";
        output << "  syscall\n";
      }
    }
  }

  return output.str();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Incorrect usage. Correct usage is:\n";
    std::cerr << "./mlang <input.m>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string contents;
  {
    std::stringstream contents_stream;
    std::fstream input(argv[1], std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
  }

  Tokenizer tokenizer(std::move(contents));
  std::vector<Token> tokens = tokenizer.tokenize();
  {
    std::fstream file("../out.asm", std::ios::out);
    file << TokensToAsm(tokens);
  }

  return EXIT_SUCCESS;
}

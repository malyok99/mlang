#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

enum class TokenType {
  kExit,
  kIntLiteral,
  kSemicolon
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
};

std::vector<Token> Tokenize(const std::string& str) {
  std::vector<Token> tokens;
  std::string buffer;

  for (int i = 0; i < str.length(); ++i) {
    char c = str.at(i);

    if (std::isalpha(c)) {
      buffer.push_back(c);
      ++i;
      while (i < str.length() && std::isalnum(str.at(i))) {
        buffer.push_back(str.at(i));
        ++i;
      }
      --i;

      if (buffer == "exit") {
        tokens.push_back({.type = TokenType::kExit});
        buffer.clear();
        continue;
      } else {
        std::cerr << "You messed up!" << std::endl;
        exit(EXIT_FAILURE);
      }

    } else if (std::isdigit(c)) {
      buffer.push_back(c);
      ++i;
      while (i < str.length() && std::isdigit(str.at(i))) {
        buffer.push_back(str.at(i));
        ++i;
      }
      --i;

      tokens.push_back({.type = TokenType::kIntLiteral, .value = buffer});
      buffer.clear();

    } else if (c == ';') {
      tokens.push_back({.type = TokenType::kSemicolon});

    } else if (std::isspace(c)) {
      continue;

    } else {
      std::cerr << "You messed up!" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return tokens;
}

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
    std::cerr << "  mlang <input.hy>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string contents;
  {
    std::stringstream contents_stream;
    std::fstream input(argv[1], std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
  }

  std::vector<Token> tokens = Tokenize(contents);

  {
    std::fstream file("../out.asm", std::ios::out);
    file << TokensToAsm(tokens);
  }

  return EXIT_SUCCESS;
}

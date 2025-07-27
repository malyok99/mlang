#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <optional>

enum class TokenType {
  kExit,
  kIntLiteral,
  kSemicolon
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
};

class Tokenizer {
public:
  inline explicit Tokenizer(const std::string src)
    : m_src(std::move(src))
  {
    
  }

  inline std::vector<Token> tokenize() {

    std::vector<Token> tokens;

    std::string buffer;

    while (peak().has_value()){
      if (std::isalpha(peak().value())) {
        buffer.push_back(consume());
        while (peak().has_value() && std::isalnum(peak().value())) {
          buffer.push_back(consume());
        }
        if (buffer == "exit") {
          tokens.push_back({ .type = TokenType::kExit });
          buffer.clear();
          continue;
        } else {
          std::cerr << "We got an error :(" << std::endl;
          exit(EXIT_FAILURE);
        }
      } else if (std::isdigit(peak().value())) {
        buffer.push_back(consume());
        while (peak().has_value() && std::isdigit(peak().value())){
          buffer.push_back(consume());
        }
        tokens.push_back({ .type = TokenType::kIntLiteral, .value = buffer });
        buffer.clear();
        continue;
      } else if (peak().value() == ';'){
        tokens.push_back({ .type = TokenType::kSemicolon });
        consume();
        continue;
      } else if (std::isspace(peak().value())) {
        consume();
        continue;
      } else {
          std::cerr << "We got an error :(" << std::endl;
          exit(EXIT_FAILURE);
      }
    }
    m_index = 0;
    return tokens;
  }

private:

  [[nodiscard]] std::optional<char> peak(int ahead = 1) const {
    if (m_index + ahead > m_src.length()) {
      return {};
    } else {
      return m_src.at(m_index);
    }
  }

  char consume() {
    return m_src.at(m_index++);
  }

  const std::string m_src;
  int m_index = 0;
};

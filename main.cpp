#include <iostream>
#include <memory>
#include <string>
#include <cstdint>

#include "bot/bot.hpp"
#include "word_generator/word_generator.hpp"

int main(int argc, char* argv[]) {
  if (argc < 4 || argc > 5) {
    std::cerr << "Usage:\n"
              << "  bot <telegram_token_path> <words_path> <state_path> [seed]\n";
    return 1;
  }

  const std::string token_path = argv[1];
  const std::string words_path = argv[2];
  const std::string state_path = argv[3];

  uint64_t seed = std::random_device{}();
  if (argc == 5) {
    try {
      seed = std::stoull(argv[4]);
    } catch (...) {
      std::cerr << "Invalid seed value\n";
      return 1;
    }
  }

  // Чтение токена
  std::ifstream token_file(token_path);
  if (!token_file.is_open()) {
    std::cerr << "Cannot open token file: " << token_path << "\n";
    return 1;
  }

  std::string token;
  std::getline(token_file, token);

  if (token.empty()) {
    std::cerr << "Token file is empty\n";
    return 1;
  }

  try {
    WordGenerator generator(words_path, seed);

    HangmanTgBot bot(token, generator, state_path);
    bot.Run();

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
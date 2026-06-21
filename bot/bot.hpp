#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <tgbot/tgbot.h>

#include "../game/hangman.hpp"
#include "../word_generator/word_generator.hpp"

class HangmanTgBot {
 private:
  TgBot::Bot bot_;
  WordGenerator word_generator_;
  std::string state_path_;
  // Хранилицщ игровых сессий: ID чата -> объект игры
  std::unordered_map<int64_t, HangmanGame> user_games_;

  // Регистрация всех команд и сообщений
  void RegisterHandlers();
  
  // Обработка текстовых сообщений (попытки угадать буквы)
  void HandleMessage(TgBot::Message::Ptr message);

  // Управление файлом состояния
  void SaveState() const;
  void LoadState();

  // Метод вывода текущего статуса игры пользователю
  std::string FormatGameStatus(const HangmanGame& game) const;

 public:
  // Конструктор принимает токет бота, генератор слов и пути к файлам
  HangmanTgBot(std::string token, WordGenerator generator, std::string state_path);

  // Запуск бесконечного цикла работы бота
  void Run();
};
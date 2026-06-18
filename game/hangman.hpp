#pragma once

#include <string>
#include <string_view>
#include <stdexcept>

class HangmanGame {
  std::string secret_word_;   // Загаданное слово
  std::string current_guess_; // Текущее состояние маски (отображения букв слова)
  int wrong_guesses_ = 0;     // Количество промахов - неудачных попыток отгадать букву

 public:
  // Константы игры
  static constexpr int kMaxWrongGuesses = 6;
  static constexpr char kMaskChar = '_';

  // Конструктор по умолчанию
  HangmanGame() = default;

  // Конструктор инициализации новой игры с загадынным словом
  explicit HangmanGame(std::string word) 
          : secret_word_(std::move(word)), current_guess_(secret_word_.size(), kMaskChar), wrong_guesses_(0) {
    if (secret_word_.empty()) {
      throw std::invalid_argument("Где секретное слово? Оно не должно быть пустым!");
    }
  }

  // обработка попытки угадывания буквы
  bool Guess(char letter) {
    // Если игра завершена, то новые буквы не обрабатываются
    if (IsWin() || IsLose()) {
      return false;
    }
    bool letter_found = false;
    // Проходим по слову за один проход
    for (size_t i = 0; i < secret_word_.size(); ++i) {
      if (secret_word_[i] == letter) {
        current_guess_[i] = letter;  // Если буква совпала - открываем ее в маске
        letter_found = true;
      }
    }
    if (!letter_found) {
      ++wrong_guesses_;  // Если буква не найдена, засчитываем ошбку
    }
    return letter_found;
  }

  // Проверка условий победы
  bool IsWin() const {
    return current_guess_ == secret_word_;
  }

  // Проверка условий поражения
  bool IsLose() const {
    return wrong_guesses_ >= kMaxWrongGuesses;
  }

  // Функция возврата текущей маски угадывания
  std::string_view GetGuess() const {
    return current_guess_;
  }

  // Функция возвращает исходное загаданное слово
  std::string_view GetWord() const {
    return secret_word_;
  }

  // Функция возвращает количество совершенных ошибок
  int GetWrongGuesses() const {
    return wrong_guesses_;
  }

  // Методы сериализации и десериализации
  // Позволяет вручную установить состяние маски при загрузке из файла
  void SetCurrentGuesses(std::string guess) {
    current_guess_ = std::move(guess);
  }

  // Позволяет вручную установить счетчик ошибок при загрузке из файла
  void SetWrongGuesses(int count) {
    wrong_guesses_ = count;
  }
};
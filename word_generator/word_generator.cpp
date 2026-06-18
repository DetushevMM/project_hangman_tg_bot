#include "word_generator.hpp"
#include <fstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

// Конструктор инициализирует генератор и сразу загружает слова
WordGenerator::WordGenerator(const std::string& words_path, uint64_t seed) : rng_(seed) {
  LoadWordsFromFile(words_path);
}

// Функция загрузки файлов
void WordGenerator::LoadWordsFromFile(const std::string& path) {
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error("Файл со словами не существует: " + path);
  }
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Ошибка открытия файла со словами: " + path);
  }
  // Создадим временный вектор
  std::vector<std::string> new_words;
  std::string token;

  // Читаем слова из файла и переносим их в вектор без копирования
  while (file >> token) {
    if (IsValidWord(token)) {
      NormalizeWord(token);
      new_words.push_back(std::move(token));
    }
  }
  // Проверяем, что словарь не пустой
  if (new_words.empty()) {
    throw std::runtime_error("Файл со словами пустой или содержит невалидные слова: " + path);
  }
  // Если всё прошло без ошибок, то заменим старый вектор новым
  words_ = std::move(new_words);
}

// Функция выдачи случайного слова с равномерным распределением
std::string WordGenerator::GetRandomWord() {
  if (words_.empty()) {
    throw std::runtime_error("Словарь пустой");
  }
  // Распределение настраивается на текущий размер вектора
  std::uniform_int_distribution<size_t> dist(0, words_.size() - 1);
  size_t random_index = dist(rng_);
  return words_[random_index];
}

size_t WordGenerator::GetDictionarySize() const {
  return words_.size();
}

// Приватный метод валидации
bool WordGenerator::IsValidWord(const std::string& word) const {
  if (word.empty()) {
    return false;
  }
  for (char c : word) {
    if (!std::isalpha(static_cast<unsigned char>(c))) {
      return false;
    }
  }
  return true;
}

// Приватный метод нормализации: возвращает нормализованную строку
void WordGenerator::NormalizeWord(std::string& word) {
  std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}
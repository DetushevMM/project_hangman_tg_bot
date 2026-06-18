#pragma once

#include <string>
#include <vector>
#include <random>

class WordGenerator {
  std::vector<std::string> words_;  // Контейнер для хранения слов в памяти
  std::mt19937_64 rng_;             // Генератор псевдослучайных чисел

  // Метод для проверки, что слово состояит только из букв
  bool IsValidWord(const std::string& word) const;

  // Метод переводит слово в нижний регистр
  void NormalizeWord(std::string& word);

 public:
  // Конструктор. Принимает путь к файлу слов и точку старта генератора случайных чисел
  explicit WordGenerator(const std::string& words_path, uint64_t seed);

  // Метод чтения словаря из файла
  void LoadWordsFromFile(const std::string& path);

  // Метод возвращает случайное слово из загруженного словаря
  std::string GetRandomWord();

  // Метод возвращает текущй размер словаря
  size_t GetDictionarySize() const;
};
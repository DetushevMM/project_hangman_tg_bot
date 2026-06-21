#include "bot.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cctype>

// =======================
// Конструктор
// =======================

HangmanTgBot::HangmanTgBot(std::string token,
                           WordGenerator generator,
                           std::string state_path)
    : bot_(std::move(token)),
      word_generator_(std::move(generator)),
      state_path_(std::move(state_path)) {
  LoadState();
  RegisterHandlers();
}

// =======================
// Регистрация обработчиков
// =======================

void HangmanTgBot::RegisterHandlers() {
  // /start
  bot_.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
    bot_.getApi().sendMessage(
        message->chat->id,
        "Привет! Игра 'Виселица'.\n"
        "/newgame - начать игру\n"
        "/help - помощь");
  });

  // /help
  bot_.getEvents().onCommand("help", [this](TgBot::Message::Ptr message) {
    bot_.getApi().sendMessage(
        message->chat->id,
        "/newgame - новая игра\n"
        "Отправляй одну букву для хода");
  });

  // /newgame
  bot_.getEvents().onCommand("newgame", [this](TgBot::Message::Ptr message) {
    try {
      std::string word = word_generator_.GetRandomWord();
      user_games_[message->chat->id] = HangmanGame(word);

      SaveState();

      bot_.getApi().sendMessage(
          message->chat->id,
          "Новая игра началась!\n" +
              FormatGameStatus(user_games_[message->chat->id]));
    } catch (const std::exception& e) {
      bot_.getApi().sendMessage(message->chat->id,
                                 std::string("Ошибка: ") + e.what());
    }
  });

  // все сообщения
  bot_.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    if (message->text.rfind("/", 0) == 0) {
      return;
    }
    HandleMessage(message);
  });
}

// =======================
// Обработка сообщений
// =======================

void HangmanTgBot::HandleMessage(TgBot::Message::Ptr message) {
  int64_t chat_id = message->chat->id;
  std::string text = message->text;

  auto it = user_games_.find(chat_id);

  if (it == user_games_.end() ||
      it->second.IsWin() ||
      it->second.IsLose()) {
    bot_.getApi().sendMessage(chat_id,
                              "Игра не активна. Используй /newgame");
    return;
  }

  if (text.size() != 1 || !std::isalpha(static_cast<unsigned char>(text[0]))) {
    bot_.getApi().sendMessage(chat_id, "Введите одну букву.");
    return;
  }

  char letter = static_cast<char>(
      std::tolower(static_cast<unsigned char>(text[0])));

  HangmanGame& game = it->second;

  bool hit = game.Guess(letter);

  SaveState();

  std::string response = hit ? "Есть буква!\n" : "Нет такой буквы.\n";
  response += FormatGameStatus(game);

  if (game.IsWin()) {
    response += "\n\nВы победили! Слово: " +
                std::string(game.GetWord());
  } else if (game.IsLose()) {
    response += "\n\nВы проиграли! Слово: " +
                std::string(game.GetWord());
  }

  bot_.getApi().sendMessage(chat_id, response);
}

// =======================
// Формат состояния игры
// =======================

std::string HangmanTgBot::FormatGameStatus(
    const HangmanGame& game) const {
  std::stringstream ss;
  ss << "Слово: " << game.GetGuess() << "\n";
  ss << "Ошибки: " << game.GetWrongGuesses()
     << " / " << HangmanGame::kMaxWrongGuesses;
  return ss.str();
}

// =======================
// Сохранение состояния
// =======================

void HangmanTgBot::SaveState() const {
  std::string tmp = state_path_ + ".tmp";
  std::ofstream file(tmp);

  if (!file.is_open()) {
    std::cerr << "SaveState error\n";
    return;
  }

  for (const auto& [chat_id, game] : user_games_) {
    file << chat_id << " "
         << game.GetWord() << " "
         << game.GetGuess() << " "
         << game.GetWrongGuesses() << "\n";
  }

  file.close();

  std::error_code ec;
  std::filesystem::rename(tmp, state_path_, ec);

  if (ec) {
    std::cerr << "Rename error: " << ec.message() << "\n";
  }
}

// =======================
// Загрузка состояния
// =======================

void HangmanTgBot::LoadState() {
  std::ifstream file(state_path_);
  if (!file.is_open()) {
    return;
  }

  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);

    int64_t chat_id;
    std::string word, guess;
    int wrong;

    if (!(ss >> chat_id >> word >> guess >> wrong)) {
      continue;
    }

    HangmanGame game(word);
    game.SetCurrentGuess(guess);
    game.SetWrongGuesses(wrong);

    user_games_[chat_id] = std::move(game);
  }
}

// =======================
// Запуск бота
// =======================

void HangmanTgBot::Run() {
  bot_.getApi().deleteWebhook();

  std::cout << "Bot started\n";

  TgBot::TgLongPoll long_poll(bot_);

  while (true) {
    try {
      long_poll.start();
    } catch (const TgBot::TgException& e) {
      std::cerr << "Telegram error: " << e.what() << "\n";
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }
}
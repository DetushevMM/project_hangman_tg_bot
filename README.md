🎮 Hangman Telegram Bot (C++20)

Telegram-бот для игры в «Виселицу», реализованный на C++20 с использованием библиотеки tgbot-cpp.


---

📌 Возможности

Игра «Виселица» в Telegram

Поддержка нескольких пользователей (по chat_id)

Сохранение и восстановление игровых сессий

Случайный выбор слова из словаря

Поддержка команд /start, /help, /newgame

Обработка ввода букв



---

🧱 Архитектура проекта

Проект разделён на 3 основных модуля:

🎯 1. Game (логика игры)

game/hangman.hpp

Отвечает за:
- хранение слова
- маску угадывания
- количество ошибок
- проверку победы/поражения



---

📚 2. Word Generator

word_generator/

Функции:
- загрузка слов из файла
- фильтрация невалидных слов
- приведение к нижнему регистру
- случайный выбор слова (mt19937_64)


---

🤖 3. Telegram Bot

bot/

Функции:
- обработка сообщений Telegram
- маршрутизация команд
- хранение состояния игр
- сохранение/загрузка state


---

📦 Требования

Система (Ubuntu / WSL)

- sudo apt update
- sudo apt install -y \
g++ \
cmake \
git \
make \
libssl-dev \
zlib1g-dev \
libcurl4-openssl-dev \
libboost-all-dev


---

Библиотека TgBot

- git clone https://github.com/reo7sp/tgbot-cpp.git
- cd tgbot-cpp
- mkdir build && cd build
- cmake ..
- make -j$(nproc)
- sudo make install
- sudo ldconfig


---

⚙️ Сборка проекта

- mkdir build
- cd build
- cmake ..
- make -j$(nproc)


---

🚀 Запуск

./bot <telegram_token_path> <words_path> <state_path> [seed]

Пример:

./bot ../token.txt ../words.txt ../state.txt


---

📄 Формат файлов

token.txt

123456789:AAHfGzxxxxxxxxxxxxxxxxxxxxxx


---

words.txt

apple
banana
computer
hangman
developer


---

state.txt

Файл состояния (создаётся автоматически):

chat_id - secret_word - mask - wrong_guesses


---

🎮 Команды бота

/start

Приветствие и инструкция

/help

Список доступных команд

/newgame

Начать новую игру


---

🎯 Игровой процесс

пользователь отправляет букву

бот проверяет наличие в слове

обновляет маску

считает ошибки



---

💾 Сохранение состояния

хранится в state.txt

обновляется после каждого хода

восстанавливается при перезапуске



---

⚠️ Ограничения

только английские буквы (рекомендуется)

одна буква за сообщение

максимум ошибок: 6



---

🧠 Технологии

C++20

STL (unordered_map, string, vector)

TgBot-cpp

OpenSSL

libcurl

Boost (зависимость TgBot)

---

🚀 Статус

✔ MVP готов
✔ Логика игры реализована
✔ Telegram интеграция подключена
✔ Сборка через CMake

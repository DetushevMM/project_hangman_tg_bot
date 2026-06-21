cmake_minimum_required(VERSION 3.16)

project(HangmanBot VERSION 1.0 LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Поиск необходимых библиотек
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# Исполняемый файл
add_executable(bot
    main.cpp
    bot/bot.cpp
    word_generator/word_generator.cpp
)

# Пути к заголовочным файлам
target_include_directories(bot PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/game
    ${PROJECT_SOURCE_DIR}/word_generator
    ${PROJECT_SOURCE_DIR}/bot
    /usr/local/include
)

# Линковка библиотек
target_link_libraries(bot PRIVATE
    TgBot
    OpenSSL::SSL
    OpenSSL::Crypto
    CURL::libcurl
    Threads::Threads
)

# Предупреждения компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(bot PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
//
// Created by mavri on 26.11.2024.
//

#include "Logger.h"

// TODO: test the windows implementation

void Logger::SetConsoleColor(Color color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color) {
        case Color::Red:
            SetConsoleTextAttribute(hConsole, 12);
            break;
        case Color::Green:
            SetConsoleTextAttribute(hConsole, 10);
            break;
        case Color::Blue:
            SetConsoleTextAttribute(hConsole, 9);
            break;
        case Color::Purple:
            SetConsoleTextAttribute(hConsole, 13);
            break;
        default:
            SetConsoleTextAttribute(hConsole, 7);
            break;
    }
#else
    switch (color) {
        case Color::Red:
            std::cout << "\033[1;31m";
            break;
        case Color::Green:
            std::cout << "\033[1;32m";
            break;
        case Color::Blue:
            std::cout << "\033[1;34m";
            break;
        case Color::Purple:
            std::cout << "\033[1;35m";
            break;
        default:
            std::cout << "\033[0m";
            break;
    }
#endif
}

void Logger::ResetConsoleColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
#else
    std::cout << "\033[0m";
#endif
}

template<typename T>
void Logger::Log(const Color &color, const T &message) {
    SetConsoleColor(color);
    std::cout << message;
    ResetConsoleColor();
}

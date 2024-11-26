//
// Created by mavri on 26.11.2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

class Logger {
public:
    enum class Color { Default, Red, Green, Blue, Purple };

    template<typename T>
    static void Log(const Color &color, const T &message);

private:
    static void SetConsoleColor(Color color);
    static void ResetConsoleColor();
};


#endif // LOGGER_H

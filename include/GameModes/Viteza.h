// //
// // Created by edi on 17.01.2025.
// //
//
// #ifndef VITEZA_H
// #define VITEZA_H
//
// #include "Game.h"
//
// class Viteza {
//     std::unique_ptr<Game> m_Game;
//
// public:
//     void                                SetGameMode(std::unique_ptr<Game> game);
//     [[nodiscard]] std::unique_ptr<Game> GetGameMode();
//     Viteza(const Viteza &other)            = default;
//     Viteza(Viteza &&other) noexcept        = default;
//     Viteza &operator=(const Viteza &other) = default;
//     ~Viteza()                              = default;
//     explicit Viteza(std::unique_ptr<Game> game);
//     Viteza()                                   = default;
//     Viteza &operator=(Viteza &&other) noexcept = default;
// };
//
// #endif // VITEZA_H

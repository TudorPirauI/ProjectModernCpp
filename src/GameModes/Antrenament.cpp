#include "GameModes/Antrenament.h"

// Modul antrenament: tabla este de 3x3; fiecare jucător are 7 cărți: 1, 1, 2, 2, 3, 3, 4; Format
// meci: 2/3;

const auto CREATE_CARDS = [](const std::array<int, 7> &values, const PlayerTurn &player) {
    std::vector<Card> cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

Antrenament::Antrenament(const std::string &nameOne, const std::string &nameTwo,
                         const std::array<bool, 3> &options) :
    Game(3, 2, nameOne, nameTwo, options) {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);
}

void Antrenament::SetNewCards() {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_Board.CleanUpBoard();
}

// void Antrenament::to_json(nlohmann::json &j, Antrenament &antrenament) {
//     j = nlohmann::json{{"boardSize", antrenament.GetBoard().GetMaxBoardSize()},
//                        {"scoreToWin", antrenament.GetScoreToWin()},
//                        {"player1", antrenament.GetPlayer1()},
//                        {"player2", antrenament.GetPlayer2()},
//                        {"options",
//                         {antrenament.GetEterEnabled(), antrenament.GetIllusionEnabled(),
//                          antrenament.ExplosionEnabled()}},
//                        {"currentPlayer", antrenament.GetCurrentPlayerTurn()},
//                        {"scorePlayer1", antrenament.GetPlayer1Score()},
//                        {"scorePlayer2", antrenament.GetPlayer2Score()},
//                        {"lastPositionPlayer1", antrenament.GetLastCardPlayer1()},
//                        {"lastPositionPlayer2", antrenament.GetLastCardPlayer2()},
//                        {"rowPlayer1", antrenament.GetRowPlayer1()},
//                        {"rowPlayer2", antrenament.GetRowPlayer2()}};
// }
//
// void Antrenament::from_json(const nlohmann::json &j, Antrenament &antrenament) {
//     int                 boardSize, scoreToWin;
//     std::string         nameOne, nameTwo;
//     std::array<bool, 3> options;
//     PlayerTurn          currentPlayer;
//     Game::GameState     gameState;
//     int                 scorePlayer1, scorePlayer2;
//     Position            lastPositionPlayer1, lastPositionPlayer2;
//     int                 rowPlayer1, rowPlayer2;
//
//     j.at("boardSize").get_to(boardSize);
//     j.at("scoreToWin").get_to(scoreToWin);
//     j.at("player1").get_to(nameOne);
//     j.at("player2").get_to(nameTwo);
//     j.at("options").get_to(options);
//     j.at("currentPlayer").get_to(currentPlayer);
//     j.at("gameState").get_to(gameState);
//     j.at("scorePlayer1").get_to(scorePlayer1);
//     j.at("scorePlayer2").get_to(scorePlayer2);
//     j.at("lastPositionPlayer1").get_to(lastPositionPlayer1);
//     j.at("lastPositionPlayer2").get_to(lastPositionPlayer2);
//     j.at("rowPlayer1").get_to(rowPlayer1);
//     j.at("rowPlayer2").get_to(rowPlayer2);
//
//     antrenament = Antrenament(nameOne, nameTwo, options);
//     antrenament.SetNextPlayerTurn(currentPlayer);
//     antrenament.SetGameState(gameState);
//     antrenament.IncreasePlayerScore(PlayerTurn::Player1);
//     antrenament.IncreasePlayerScore(PlayerTurn::Player2);
//     antrenament.SetLastCardPlayer1(lastPositionPlayer1);
//     antrenament.SetLastCardPlayer2(lastPositionPlayer2);
//     antrenament.SetRowPlayer1(rowPlayer1);
//     antrenament.SetRowPlayer2(rowPlayer2);
// }

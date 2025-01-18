#include "GameModes/DuelulVrajitorilor.h"

const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    std::vector<Card> cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

void DuelulVrajitorilor::SetNewCards() {
    m_Player1.SetHand(
            CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1)); // Creează cărțile direct aici
    m_Player2.SetHand(CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2));

    Card cardEter(1, PlayerTurn::Player1); // Inițializare directă cu constructor
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);

    cardEter.SetPlacedBy(PlayerTurn::Player2);
    m_Player2.GiveCard(cardEter);

    m_Board.CleanUpBoard();
}

Wizard DuelulVrajitorilor::GetPlayerAbility1() const { return m_AbilityPlayer1; }

Wizard DuelulVrajitorilor::GetPlayerAbility2() const { return m_AbilityPlayer2; }

DuelulVrajitorilor::DuelulVrajitorilor(const std::string &nameOne, const std::string &nameTwo,
                                       const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options) {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_AbilityPlayer1.ResetPowerForNewMatch();
    m_AbilityPlayer2.ResetPowerForNewMatch();

    Card cardEter;

    cardEter.SetValue(1);
    cardEter.SetEter(true);
    cardEter.SetPlacedBy(PlayerTurn::Player1);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);
    m_Player2.GiveCard(cardEter);
}

void DuelulVrajitorilor::SetPlayerAbility1(const Wizard &ability) { m_AbilityPlayer1 = ability; }

void DuelulVrajitorilor::SetPlayerAbility2(const Wizard &ability) { m_AbilityPlayer2 = ability; }

// void DuelulVrajitorilor::to_json(nlohmann::json &j, DuelulVrajitorilor &duelulVrajitorilor) {
//     j = nlohmann::json{
//             {"boardSize", duelulVrajitorilor.GetBoard().GetMaxBoardSize()},
//             {"scoreToWin", duelulVrajitorilor.GetScoreToWin()},
//             {"player1", duelulVrajitorilor.GetPlayer1()},
//             {"player2", duelulVrajitorilor.GetPlayer2()},
//             {"options",
//              {duelulVrajitorilor.GetEterEnabled(), duelulVrajitorilor.GetIllusionEnabled(),
//               duelulVrajitorilor.ExplosionEnabled()}},
//             {"currentPlayer", duelulVrajitorilor.GetCurrentPlayerTurn()},
//             {"scorePlayer1", duelulVrajitorilor.GetPlayer1Score()},
//             {"scorePlayer2", duelulVrajitorilor.GetPlayer2Score()},
//             {"lastPositionPlayer1", duelulVrajitorilor.GetLastCardPlayer1()},
//             {"lastPositionPlayer2", duelulVrajitorilor.GetLastCardPlayer2()},
//             {"rowPlayer1", duelulVrajitorilor.GetRowPlayer1()},
//             {"rowPlayer2", duelulVrajitorilor.GetRowPlayer2()},
//             {"abilityPlayer1", duelulVrajitorilor.GetPlayerAbility1()},
//             {"abilityPlayer2", duelulVrajitorilor.GetPlayerAbility2()}};
// }
//
// void DuelulVrajitorilor::from_json(const nlohmann::json &j,
//                                    DuelulVrajitorilor   &duelulVrajitorilor) {
//     int                 boardSize, scoreToWin;
//     std::string         nameOne, nameTwo;
//     std::array<bool, 3> options;
//     PlayerTurn          currentPlayer;
//     Game::GameState     gameState;
//     int                 scorePlayer1, scorePlayer2;
//     Position            lastPositionPlayer1, lastPositionPlayer2;
//     int                 rowPlayer1, rowPlayer2;
//     Wizard              abilityPlayer1, abilityPlayer2;
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
//     j.at("abilityPlayer1").get_to(abilityPlayer1);
//     j.at("abilityPlayer2").get_to(abilityPlayer2);
//
//     duelulVrajitorilor = DuelulVrajitorilor(nameOne, nameTwo, options);
//     duelulVrajitorilor.SetNextPlayerTurn(currentPlayer);
//     duelulVrajitorilor.SetGameState(gameState);
//     duelulVrajitorilor.IncreasePlayerScore(PlayerTurn::Player1);
//     duelulVrajitorilor.IncreasePlayerScore(PlayerTurn::Player2);
//     duelulVrajitorilor.SetLastCardPlayer1(lastPositionPlayer1);
//     duelulVrajitorilor.SetLastCardPlayer2(lastPositionPlayer2);
//     duelulVrajitorilor.SetRowPlayer1(rowPlayer1);
//     duelulVrajitorilor.SetRowPlayer2(rowPlayer2);
//     duelulVrajitorilor.SetPlayerAbility1(abilityPlayer1);
//     duelulVrajitorilor.SetPlayerAbility2(abilityPlayer2);
// }

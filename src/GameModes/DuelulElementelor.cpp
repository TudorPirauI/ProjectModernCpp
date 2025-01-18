//
// Created by Miruna on 1/16/2025.
//

#include "GameModes/DuelulElementelor.h"
#include <array>

// Funcție pentru crearea setului de cărți inițial
const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    Hand cards; // Presupunând că Hand este definit ca std::vector<Card> sau similar
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

DuelulElementelor &DuelulElementelor::operator=(DuelulElementelor other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void DuelulElementelor::SetNewCards() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
    m_Board.CleanUpBoard();
}

ElementPower DuelulElementelor::GetPlayerAbility1() const { return m_AbilityPlayer1; }

ElementPower DuelulElementelor::GetPlayerAbility2() const { return m_AbilityPlayer2; }

DuelulElementelor::DuelulElementelor(const std::string &nameOne, const std::string &nameTwo,
                                     const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options), m_AbilityPlayer1(), m_AbilityPlayer2() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_AbilityPlayer1.RandomPower();
    m_AbilityPlayer2.RandomPower();

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
}

void DuelulElementelor::SetPlayerAbility1(const ElementPower &ability) {
    m_AbilityPlayer1 = ability;
}

void DuelulElementelor::SetPlayerAbility2(const ElementPower &ability) {
    m_AbilityPlayer2 = ability;
}

void to_json(nlohmann::json &j, DuelulElementelor &duelulElementelor) {
    j = nlohmann::json{{"boardSize", duelulElementelor.GetBoard().GetMaxBoardSize()},
                       {"scoreToWin", duelulElementelor.GetScoreToWin()},
                       {"player1", duelulElementelor.GetPlayer1()},
                       {"player2", duelulElementelor.GetPlayer2()},
                       {"options",
                        {duelulElementelor.GetEterEnabled(), duelulElementelor.GetIllusionEnabled(),
                         duelulElementelor.ExplosionEnabled()}},
                       {"currentPlayer", duelulElementelor.GetCurrentPlayerTurn()},
                       {"scorePlayer1", duelulElementelor.GetPlayer1Score()},
                       {"scorePlayer2", duelulElementelor.GetPlayer2Score()},
                       {"lastPositionPlayer1", duelulElementelor.GetLastCardPlayer1()},
                       {"lastPositionPlayer2", duelulElementelor.GetLastCardPlayer2()},
                       {"rowPlayer1", duelulElementelor.GetRowPlayer1()},
                       {"rowPlayer2", duelulElementelor.GetRowPlayer2()},
                       {"abilityPlayer1", duelulElementelor.GetPlayerAbility1()},
                       {"abilityPlayer2", duelulElementelor.GetPlayerAbility2()}};
}

void from_json(const nlohmann::json &j, DuelulElementelor &duelulElementelor) {
    int                 boardSize, scoreToWin;
    std::string         nameOne, nameTwo;
    std::array<bool, 3> options;
    PlayerTurn          currentPlayer;
    Game::GameState     gameState;
    int                 scorePlayer1, scorePlayer2;
    Position            lastPositionPlayer1, lastPositionPlayer2;
    int                 rowPlayer1, rowPlayer2;
    ElementPower        abilityPlayer1, abilityPlayer2;

    j.at("boardSize").get_to(boardSize);
    j.at("scoreToWin").get_to(scoreToWin);
    j.at("player1").get_to(nameOne);
    j.at("player2").get_to(nameTwo);
    j.at("options").get_to(options);
    j.at("currentPlayer").get_to(currentPlayer);
    j.at("gameState").get_to(gameState);
    j.at("scorePlayer1").get_to(scorePlayer1);
    j.at("scorePlayer2").get_to(scorePlayer2);
    j.at("lastPositionPlayer1").get_to(lastPositionPlayer1);
    j.at("lastPositionPlayer2").get_to(lastPositionPlayer2);
    j.at("rowPlayer1").get_to(rowPlayer1);
    j.at("rowPlayer2").get_to(rowPlayer2);
    j.at("abilityPlayer1").get_to(abilityPlayer1);
    j.at("abilityPlayer2").get_to(abilityPlayer2);

    duelulElementelor = DuelulElementelor(nameOne, nameTwo, options);
    duelulElementelor.SetNextPlayerTurn(currentPlayer);
    duelulElementelor.IncreasePlayerScore(PlayerTurn::Player1);
    duelulElementelor.IncreasePlayerScore(PlayerTurn::Player2);
    duelulElementelor.SetLastCardPlayer1(lastPositionPlayer1);
    duelulElementelor.SetLastCardPlayer2(lastPositionPlayer2);
    duelulElementelor.SetRowPlayer1(rowPlayer1);
    duelulElementelor.SetRowPlayer2(rowPlayer2);
    duelulElementelor.SetPlayerAbility1(abilityPlayer1);
    duelulElementelor.SetPlayerAbility2(abilityPlayer2);
}

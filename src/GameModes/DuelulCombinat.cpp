//
// Created by Miruna on 1/16/2025.
//

#include "GameModes/DuelulCombinat.h"
#include <array>
#include "GameComponents/Card.h"
#include "GameComponents/Player.h"

const auto CREATE_CARDS = [](const std::array<int, 9> &values, const PlayerTurn &player) {
    Hand cards;
    for (int value : values) {
        cards.emplace_back(value, player);
    }
    return cards;
};

constexpr std::array    CARD_VALUES{1, 1, 2, 2, 2, 3, 3, 3, 4};
const std::vector<Card> CARDS_PLAYER1 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player1);
const std::vector<Card> CARDS_PLAYER2 = CREATE_CARDS(CARD_VALUES, PlayerTurn::Player2);

DuelulCombinat &DuelulCombinat::operator=(DuelulCombinat other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

void DuelulCombinat::SetNewCards() {
    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
    m_Board.CleanUpBoard();
}

Wizard DuelulCombinat::GetWizardAbility() const { return m_WizardAbility1; }

ElementPower DuelulCombinat::GetElementalAbility() const { return m_ElementalAbility1; }

DuelulCombinat::DuelulCombinat(const std::string &nameOne, const std::string &nameTwo,
                               const std::array<bool, 3> &options) :
    Game(4, 5, nameOne, nameTwo, options), m_ElementalAbility1() {

    m_Player1.SetHand(CARDS_PLAYER1);
    m_Player2.SetHand(CARDS_PLAYER2);

    m_WizardAbility1.RandomPower();
    m_ElementalAbility1.RandomPower();

    m_WizardAbility2.RandomPower();
    m_ElementalAbility2.RandomPower();

    Card cardEter(1, PlayerTurn::Player1);
    cardEter.SetEter(true);

    m_Player1.GiveCard(cardEter);
    cardEter.SetPlacedBy(PlayerTurn::Player2);

    m_Player2.GiveCard(cardEter);
}

void DuelulCombinat::SetWizardAbility(const Wizard &wizardAbility) {
    m_WizardAbility1 = wizardAbility;
}

void DuelulCombinat::SetElementalAbility(const ElementPower &elementalAbility) {
    m_ElementalAbility1 = elementalAbility;
}

void to_json(nlohmann::json &j, DuelulCombinat &duelulCombinat) {
    j = nlohmann::json{{"boardSize", duelulCombinat.GetBoard().GetMaxBoardSize()},
                       {"scoreToWin", duelulCombinat.GetScoreToWin()},
                       {"player1", duelulCombinat.GetPlayer1()},
                       {"player2", duelulCombinat.GetPlayer2()},
                       {"options",
                        {duelulCombinat.GetEterEnabled(), duelulCombinat.GetIllusionEnabled(),
                         duelulCombinat.ExplosionEnabled()}},
                       {"currentPlayer", duelulCombinat.GetCurrentPlayer()},
                       {"scorePlayer1", duelulCombinat.GetPlayer1Score()},
                       {"scorePlayer2", duelulCombinat.GetPlayer2Score()},
                       {"lastPositionPlayer1", duelulCombinat.GetLastCardPlayer1()},
                       {"lastPositionPlayer2", duelulCombinat.GetLastCardPlayer2()},
                       {"rowPlayer1", duelulCombinat.GetRowPlayer1()},
                       {"rowPlayer2", duelulCombinat.GetRowPlayer2()},
                       {"wizardAbility1", duelulCombinat.GetWizardAbility()},
                       {"elementalAbility1", duelulCombinat.GetElementalAbility()}};
}

void from_json(const nlohmann::json &j, DuelulCombinat &duelulCombinat) {
    int                 boardSize, scoreToWin;
    std::string         nameOne, nameTwo;
    std::array<bool, 3> options;
    PlayerTurn          currentPlayer;
    Game::GameState     gameState;
    int                 scorePlayer1, scorePlayer2;
    Position            lastPositionPlayer1, lastPositionPlayer2;
    int                 rowPlayer1, rowPlayer2;
    Wizard              wizardAbility1;
    ElementPower        elementalAbility1;

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
    j.at("wizardAbility1").get_to(wizardAbility1);
    j.at("elementalAbility1").get_to(elementalAbility1);

    duelulCombinat = DuelulCombinat(nameOne, nameTwo, options);
    duelulCombinat.SetNextPlayerTurn(currentPlayer);
    duelulCombinat.SetGameState(gameState);
    duelulCombinat.IncreasePlayerScore(PlayerTurn::Player1);
    duelulCombinat.IncreasePlayerScore(PlayerTurn::Player2);
    duelulCombinat.SetLastCardPlayer1(lastPositionPlayer1);
    duelulCombinat.SetLastCardPlayer2(lastPositionPlayer2);
    duelulCombinat.SetRowPlayer1(rowPlayer1);
    duelulCombinat.SetRowPlayer2(rowPlayer2);
    duelulCombinat.SetWizardAbility(wizardAbility1);
    duelulCombinat.SetElementalAbility(elementalAbility1);
}

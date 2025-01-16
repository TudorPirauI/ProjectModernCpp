//
// Created by edi on 28.11.2024.
//

#ifndef GAME_H
#define GAME_H

#include "GameComponents/Board.h"
#include "GameComponents/Player.h"
#include "Powers/ElementPower.h"
#include "Powers/Wizard.h"

// todo: make another game mode using DuelulVrajitorilor and DuelulElementelor
// todo: restart the game or continue the last game
// todo: the same power that was used by a player can't be used by another player
// todo: DuelulElementelor - the players must have:
// fiecare jucător are 10 cărți: 1, 1, 2, 2, 2, 3,  3, 3, 4, Eter; se aleg 2 puteri distincte în mod
// aleatoriu; cele 2 puteri pot fi folosite de amândoi jucătorii; Format meci: 3/5

class Game {
public:
    enum class GameState { NotFinished, NormalWin, FinishedCards };

    virtual ~Game() = default;
    Game(int boardSize, int scoreToWin, const std::string &nameOne, const std::string &nameTwo);
    Game();

    [[nodiscard]] Board     &GetBoard();
    [[nodiscard]] Player    &GetPlayer1();
    [[nodiscard]] Player    &GetPlayer2();
    void                     SwitchPlayerTurn();
    [[nodiscard]] int        GetPlayer1Score() const;
    [[nodiscard]] int        GetPlayer2Score() const;
    [[nodiscard]] int        GetScoreToWin() const;
    [[nodiscard]] PlayerTurn GetCurrentPlayer() const;
    bool                     CheckWinningConditions();
    void                     SetGameState(GameState gameState);
    void                     IncreasePlayerScore(PlayerTurn turn);
    void                     SetNextPlayerTurn(PlayerTurn playerTurn);
    virtual void             SetNewCards() = 0;

    bool VerifyWizardPower(const WizardPower &power, const Position &position,
                           const Position &posStack, const Card &card,
                           const PlayerTurn &playerTurn);

    bool VerifyElementalPower(const ElementIndexPower &power, const Position &firstPosition,
                              const Position &secondPosition, const Card &card,
                              PlayerTurn playerTurn, int chosenNumber);

    Game(const Game &other)                = default;
    Game(Game &&other) noexcept            = default;
    Game &operator=(const Game &other)     = default;
    Game &operator=(Game &&other) noexcept = default;

    void SetLastCardPlayer1(const Position &position);
    void SetLastCardPlayer2(const Position &position);

    Position GetLastCardPlayer1();
    Position GetLastCardPlayer2();

    void SetIllusionEnabled(bool value);
    bool GetIllusionEnabled() const;

    int GetRowPlayer1();
    int GetRowPlayer2();

    bool CheckPlayerIllusion(Player &player);
    void LoadDataInJson();
    void SaveDataInJson();

protected:
    Board      m_Board;
    Player     m_Player1;
    Player     m_Player2;
    PlayerTurn m_PlayerTurn{PlayerTurn::Player1};
    GameState  m_GameState{GameState::NotFinished};
    int        m_ScorePlayer1{0};
    int        m_ScorePlayer2{0};
    int        m_RowPlayer1{-200};
    int        m_RowPlayer2{-200};
    Position   m_LastPositionPlayer1;
    Position   m_LastPositionPlayer2;
    int        m_ScoreToWin{};
    bool       m_IllusionEnabled{false};

    static constexpr std::string m_jsonFileName = "reload.json";

    Board    RemadeGameBoard(Board board);
    void     CheckModifierCard(std::stack<Card> &stack);
    CardType GetCardType(const Card &card);
};

#endif // GAME_H

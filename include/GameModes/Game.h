//
// Created by edi on 28.11.2024.
//

#ifndef GAME_H
#define GAME_H

#include "GameComponents/Board.h"
#include "GameComponents/Player.h"
#include "Powers/ElementPower.h"
#include "Powers/Explosion.h"
#include "Powers/Wizard.h"

class Game {
public:
    enum class GameState { NotFinished, NormalWin, FinishedCards };
    enum class WinningCondition {
        NoWin,
        LineWin,
        ColumnWin,
        DiagonalPrincipalWin,
        DiagonalSecondaryWin,
    };

    virtual ~Game() = default;
    Game(int boardSize, int scoreToWin, const std::string &nameOne, const std::string &nameTwo,
         const std::array<bool, 3> &options);
    Game();

    [[nodiscard]] Board     &GetBoard();
    [[nodiscard]] Player    &GetPlayer1();
    [[nodiscard]] Player    &GetPlayer2();
    void                     SwitchPlayerTurn();
    [[nodiscard]] int        GetPlayer1Score() const;
    [[nodiscard]] int        GetPlayer2Score() const;
    [[nodiscard]] int        GetScoreToWin() const;
    [[nodiscard]] PlayerTurn GetCurrentPlayer() const;
    WinningCondition         CheckWinningConditions();
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
    // void LoadDataInJson();
    // void SaveDataInJson();

    bool CheckExplosion();

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
    bool       m_EterEnabled{false};
    bool       m_ExplosionEnabled{false};

    static constexpr std::string JSON_FILE_NAME = "reload.json";

    Board    RemadeGameBoard(Board board);
    void     CheckModifierCard(std::stack<Card> &stack);
    CardType GetCardType(const Card &card);
};

#endif // GAME_H

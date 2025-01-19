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
    [[nodiscard]] int        GetPlayer1Score() const;
    [[nodiscard]] int        GetPlayer2Score() const;
    [[nodiscard]] int        GetScoreToWin() const;
    [[nodiscard]] PlayerTurn GetCurrentPlayerTurn() const;
    Player                  &GetCurrentPlayer();
    void                     SwapTurn();
    WinningCondition         CheckWinningConditions();
    void                     SwitchPlayerTurn();
    void                     SetGameState(GameState gameState);
    void                     IncreasePlayerScore(PlayerTurn turn);
    void                     SetNextPlayerTurn(PlayerTurn playerTurn);
    virtual void             SetNewCards();

    void SetPlayerTurn(const PlayerTurn &playerTurn);

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

    int GetRowPlayer1() const;
    int GetRowPlayer2() const;

    static bool CheckPlayerIllusion(Player &player);

    bool CheckExplosion();

    [[nodiscard]] bool GetIllusionEnabled() const;
    [[nodiscard]] bool GetEterEnabled() const;
    [[nodiscard]] bool ExplosionEnabled() const;
    void               SetRowPlayer1(int row);
    void               SetRowPlayer2(int row);

    // void LoadDataInJson();
    // void SaveDataInJson();

    std::pair<Position, Card> RecommendMove();

    bool     CanWinNextMove(const Player &player, const Card &card);
    bool     CanOpponentWinNextMove(const Player &opponent, const Card &card);
    bool     CanSetupNextMoveWin(const Player &player, const Card &card);
    bool     CanPlaceOverOpponentCard(const Player &player, const Card &card);
    Position GetWinningPosition(const Player &player, const Card &card);
    Position GetBlockingPosition(const Player &opponent, const Card &card);
    Position GetSetupPosition(const Player &player, const Card &card);
    Position GetPlacementPosition(const Player &player, const Card &card);
    Position GetDefaultPosition();
    void     SaveGameState();
    void     SetBoard(const GameBoard &map);

    void SetPlayer1(const Player &player);
    void SetPlayer2(const Player &player);

    void SetCurrentPlayerTurn(const PlayerTurn &playerTurn);

    void SetPlayer1Score(int score);
    void SetPlayer2Score(int score);

    void SetScoreToWin(int scoreToWin);
    void SetIllusionEnabled(bool value);
    void SetEterEnabled(bool value);
    void SetExplosionEnabled(bool value);

    // void     to_json(nlohmann::json &j, const Game &game);
    // void     from_json(const nlohmann::json &j, Game &game);

    Explosion GenerateExplosion() const;
    bool      ApplyExplosion(const Explosion &explosion);

    // TODO: Simple recomandation system
    // Takes in -> Current board, current players, current hand of each players
    // first check: can we win in the next move? if yes, do it (override another card and make a
    // straight line, column or diagonal / just by placing a card)
    // second check: can the opponent win in the next move? if yes, block it
    // if we can't do either, priortize placing cards that would make another card win in the next
    // if we can't do the above, try prioritizing having as many face cards up as possible (placing
    // cards over the opponent's)
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
    bool       m_ExplosionUsed{false};

    static constexpr std::string JSON_FILE_NAME = "reload.json";

    Board RemadeGameBoard(Board board);

    static void     CheckModifierCard(std::stack<Card> &stack);
    static CardType GetCardType(const Card &card);
};

#endif // GAME_H

#include "../../include/GameModes/Turneu.h"

#include "../../include/GameModes/Turneu.h"

Turneu::Turneu(int boardSize, int scoreToWin, const std::string &nameOne,
               const std::string &nameTwo, const std::array<bool, 3> &options, Mode mode) :
    m_Game(std::make_unique<Game>(boardSize, scoreToWin, nameOne, nameTwo, options)),
    m_Arena(boardSize), m_Mode(mode), m_GamesPlayed(0), m_Player1Wins(0), m_Player2Wins(0) {}

void Turneu::Start() {
    while (m_GamesPlayed < 5 && !CheckMatchWin()) {
        PlayGame();
        m_GamesPlayed++;
    }
}

void Turneu::PlayGame() {
    m_Game->SetNewCards();
    while (m_Game->CheckWinningConditions() == Game::WinningCondition::NoWin) {
        m_Game->SwitchPlayerTurn();
    }

    PlayerTurn winner  = m_Game->GetCurrentPlayer();
    Position   lastPos = (winner == PlayerTurn::Player1) ? m_Game->GetLastCardPlayer1()
                                                         : m_Game->GetLastCardPlayer2();
    PlacePieceInArena(lastPos, winner);

    if (winner == PlayerTurn::Player1) {
        m_Player1Wins++;
    } else {
        m_Player2Wins++;
    }
}

void Turneu::PlacePieceInArena(const Position &pos, PlayerTurn player) {
    PlayerPiece piece =
            (player == PlayerTurn::Player1) ? PlayerPiece::Player1 : PlayerPiece::Player2;
    m_Arena.PlacePiece(pos.second, pos.first, piece);
}

bool Turneu::CheckMatchWin() const {
    return m_Arena.CheckWin(PlayerPiece::Player1) || m_Arena.CheckWin(PlayerPiece::Player2);
}

PlayerTurn Turneu::GetMatchWinner() const {
    if (m_Arena.CheckWin(PlayerPiece::Player1)) {
        return PlayerTurn::Player1;
    } else if (m_Arena.CheckWin(PlayerPiece::Player2)) {
        return PlayerTurn::Player2;
    } else if (m_Player1Wins > m_Player2Wins) {
        return PlayerTurn::Player1;
    } else if (m_Player2Wins > m_Player1Wins) {
        return PlayerTurn::Player2;
    }
}

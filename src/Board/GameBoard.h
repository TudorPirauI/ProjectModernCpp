//
// Created by mavri on 08.11.2024.
//

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <memory>
#include <stack>
#include <vector>
#include<cstdint>

#include "../Card.h"
#include "CardStack.h"

/*
* // class gameBoard {};
/*
        Easy -> gready algorithm
        Medium -> dynamic programming
        Hard -> AI / machine learning
        Impossible -> know what cards you have on hand +
                        AI / machine learning
*/
// this class will be pure virtual, it'll be inherited by the other special
// gamemode classes
// class gameBoard -> things about adding cards, checking if the matrix is full,
// etc...
// class Game {
/*
gameBoard: std::vector<std::vector<std::vector<Card*>>> 3x3 matrix (could also
be 4x4 depending on gamemode) potentially making the last vector
std::stack<Card*>
isLocked: bool -> if we found a 3x3/4x4 matrix
gameMode: Enum with all available gamemodes
playerOne: Player
playerTwo: Player
turn: bool -> true if playerOne's turn, false if playerTwo's turn
isFull(): bool -> returns true if the matrix is full with cards
willBecomeIsolated(effect: Effect, int row/column) TBD, based on special cards
*/

/*
Moduri de joc
a) Modul antrenament:
* tabla este de 3x3;
* fiecare jucător are 7 cărți: 1, 1, 2, 2, 3, 3, 4;
* Format meci: 2/3;

b) Duelul vrăjitorilor:
* tabla este 4x4
* fiecare jucător are 10 cărți: 1, 1, 2, 2, 2, 3, 3, 3, 4, Eter;
* fiecare jucător primește un vrăjitor aleatoriu și distinct;
* Format meci: 3/5

c) Duelul elementelor.
* tabla este 4x4,
* fiecare jucător are 10 cărți: 1, 2, 2, 2, 3, 3, 3, 4, Eter;
* se aleg 2 puteri distincte în mod aleatoriu;
* cele 2 puteri pot fi folosite de amândoi jucătorii;
* Format meci: 3/5
* Obs: Nu uitați că o putere folosită de un jucător nu mai e disponibilă pentru celălalt jucător!
* Obs: Puterile elementelor sunt alese din listă o singură dată pe meci

d) Modul turneu:
* Se alege unul din modurile a, b, c sau b+c.
* Formatul unui meci: 5 jocuri.
* Se folosește o nouă tablă de joc numită “Arena”, de aceeași mărime ca tabla de joc.
* Arena se populează cu piese (markere) aparținând jucătorilor.
* Când un jucător câștigă un joc printr-o linie/coloană/diagonală, poziția ultimei sale cărți jucate indică poziția din
arenă unde se pune o piesă. Un jucător poate să aibă mai multe piese pe aceeași poziție în arenă. Dacă pe poziția
respectivă există cel puțin o piesă a adversarului, atunci doar o piesă a oponentului se înlocuiește cu o piesă a
câștigătorului.
* Dacă la finalul unui joc tabla nu are granițe bine definite, atunci câștigătorul decide granițele pentru
a determina poziția ultimei sale cărți.
* Dacă jocul e câștigat prin numărătoare de puncte, atunci câștigătorul poate
plasa o piesă oriunde în arenă.
* Meciul e câștigat imediat de jucătorul care creează o linie/coloană/diagonală cu 3 din piesele sale.
* Dacă niciun jucător nu reușește acest lucru după 5 jocuri, atunci meciul e câștigat de jucătorul cu cele
mai multe piese din arenă. Altfel e remiză.

e) Modul viteză:
* Se alege unul din modurile a, b, c, b+c sau d
* Fiecare jucător are un cronometru cu 60/90/120 de secunde care măsoară timpul doar pe parcursul turelor jucătorului
respectiv.
* Dacă timpul expiră, acel jucător pierde.
* Obs: timpul este configurat la începutul unui meci
* Cronometrul se resetează la fiecare joc.
 */

using Board = std::vector<std::vector<std::optional<CardStack>>>;

class GameBoard {
    Board m_GameBoard;
    bool  m_IsLocked{false};

public:
    void        growBoardIfNeeded();
    static void placeCard(const Card &card, uint8_t row, uint8_t column);

    [[nodiscard]] bool checkIsolation() const;
};


#endif // GAMEBOARD_H

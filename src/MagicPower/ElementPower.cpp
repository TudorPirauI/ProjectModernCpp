//
// Created by Miruna on 11/24/2024.
//

#include "ElementPower.h"

#include <random>
#include <utility>

ElementPower RandomPower() {
    constexpr int totalPowers = 24;

    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, totalPowers - 1);


    return static_cast<ElementPower>(dis(gen));
}

std::string getElementPowerDescription(ElementPower power) {
    switch (power) {
        case ElementPower::I:
            return "Tabla explodează!";
        case ElementPower::II:
            return "Elimină din joc ultima carte jucată de adversar.";
        case ElementPower::III:
            return "Întoarce iluzia adversarului cu fața în sus. Apoi joacă o carte pe oricare poziție de pe tablă.";
        case ElementPower::IV:
            return "Alege un număr cu condiția că cel puțin 2 cărți cu acel număr sunt vizibile pe tablă. Toate "
                   "cărțile vizibile cu acest număr se întorc în mâinile proprietarilor.";
        case ElementPower::V:
            return "Alege o carte proprie eliminată din joc și joac-o imediat.";
        case ElementPower::VI:
            return "Alege orice carte proprie acoperită de adversar și joac-o imediat pe altă poziție.";
        case ElementPower::VII:
            return "Întoarce o carte vizibilă a oponentului în mâna sa.";
        case ElementPower::VIII:
            return "Toate cărțile acoperite de alte cărți se întorc în mâinile proprietarilor.";
        case ElementPower::IX:
            return "Shiftează un rând complet ocupat cu 1 poziție în direcția dorită. Cărțile din teancul care a ieșit "
                   "din granița tablei se întorc în mâinile proprietarilor.";
        case ElementPower::X:
            return "Mută orizontal sau vertical oricare carte vizibilă de pe tablă pe o poziție adiacentă cu o carte "
                   "cu un număr mai mic.";
        case ElementPower::XI:
            return "Înlocuiește propria iluzie plasată cu o altă iluzie.";
        case ElementPower::XII:
            return "Elimină din joc oricare teanc de cărți ce conține 2 sau mai multe cărți.";
        case ElementPower::XIII:
            return "Interschimbă pozițiile a 2 teancuri de cărți.";
        case ElementPower::XIV:
            return "Joacă încă o iluzie.";
        case ElementPower::XV:
            return "Mută un teanc pe o poziție adiacentă goală. Joacă o carte pe noua poziție goală.";
        case ElementPower::XVI:
            return "Mută 2 cărți de pe același rând, dar despărțite de un spațiu gol, pe acel spațiu gol. Cartea cu "
                   "numărul mai mare se pune deasupra, iar în caz de egalitate, jucătorul alege.";
        case ElementPower::XVII:
            return "Alege un rând. În următoarea tură a adversarului, acesta nu poate plasa cărți pe rândul respectiv.";
        case ElementPower::XVIII:
            return "Alege un rând cu cel puțin 3 poziții ocupate. Teancurile de pe rândul respectiv 'cad' spre o "
                   "margine și formează un nou teanc la marginea respectivă.";
        case ElementPower::XIX:
            return "Valoarea unei cărți proprii 1/2/3 crește cu 1. Dacă acea carte e acoperită sau întoarsă în mână, "
                   "își pierde bonusul.";
        case ElementPower::XX:
            return "Elimină de pe tablă toate cărțile vizibile cu numărul 1.";
        case ElementPower::XXI:
            return "Valoarea unei cărți a adversarului 2/3/4 scade cu 1. Dacă acea carte e acoperită sau întoarsă în "
                   "mână, își pierde penalizarea.";
        case ElementPower::XXII:
            return "Plasează o carte neutră pe tablă astfel încât ea să definească cel puțin o graniță a tablei de "
                   "joc.";
        case ElementPower::XXIII:
            return "Alege 2 teancuri adiacente pe orizontală/verticală. Shiftează cele 2 teancuri pe rândul pe care se "
                   "află cu 1 poziție.";
        case ElementPower::XXIV:
            return "Acoperă orice iluzie cu o carte (din mână), fără a întoarce iluzia cu fața în sus.";
        default:
            return "Putere necunoscută.";
    }
}

//
// Created by Tudor on 11/24/2024.
//

#ifndef WIZARD_H
#define WIZARD_H

#include <cstdlib>
#include <ctime>
#include "MagicPower.h"

class Wizard : public MagicPower {
private:
    enum class WIZARD_TYPE { I, II, III, IV, V, VI, VII, VIII };
    WIZARD_TYPE m_type;
    int         RandomPower();

public:
    Wizard(std::string name, int id);
    Wizard(std::string name, int id, WIZARD_TYPE m_type);
    ~Wizard();
};


#endif // WIZARD_H

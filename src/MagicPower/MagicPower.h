//
// Created by Tudor on 11/24/2024.
//
#include <string>

#ifndef MAGICPOWER_H
#define MAGICPOWER_H


class MagicPower {
private:
    std::string m_name;
    int         m_id;
    bool        m_WasUsed{false};

public:
    MagicPower(std::string name, int id);
    void        setName(std::string name);
    void        setId(int id);
    void        setWasUsed(bool wasUsed);
    std::string getName();
    int         getId();
    bool        getWasUsed();
    ~MagicPower();
};


#endif // MAGICPOWER_H

//
// Created by Tudor on 11/24/2024.
//

#ifndef MAGICPOWER_H
#define MAGICPOWER_H

#include <string>

class MagicPower {
private:
    std::string m_name;
    int         m_id;
    bool        m_WasUsed{false};

public:
    MagicPower(std::string name, int id);
    ~MagicPower();

    void                      setName(const std::string &name);
    void                      setId(int id);
    void                      setWasUsed(bool wasUsed);
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int         getId() const;
    [[nodiscard]] bool        getWasUsed() const;
};


#endif // MAGICPOWER_H

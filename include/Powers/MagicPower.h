//
// Created by Tudor on 11/24/2024.
//

#ifndef MAGICPOWER_H
#define MAGICPOWER_H

#include "pch.h"

class MagicPower {
    std::string m_Name;
    int         m_Id;
    bool        m_WasUsed{false};

public:
    MagicPower(std::string name, int id);
    ~MagicPower();

    void SetName(const std::string &name);
    MagicPower(const MagicPower &other)                              = default;
    MagicPower(MagicPower &&other) noexcept                          = default;
    MagicPower               &operator=(const MagicPower &other)     = default;
    MagicPower               &operator=(MagicPower &&other) noexcept = default;
    void                      SetId(int id);
    void                      SetWasUsed(bool wasUsed);
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] int         GetId() const;
    [[nodiscard]] bool        GetWasUsed() const;
};

#endif // MAGICPOWER_H

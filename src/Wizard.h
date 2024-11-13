//
// Created by Miruna on 11/3/2024.
//

#ifndef WIZARD_H
#define WIZARD_H

#pragma once

/*
clasa virtual vrajitor
  std::string m_nume;
  EPower m_power; // enum cu toate puterile magice (si functie to-string pentru display)
  bool m_wasUsed = false; // nu apare in constructor deloc, se va seta din usePower in false si devine useless

public:
  bool canUsePower(const std::pair<int, int>& locatie) const // se verifica daca ar ramane carti izolate etc.
  void usePower(const std::pair<int, int>& locatie)
  bool wasUsed() const
*/

#include <stdexcept>
#include <string>
#include <utility>

enum class EPower {
    EliminaCarteAcoperita,
    EliminaRandCarti,
    AcoperaCarteOponent,
    CreeazaGroapa,
    MutaTeancPropriu,
    ExtraCarteEter,
    MutaTeancOponent,
    MutaRandMargine
};


class Wizard {
    std::string m_name;
    EPower      m_power;
    bool        m_WasUsed = false;

public:
    virtual ~                         Wizard() = default;
                                      Wizard() = default;
                                      Wizard(std::string name, EPower power);
    virtual void                      setName(std::string name);
    [[nodiscard]] virtual std::string getName() const;
    virtual void                      setPower(EPower power);
    [[nodiscard]] virtual EPower      getPower() const;
    [[nodiscard]] virtual bool        IsUsed() const;

    [[nodiscard]] virtual bool       CanUsePower(const std::pair<int, int> &locatie) const;
    virtual void                     UsePower(const std::pair<int, int> &locatie);
    [[nodiscard]] std::string ToString(const EPower &power) const;

};

class RemoveWizardCard : public Wizard {
public:
    void UsePower(const std::pair<int, int> &locatie) override {
        // TO DO
    }
};

class PowerStrategy {
public:
    virtual ~    PowerStrategy()                              = default;
    virtual void UsePower(const std::pair<int, int> &locatie) = 0;
};
#endif // WIZARD_H

//
// Created by Tudor on 1/17/2025.
//

#ifndef ETER_IDUELULELEMENTELOR_H
#define ETER_IDUELULELEMENTELOR_H

#include "GameComponents/Card.h"
#include "GameModes/DuelulElementelor.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"
#include "SpecialOptions.h"

class ElementWidget;

class IDuelulElementelor final : public QWidget {
    Q_OBJECT

public:
    IDuelulElementelor(const std::string &nameOne, const std::string &nameTwo,
                       const std::array<bool, 3> &options, QWidget *parent);

public slots:
    void OnCardSelected(int cardIndex);
    void OnPositionSelected(int x, int y);
    void OnModifierSelected(int modifier);
    void OnExplosion();
    void OnDialogAccepted(const std::vector<QString> &info);

signals:
    void GameFinished();

private:
    void SwitchTurn();

    DuelulElementelor   m_CurrentGame;
    Explosion           m_CurrentExplosion;
    std::optional<Card> m_SelectedCard;

    bool m_IsIllusionSelected{false};

    bool m_PlayerOneUsedPower{false};
    bool m_PlayerTwoUsedPower{false};

    QWidget        *m_ParentWidget;
    BoardWidget    *m_BoardWidget;
    HandWidget     *m_HandWidget;
    SpecialOptions *m_SpecialOptions;
    ElementWidget  *m_ElementWidget;
};
#endif // ETER_IDUELULELEMENTELOR_H

//
// Created by mavri on 19 Jan 2025.
//

#ifndef IDUELULCOMBINAT_H
#define IDUELULCOMBINAT_H

#include "ElementWidget.h"
#include "GameComponents/Card.h"
#include "GameModes/DuelulCombinat.h"
#include "Interface/BoardWidget.h"
#include "Interface/HandWidget.h"
#include "SpecialOptions.h"
#include "VrajitorWidget.h"

class IDuelulCombinat final : public QWidget {
    Q_OBJECT

public:
    IDuelulCombinat(const std::string &nameOne, const std::string &nameTwo,
                    const std::array<bool, 3> &options, QWidget *parent);

public slots:
    void OnCardSelected(int cardIndex);
    void OnPositionSelected(int x, int y);
    void OnModifierSelected(int modifier);
    void OnExplosion();
    void OnDialogAccepted(const std::vector<QString> &info, bool isWizardPower);

signals:
    void GameFinished();

private:
    void SwitchTurn();

    DuelulCombinat      m_CurrentGame;
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
    VrajitorWidget *m_WizardWidget;
};

#endif // IDUELULCOMBINAT_H

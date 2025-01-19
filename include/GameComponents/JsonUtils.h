//
// Created by mavri on 19 Jan 2025.
//

#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QJsonObject>
#include <stack>

#include "Board.h"
#include "Card.h"
#include "GameModes/Antrenament.h"
#include "GameModes/DuelulCombinat.h"
#include "GameModes/DuelulElementelor.h"
#include "GameModes/DuelulVrajitorilor.h"
#include "GameModes/Game.h"

class JsonUtils {
public:
    static QJsonObject CardToJson(Card &card);
    static QJsonArray  StackToJson(std::stack<Card> &stack);
    static QJsonObject GameBoardToJson(GameBoard &gameBoard);
    static void        SaveToJson(GameBoard &gameBoard, const QString &fileName);
    static QJsonObject PlayerToJson(Player &player);
    static QJsonArray  HandToJson(std::vector<Card> &hand);
    static QJsonObject PairToJson(std::pair<int, int> &pos);

    static QJsonObject         TrainingModeToJson(Antrenament &mode);
    static QJsonObject         WizardDuelToJson(DuelulVrajitorilor &duel);
    static QJsonObject         ElementDuelToJson(DuelulElementelor &duel);
    static QJsonObject         CombinedDuelToJson(DuelulCombinat &duel);
    static ElementPower        JsonToElementPower(const QJsonObject &json);
    static Wizard              JsonToWizard(const QJsonObject &json);
    static DuelulElementelor   JsonToElementDuel(const QJsonObject &json);
    static DuelulVrajitorilor  JsonToWizardDuel(const QJsonObject &json);
    static GameBoard           JsonToGameBoard(const QJsonObject &json);
    static Player              JsonToPlayer(const QJsonObject &json);
    static std::pair<int, int> JsonToPair(const QJsonObject &json);
    static Card                JsonToCard(const QJsonObject &json);
    static std::vector<Card>   JsonToHand(const QJsonArray &jsonArray);
    static Antrenament                JsonToTrainingMode(const QJsonObject &json);
    static QJsonObject         ElementPowersToJson(ElementPower &powers);
    static QJsonObject         WizardPowersToJson(Wizard &powers);
};

#endif // JSONUTILS_H

//
// Created by mavri on 19 Jan 2025.
//

#include "../include/GameComponents/JsonUtils.h"

QJsonObject JsonUtils::CardToJson(Card &card) {
    QJsonObject json;
    json["value"]      = card.GetValue();
    json["isIllusion"] = card.GetIsIllusion();
    json["isGranite"]  = card.GetIsGranite();
    json["modifier"]   = card.GetModifier();
    json["isEter"]     = card.GetIsEter();
    json["isHole"]     = card.GetIsHole();
    json["isFlipped"]  = card.GetIsFlipped();
    json["owner"]      = static_cast<int>(card.GetPlacedBy());
    return json;
}

QJsonArray JsonUtils::StackToJson(std::stack<Card> &stack) {
    QJsonArray       jsonArray;
    std::stack<Card> copy = stack;
    while (!copy.empty()) {
        jsonArray.append(CardToJson(copy.top()));
        copy.pop();
    }
    return jsonArray;
}

QJsonObject JsonUtils::GameBoardToJson(GameBoard &gameBoard) {
    QJsonObject json;
    for (const auto &pair : gameBoard) {
        QString key = QString::number(pair.first.first) + "," + QString::number(pair.first.second);
        auto    st  = pair.second;
        json[key]   = StackToJson(st);
    }
    return json;
}

void JsonUtils::SaveToJson(GameBoard &gameBoard, const QString &fileName) {
    QJsonObject json;
    json["gameBoard"] = GameBoardToJson(gameBoard);

    QJsonDocument doc(json);
    QFile         file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

QJsonObject JsonUtils::PlayerToJson(Player &player) {
    QJsonObject json;
    json["userName"]          = QString::fromStdString(player.GetUserName());
    json["score"]             = player.GetScore();
    json["numberOfIllusion"]  = player.GetIllusion();
    json["hasIllusionInGame"] = player.GetHasIllusionInGame();
    json["hasExplosion"]      = player.GetHasExplosion();
    auto hand                 = player.GetHand();
    json["hand"]              = HandToJson(hand);
    json["removedCards"]      = HandToJson(player.GetRemovedCards());
    return json;
}

QJsonArray JsonUtils::HandToJson(std::vector<Card> &hand) {
    QJsonArray jsonArray;
    for (Card &card : hand) {
        jsonArray.append(CardToJson(card));
    }
    return jsonArray;
}

QJsonObject JsonUtils::PairToJson(std::pair<int, int> &pos) {
    QJsonObject json;
    json["first"]  = pos.first;
    json["second"] = pos.second;

    return json;
}

// New functions for game modes and powers
QJsonObject JsonUtils::TrainingModeToJson(Antrenament &mode) {
    QJsonObject json;
    json["board"]               = GameBoardToJson(mode.GetBoard().GetGameBoard());
    auto player1                = mode.GetPlayer1();
    json["player1"]             = PlayerToJson(player1);
    auto player2                = mode.GetPlayer2();
    json["player2"]             = PlayerToJson(player2);
    json["currentPlayerTurn"]   = static_cast<int>(mode.GetCurrentPlayerTurn());
    json["scorePlayer1"]        = mode.GetPlayer1Score();
    json["scorePlayer2"]        = mode.GetPlayer2Score();
    json["rowPlayer1"]          = mode.GetRowPlayer1();
    json["rowPlayer2"]          = mode.GetRowPlayer2();
    auto posOne                 = mode.GetLastCardPlayer1();
    json["lastPositionPlayer1"] = PairToJson(posOne);
    auto posTwo                 = mode.GetLastCardPlayer2();
    json["lastPositionPlayer2"] = PairToJson(posTwo);
    json["scoreToWin"]          = mode.GetScoreToWin();
    json["illusionEnabled"]     = mode.GetIllusionEnabled();
    json["eterEnabled"]         = mode.GetEterEnabled();
    json["explosionEnabled"]    = mode.ExplosionEnabled();
    return json;
}

QJsonObject JsonUtils::WizardDuelToJson(DuelulVrajitorilor &duel) {
    QJsonObject json;
    json["board"]               = GameBoardToJson(duel.GetBoard().GetGameBoard());
    json["player1"]             = PlayerToJson(duel.GetPlayer1());
    json["player2"]             = PlayerToJson(duel.GetPlayer2());
    json["currentPlayerTurn"]   = static_cast<int>(duel.GetCurrentPlayerTurn());
    json["scorePlayer1"]        = duel.GetPlayer1Score();
    json["scorePlayer2"]        = duel.GetPlayer2Score();
    json["rowPlayer1"]          = duel.GetRowPlayer1();
    json["rowPlayer2"]          = duel.GetRowPlayer2();
    auto cardOne                = duel.GetLastCardPlayer1();
    json["lastPositionPlayer1"] = PairToJson(cardOne);
    auto cardTwo                = duel.GetLastCardPlayer2();
    json["lastPositionPlayer2"] = PairToJson(cardTwo);
    json["scoreToWin"]          = duel.GetScoreToWin();
    json["illusionEnabled"]     = duel.GetIllusionEnabled();
    json["eterEnabled"]         = duel.GetEterEnabled();
    json["explosionEnabled"]    = duel.ExplosionEnabled();
    auto player1Ability1        = duel.GetPlayerAbility1();
    json["playerAbility1"]      = WizardPowersToJson(player1Ability1);
    auto player2Ability2        = duel.GetPlayerAbility2();
    json["playerAbility2"]      = WizardPowersToJson(player2Ability2);
    return json;
}

QJsonObject JsonUtils::ElementDuelToJson(DuelulElementelor &duel) {
    QJsonObject json;
    json["board"]               = GameBoardToJson(duel.GetBoard().GetGameBoard());
    json["player1"]             = PlayerToJson(duel.GetPlayer1());
    json["player2"]             = PlayerToJson(duel.GetPlayer2());
    json["currentPlayerTurn"]   = static_cast<int>(duel.GetCurrentPlayerTurn());
    json["scorePlayer1"]        = duel.GetPlayer1Score();
    json["scorePlayer2"]        = duel.GetPlayer2Score();
    json["rowPlayer1"]          = duel.GetRowPlayer1();
    json["rowPlayer2"]          = duel.GetRowPlayer2();
    auto cardOne                = duel.GetLastCardPlayer1();
    json["lastPositionPlayer1"] = PairToJson(cardOne);
    auto cardTwo                = duel.GetLastCardPlayer2();
    json["lastPositionPlayer2"] = PairToJson(cardTwo);
    json["scoreToWin"]          = duel.GetScoreToWin();
    json["illusionEnabled"]     = duel.GetIllusionEnabled();
    json["eterEnabled"]         = duel.GetEterEnabled();
    json["explosionEnabled"]    = duel.ExplosionEnabled();
    auto player1Ability1        = duel.GetPlayerAbility1();
    json["playerAbility1"]      = ElementPowersToJson(player1Ability1);
    auto player2Ability2        = duel.GetPlayerAbility2();
    json["playerAbility2"]      = ElementPowersToJson(player2Ability2);
    return json;
}

QJsonObject JsonUtils::ElementPowersToJson(ElementPower &powers) {
    QJsonObject json;
    json["power"]               = static_cast<int>(powers.GetPower());
    json["hasUsedPowerInMatch"] = powers.GetUsedPowerInMatch();
    return json;
}

QJsonObject JsonUtils::WizardPowersToJson(Wizard &powers) {
    QJsonObject json;
    json["type"]                = static_cast<int>(powers.GetType());
    json["hasUsedPowerInMatch"] = powers.HasUsedPowerInMatch();
    return json;
}

QJsonObject JsonUtils::CombinedDuelToJson(DuelulCombinat &duel) {
    QJsonObject json;
    // Add properties of CombinedDuel to json
    return json;
}

ElementPower JsonUtils::JsonToElementPower(const QJsonObject &json) {
    ElementPower powers;
    powers.SetPower(static_cast<ElementIndexPower>(json["power"].toInt()));
    powers.SetUsedPowerInMatch(json["hasUsedPowerInMatch"].toBool());
    return powers;
}

Wizard JsonUtils::JsonToWizard(const QJsonObject &json) {
    Wizard powers;
    powers.SetType(static_cast<WizardPower>(json["type"].toInt()));
    powers.SetUsedPowerInMatch(json["hasUsedPowerInMatch"].toBool());
    return powers;
}

DuelulElementelor JsonUtils::JsonToElementDuel(const QJsonObject &json) {
    DuelulElementelor duel{json["player1"].toObject()["userName"].toString().toStdString(),
                           json["player2"].toObject()["userName"].toString().toStdString(),
                           {json["illusionEnabled"].toBool(), json["eterEnabled"].toBool(),
                            json["explosionEnabled"].toBool()}};

    duel.GetBoard().SetGameBoard(JsonToGameBoard(json["board"].toObject()));
    duel.SetPlayer1(JsonToPlayer(json["player1"].toObject()));
    duel.SetPlayer2(JsonToPlayer(json["player2"].toObject()));
    duel.SetCurrentPlayerTurn(static_cast<PlayerTurn>(json["currentPlayerTurn"].toInt()));
    duel.SetPlayer1Score(json["scorePlayer1"].toInt());
    duel.SetPlayer2Score(json["scorePlayer2"].toInt());
    duel.SetRowPlayer1(json["rowPlayer1"].toInt());
    duel.SetRowPlayer2(json["rowPlayer2"].toInt());
    duel.SetLastCardPlayer1(JsonToPair(json["lastPositionPlayer1"].toObject()));
    duel.SetLastCardPlayer2(JsonToPair(json["lastPositionPlayer2"].toObject()));
    duel.SetScoreToWin(json["scoreToWin"].toInt());
    duel.SetIllusionEnabled(json["illusionEnabled"].toBool());
    duel.SetEterEnabled(json["eterEnabled"].toBool());
    duel.SetExplosionEnabled(json["explosionEnabled"].toBool());
    duel.SetPlayerAbility1(JsonToElementPower(json["playerAbility1"].toObject()));
    duel.SetPlayerAbility2(JsonToElementPower(json["playerAbility2"].toObject()));
    return duel;
}

DuelulVrajitorilor JsonUtils::JsonToWizardDuel(const QJsonObject &json) {
    const auto         nameOne = json["player1"].toObject()["userName"].toString().toStdString();
    const auto         nameTwo = json["player2"].toObject()["userName"].toString().toStdString();
    DuelulVrajitorilor duel{nameOne,
                            nameTwo,
                            {json["illusionEnabled"].toBool(), json["eterEnabled"].toBool(),
                             json["explosionEnabled"].toBool()}};

    duel.SetBoard(JsonToGameBoard(json["board"].toObject()));
    duel.SetPlayer1(JsonToPlayer(json["player1"].toObject()));
    duel.SetPlayer2(JsonToPlayer(json["player2"].toObject()));
    duel.SetCurrentPlayerTurn(static_cast<PlayerTurn>(json["currentPlayerTurn"].toInt()));
    duel.SetPlayer1Score(json["scorePlayer1"].toInt());
    duel.SetPlayer2Score(json["scorePlayer2"].toInt());
    duel.SetRowPlayer1(json["rowPlayer1"].toInt());
    duel.SetRowPlayer2(json["rowPlayer2"].toInt());
    duel.SetLastCardPlayer1(JsonToPair(json["lastPositionPlayer1"].toObject()));
    duel.SetLastCardPlayer2(JsonToPair(json["lastPositionPlayer2"].toObject()));
    duel.SetScoreToWin(json["scoreToWin"].toInt());
    duel.SetIllusionEnabled(json["illusionEnabled"].toBool());
    duel.SetEterEnabled(json["eterEnabled"].toBool());
    duel.SetExplosionEnabled(json["explosionEnabled"].toBool());
    duel.SetPlayerAbility1(JsonToWizard(json["playerAbility1"].toObject()));
    duel.SetPlayerAbility2(JsonToWizard(json["playerAbility2"].toObject()));
    return duel;
}

GameBoard JsonUtils::JsonToGameBoard(const QJsonObject &json) {
    GameBoard gameBoard;
    for (auto it = json.begin(); it != json.end(); ++it) {
        QStringList      coordinates = it.key().split(",");
        int              x           = coordinates[0].toInt();
        int              y           = coordinates[1].toInt();
        QJsonArray       stackArray  = it.value().toArray();
        std::stack<Card> stack;
        for (const QJsonValue &value : stackArray) {
            stack.push(JsonToCard(value.toObject()));
        }
        gameBoard[std::make_pair(x, y)] = stack;
    }
    return gameBoard;
}

Player JsonUtils::JsonToPlayer(const QJsonObject &json) {
    Player player;
    player.SetUserName(json["userName"].toString().toStdString());
    player.SetScore(json["score"].toInt());
    player.SetIllusion(json["numberOfIllusion"].toInt());
    player.SetHasIllusionInGame(json["hasIllusionInGame"].toBool());
    player.SetHasExplosion(json["hasExplosion"].toBool());
    player.SetHand(JsonToHand(json["hand"].toArray()));
    player.SetRemovedCards(JsonToHand(json["removedCards"].toArray()));
    return player;
}

std::pair<int, int> JsonUtils::JsonToPair(const QJsonObject &json) {
    return std::make_pair(json["first"].toInt(), json["second"].toInt());
}

Card JsonUtils::JsonToCard(const QJsonObject &json) {
    Card card;
    card.SetValue(json["value"].toInt());
    card.SetIllusion(json["isIllusion"].toBool());
    card.SetEter(json["isEter"].toBool());
    card.SetIsHole(json["isHole"].toBool());
    card.SetIsFlipped(json["isFlipped"].toBool());
    card.SetIsGranite(json["isGranite"].toBool());
    card.SetModifier(json["modifier"].toInt());
    card.SetPlacedBy(static_cast<PlayerTurn>(json["owner"].toInt()));
    return card;
}

std::vector<Card> JsonUtils::JsonToHand(const QJsonArray &jsonArray) {
    std::vector<Card> hand;
    for (const QJsonValue &value : jsonArray) {
        hand.push_back(JsonToCard(value.toObject()));
    }
    return hand;
}

Antrenament JsonUtils::JsonToTrainingMode(const QJsonObject &json) {
    Antrenament mode{json["player1"].toObject()["userName"].toString().toStdString(),
                     json["player2"].toObject()["userName"].toString().toStdString(),
                     {json["illusionEnabled"].toBool(), json["eterEnabled"].toBool(),
                      json["explosionEnabled"].toBool()}};

    mode.GetBoard().SetGameBoard(JsonToGameBoard(json["board"].toObject()));
    mode.SetPlayer1(JsonToPlayer(json["player1"].toObject()));
    mode.SetPlayer2(JsonToPlayer(json["player2"].toObject()));
    mode.SetCurrentPlayerTurn(static_cast<PlayerTurn>(json["currentPlayerTurn"].toInt()));
    mode.SetPlayer1Score(json["scorePlayer1"].toInt());
    mode.SetPlayer2Score(json["scorePlayer2"].toInt());
    mode.SetRowPlayer1(json["rowPlayer1"].toInt());
    mode.SetRowPlayer2(json["rowPlayer2"].toInt());
    mode.SetLastCardPlayer1(JsonToPair(json["lastPositionPlayer1"].toObject()));
    mode.SetLastCardPlayer2(JsonToPair(json["lastPositionPlayer2"].toObject()));
    mode.SetScoreToWin(json["scoreToWin"].toInt());
    mode.SetIllusionEnabled(json["illusionEnabled"].toBool());
    mode.SetEterEnabled(json["eterEnabled"].toBool());
    mode.SetExplosionEnabled(json["explosionEnabled"].toBool());
    return mode;
}

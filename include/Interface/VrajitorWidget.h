//
// Created by mavri on 19 Jan 2025.
//

#ifndef VRAJITORWIDGET_H
#define VRAJITORWIDGET_H
#include "Powers/Wizard.h"

class VrajitorWidget final : public QPushButton {
    Q_OBJECT

public:
    explicit VrajitorWidget(const Wizard &power, QWidget *parent = nullptr);

    void SetPower(const Wizard &power);
};

#endif // VRAJITORWIDGET_H

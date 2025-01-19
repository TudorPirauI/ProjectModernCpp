//
// Created by mavri on 19 Jan 2025.
//

#include "Interface/ElementWidget.h"

#include "Interface/ElementDialog.h"

ElementWidget::ElementWidget(const ElementPower &power, QWidget *parent) :
    QPushButton(QString::fromStdString(ElementPower::GetElementPowerDescription(power.GetPower())),
                parent),
    m_Power(power) {
    SetPower(power, true);
}

void ElementWidget::SetPower(const ElementPower &power, const bool enabled = true) {
    setText("Use Power");
    setToolTip(QString::fromStdString(ElementPower::GetElementPowerDescription(power.GetPower())));
    if (!enabled) {
        setEnabled(false);
    }
}

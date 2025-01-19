//
// Created by mavri on 19 Jan 2025.
//

#include "Interface/VrajitorWidget.h"
VrajitorWidget::VrajitorWidget(const Wizard &power, QWidget *parent) : QPushButton{parent} {
    SetPower(power);
}

void VrajitorWidget::SetPower(const Wizard &power) {
    setText("Use Wizard Power");
    setDisabled(power.HasUsedPowerInMatch());
    setToolTip(QString::fromStdString(power.GetWizardPowerDescription()));
}

//
// Created by mavri on 19 Jan 2025.
//

#include "Interface/ElementWidget.h"

#include "Interface/ElementDialog.h"

ElementWidget::ElementWidget(const ElementPower &power, QWidget *parent) :
    QPushButton(QString::fromStdString(ElementPower::GetElementPowerDescription(power.GetPower())),
                parent),
    m_Power(power) {
    SetPower(power);

    connect(this, &ElementWidget::clicked, this, [this] {
        ElementDialog dialog(m_Power, this);
        if (dialog.exec() == QDialog::Accepted) {
            // emit ElementClicked();
            std::cout << "Dialog accepted\n";
        }
    });
}

void ElementWidget::SetPower(const ElementPower &power) {
    setText("Use Power");
    setToolTip(QString::fromStdString(ElementPower::GetElementPowerDescription(power.GetPower())));
}

//
// Created by mavri on 19 Jan 2025.
//

#include "Interface/ElementDialog.h"

ElementDialog::ElementDialog(const ElementPower &power, QWidget *parent) :
    QDialog(parent), m_Power(power) {
    setMinimumSize(200, 150);

    m_FormLayout = new QFormLayout(this);
    m_OkButton   = new QPushButton("OK", this);

    auto *descriptionLabel = new QLabel(
            QString::fromStdString(ElementPower::GetElementPowerDescription(power.GetPower())),
            this);
    m_FormLayout->addRow(descriptionLabel);

    auto *infoLabel = new QLabel("Required Information:", this);
    m_FormLayout->addRow(infoLabel);

    const auto requiredInfo = ElementPower::GetRequiredInfo(power.GetPower());
    for (const auto &info : requiredInfo) {
        QWidget *input = nullptr;
        switch (info) {
            case ElementPowerInfo::PositionOne:
            case ElementPowerInfo::PositionTwo:
                input = new QLineEdit(this);
                m_FormLayout->addRow("Position:", input);
                break;
            case ElementPowerInfo::Number:
                input = new QLineEdit(this);
                m_FormLayout->addRow("Number:", input);
                break;
            case ElementPowerInfo::Card:
                input = new QLineEdit(this);
                m_FormLayout->addRow("Card:", input);
                break;
        }
        if (input) {
            m_Inputs.push_back(input);
        }
    }

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_OkButton);
    buttonLayout->addStretch();

    m_FormLayout->addRow(buttonLayout);
    connect(m_OkButton, &QPushButton::clicked, this, &ElementDialog::onOkClicked);
}

void ElementDialog::onOkClicked() {
    // Process the inputs if needed
    accept();
}

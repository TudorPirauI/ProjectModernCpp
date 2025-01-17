//
// Created by edi on 17.01.2025.
//

#include "../../include/Interface/SpecialOptions.h"

SpecialOptions::SpecialOptions(QWidget *parent) : QWidget(parent) {}

void SpecialOptions::SetPowers(const bool eter, const bool illusion, const bool explosion) {
    if (layout() == nullptr) {
        const auto layout = new QHBoxLayout(this);
        setLayout(layout);
    } else {
        QLayoutItem *item;
        while ((item = layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    m_Buttons.clear();

    for (int i = 0; i < 3; ++i) {
        auto button = new QPushButton(this);
        button->setCheckable(false);
        button->setFixedSize(100, 100);

        if (i == 0) {
            if (eter) {
                button->setCheckable(eter);
            } else {
                button->setChecked(false);
            }
        }

        if (i == 1) {
            if (illusion) {
                button->setCheckable(illusion);
            } else {
                button->setChecked(false);
            }
        }

        if (i == 2) {
            if (explosion) {
                button->setCheckable(explosion);
            } else {
                button->setChecked(false);
            }
        }

        connect(button, &QPushButton::clicked, [this, i, button] {
            emit OptionSelected(i);
        });

        m_Buttons.push_back(button);
        layout()->addWidget(button);
    }
}

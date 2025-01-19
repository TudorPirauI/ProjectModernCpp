//
// Created by mavri on 19 Jan 2025.
//

#ifndef ELEMENTWIDGET_H
#define ELEMENTWIDGET_H

#include "Powers/ElementPower.h"

class ElementWidget final : public QPushButton {
    Q_OBJECT

public:
    explicit ElementWidget(const ElementPower &power, QWidget *parent = nullptr);

    void SetPower(const ElementPower &power, bool enabled);

signals:
    void ElementClicked();

private:
    ElementPower m_Power;
};

#endif // ELEMENTWIDGET_H

//
// Created by mavri on 19 Jan 2025.
//

#ifndef ELEMENTDIALOG_H
#define ELEMENTDIALOG_H

#include "Powers/ElementPower.h"
#include "pch.h"

class ElementDialog : public QDialog {
    Q_OBJECT

public:
    ElementDialog(const ElementPower &power, QWidget *parent = nullptr);

private slots:
    void onOkClicked();

private:
    ElementPower           m_Power;
    QFormLayout           *m_FormLayout;
    QPushButton           *m_OkButton;
    std::vector<QWidget *> m_Inputs;
};

#endif // ELEMENTDIALOG_H

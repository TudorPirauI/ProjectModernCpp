//
// Created by mavri on 19 Jan 2025.
//

#ifndef VRAJITORDIALOG_H
#define VRAJITORDIALOG_H

#include "Powers/Wizard.h"

class VrajitorDialog final : public QDialog {
    Q_OBJECT

public:
    explicit VrajitorDialog(const Wizard &power, QWidget *parent = nullptr);

signals:
    void DialogAccepted(const std::vector<QString> &info, bool isWizardPower);

private slots:
    void onOkClicked();

private:
    Wizard                 m_Power;
    QFormLayout           *m_FormLayout;
    QPushButton           *m_OkButton;
    std::vector<QWidget *> m_Inputs;
};

#endif // VRAJITORDIALOG_H

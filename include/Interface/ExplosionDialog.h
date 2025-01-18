//
// Created by mavri on 18 Jan 2025.
//

#ifndef EXPLOSIONDIALOG_H
#define EXPLOSIONDIALOG_H

#include "Powers/Explosion.h"

class ExplosionDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ExplosionDialog(const Explosion &explosion, QWidget *parent = nullptr);

signals:
    void ExplosionTriggered();

private slots:
    void OnTriggerButtonClicked();
    void OnCancelButtonClicked();

private:
    Explosion    m_Explosion;
    QLabel      *m_ExplosionLabel;
    QPushButton *m_TriggerButton;
    QPushButton *m_CancelButton;
};

#endif // EXPLOSIONDIALOG_H

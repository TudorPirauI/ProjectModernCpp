//
// Created by mavri on 18 Jan 2025.
//

#include "../../include/Interface/ExplosionDialog.h"

#include "Interface/IAntrenament.h"

ExplosionDialog::ExplosionDialog(const Explosion &explosion, QWidget *parent) :
    QDialog(parent), m_Explosion(explosion) {
    setWindowTitle("Explosion Card");

    m_ExplosionLabel = new QLabel("Explosion Card Effects:", this);
    QString effectsText;
    for (const auto &[fst, snd] : explosion.GetEffects()) {
        QString effectName = "UnkN";

        switch (snd) {
            case Explosion::Effect::Nothing:
                effectName = "Nothing";
                break;
            case Explosion::Effect::Eliminate:
                effectName = "Eliminate";
                break;
            case Explosion::Effect::Return:
                effectName = "Return";
                break;
            case Explosion::Effect::Hole:
                effectName = "Hole";
                break;
        }

        effectsText += QString("Position: (%1, %2), Effect: %3\n")
                               .arg(fst.first)
                               .arg(fst.second)
                               .arg(effectName);
    }
    m_ExplosionLabel->setText(effectsText);

    m_TriggerButton = new QPushButton("Trigger", this);
    m_CancelButton  = new QPushButton("Cancel", this);

    connect(m_TriggerButton, &QPushButton::clicked, this, &ExplosionDialog::OnTriggerButtonClicked);
    connect(m_CancelButton, &QPushButton::clicked, this, &ExplosionDialog::OnCancelButtonClicked);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_ExplosionLabel);
    layout->addWidget(m_TriggerButton);
    layout->addWidget(m_CancelButton);

    setLayout(layout);
}

void ExplosionDialog::OnTriggerButtonClicked() {
    emit ExplosionTriggered();
    accept();
}

void ExplosionDialog::OnCancelButtonClicked() {
    const auto result = QMessageBox::question(this, "Cancel Explosion",
                                              "Are you sure you want to cancel the explosion? Your "
                                              "opponent may be able to take advantage of this.",
                                              QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        reject();
    }
}

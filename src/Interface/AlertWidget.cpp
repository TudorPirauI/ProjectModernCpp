//
// Created by mavri on 16 Jan 2025.
//

#include "../../include/Interface/AlertWidget.h"

AlertWidget::AlertWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    const auto layout = new QVBoxLayout(this);
    m_MessageLabel    = new QLabel(this);
    m_MessageLabel->setAlignment(Qt::AlignCenter);
    m_MessageLabel->setStyleSheet(
            "background-color: white; padding: 10px; border-radius: 5px; font-size: 18px; "
            "font-weight: bold; color: #333;");

    layout->addWidget(m_MessageLabel);
    setLayout(layout);

    m_CloseTimer = new QTimer(this);
    connect(m_CloseTimer, &QTimer::timeout, this, &AlertWidget::close);
}
void AlertWidget::ShowAlert(const QString &message) {
    m_MessageLabel->setText(message);
    m_CloseTimer->start(1500);

    if (parentWidget()) {
        parentWidget()->setEnabled(false); // Disable parent widget
        auto blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurRadius(10);
        parentWidget()->setGraphicsEffect(blurEffect);
    }

    connect(m_CloseTimer, &QTimer::timeout, this, [this] {
        if (parentWidget()) {
            parentWidget()->setGraphicsEffect(nullptr);
            parentWidget()->setEnabled(true); // Re-enable parent widget
        }
        close();
    });

    show();
    raise();
    activateWindow();
}

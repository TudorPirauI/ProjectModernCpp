//
// Created by mavri on 16 Jan 2025.
//

#include "../../include/Interface/AlertWidget.h"

AlertWidget::AlertWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    auto layout    = new QVBoxLayout(this);
    m_MessageLabel = new QLabel(this);
    m_MessageLabel->setAlignment(Qt::AlignCenter); // Center the text within the label
    m_MessageLabel->setStyleSheet(
            "background-color: white; padding: 10px; border-radius: 5px; font-size: 18px; "
            "font-weight: bold; color: #333;"); // Add styling

    layout->addWidget(m_MessageLabel);
    setLayout(layout);

    m_CloseTimer = new QTimer(this);
    connect(m_CloseTimer, &QTimer::timeout, this, &AlertWidget::close);
}

void AlertWidget::ShowAlert(const QString &message) {
    m_MessageLabel->setText(message);
    m_CloseTimer->start(3000);

    // Apply blur effect to the parent widget
    if (parentWidget()) {
        auto blurEffect = new QGraphicsBlurEffect(this);
        blurEffect->setBlurRadius(10);
        parentWidget()->setGraphicsEffect(blurEffect);
    }

    // Start the timer and connect its timeout signal to a lambda function
    m_CloseTimer->start(3000);
    connect(m_CloseTimer, &QTimer::timeout, this, [this]() {
        if (parentWidget()) {
            parentWidget()->setGraphicsEffect(nullptr); // Remove blur effect
        }
        close();
    });

    show();
    raise();
    activateWindow();
}

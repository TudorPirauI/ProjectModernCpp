//
// Created by mavri on 16 Jan 2025.
//

#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

class AlertWidget final : public QWidget {
    Q_OBJECT

public:
    explicit AlertWidget(QWidget *parent = nullptr);
    void ShowAlert(const QString &message);

private:
    QLabel *m_MessageLabel;
    QTimer *m_CloseTimer;
};

#endif // ALERTWIDGET_H

//
// Created by edi on 17.01.2025.
//

#ifndef SPECIALOPTIONS_H
#define SPECIALOPTIONS_H

class SpecialOptions final : public QWidget {
    Q_OBJECT

    std::vector<QPushButton *> m_Buttons;
    static constexpr int       WIDTH   = 100;
    static constexpr int       HEIGHT  = 150;
    static constexpr int       SPACING = 10;

public:
    explicit SpecialOptions(QWidget *parent = nullptr);
    void SetPowers(bool eter, bool illusion, bool explosion);
signals:
    void OptionSelected(int value);
};

#endif // SPECIALOPTIONS_H

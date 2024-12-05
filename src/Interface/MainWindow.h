//
// Created by mavri on 05.12.2024.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../GameBoard/Board.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void DrawMenu();

private:
    GameBoard *m_GameBoard;
};

#endif // MAINWINDOW_H

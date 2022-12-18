#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class HumanState : int {
    LOSE =  1,
    TIE  =  0,
    WIN  = -1
};

namespace Player {
    enum Player  {
        AI    = 'O',
        HUMAN = 'X',
        NONE  = ' '
    };
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void playTheBest();
    ~MainWindow();

private slots:
    void on_actionNew_Game_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<char>> board;
    bool isHumanTurn = true;
    std::vector<std::vector<QPushButton*>> btns;

    void openMsgBox(QString msg);
    int miniMax(std::vector<std::vector<char>> gameBoard, int depth, bool isAi) const;
    HumanState calculateScore(std::vector<std::vector<char>> gameBoard) const;
    bool isGameOver(std::vector<std::vector<char>> gameBoard) const;
    bool checkWin(Player::Player player, std::vector<std::vector<char>> gameBoard) const;
};
#endif // MAINWINDOW_H

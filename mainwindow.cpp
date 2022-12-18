#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , board(3, std::vector<char>(3, Player::NONE))
    , btns(3, std::vector<QPushButton*>(3))
{
    ui->setupUi(this);
    for (int x = 0; x < 3; ++x)
        for(int y = 0; y < 3; ++y) {
            QPushButton* btn = new QPushButton(" ");
            btn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,
                                           QSizePolicy::Minimum));
            btn->setCheckable(true);
            ui->gridLayout_2->addWidget(btn, x, y);
            btns.at(x).at(y) = btn;
            connect(btn, &QPushButton::clicked, this, [=] (bool clicked) {
                if (isHumanTurn) {
                    if (clicked) {
                        btn->setText((QString)Player::HUMAN);
                        board.at(x).at(y) = Player::HUMAN;
                        if (checkWin(Player::HUMAN, board)) {
                            openMsgBox("U wiiiiin!");
                        } else {
                            isHumanTurn = false;
                            playTheBest();
                        }
                    } else {
                        btn->setChecked(true);
                    }
                } else {
                    btn->setChecked(!clicked);
                }
            });
        }
}

void MainWindow::playTheBest()
{
    int topScore  = 0,
        bestScore = INT_MIN,
        bestRow = 0,
        bestColumn = 0;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board.at(i).at(j) == Player::NONE) {
                board.at(i).at(j) = Player::AI;
                topScore = miniMax(board, 0, false);
                board.at(i).at(j) = Player::NONE;

                if(bestScore < topScore){
                    bestScore = topScore;
                    bestRow = i;
                    bestColumn = j;
                }
            }

    board.at(bestRow).at(bestColumn) = Player::AI;
    btns.at(bestRow).at(bestColumn)->setText((QString)Player::AI);
    btns.at(bestRow).at(bestColumn)->setChecked(true);

    if (checkWin(Player::AI, board))
        openMsgBox("U loseeee!");
    else
        isHumanTurn = true;
}

int MainWindow::miniMax(std::vector<std::vector<char>> gameBoard,
                        int depth,
                        bool isAi) const
{
    int topScore  {0};
    int bestScore {0};
    if (isGameOver(gameBoard))
        return (int) calculateScore(gameBoard);

    bestScore = (isAi) ? INT_MIN : INT_MAX;
    for(auto &row: gameBoard)
        for(char &item: row)
            if (item == Player::NONE) {
                item = (isAi) ? Player::AI : Player::HUMAN;
                topScore = miniMax(gameBoard, depth + 1, !isAi);
                item = Player::NONE;
                bestScore = (isAi)                        ?
                            std::max(topScore, bestScore) :
                            std::min(topScore, bestScore) ;
            }
    return bestScore;
}

HumanState MainWindow::calculateScore(std::vector<std::vector<char>> gameBoard) const
{
    return (checkWin(Player::HUMAN, gameBoard)) ?
           HumanState::WIN                      :
           (checkWin(Player::AI, gameBoard))    ?
           HumanState::LOSE                     :
           HumanState::TIE                      ;
}

bool MainWindow::isGameOver(std::vector<std::vector<char>> gameBoard) const
{
    if(checkWin(Player::HUMAN, gameBoard) || checkWin(Player::AI, gameBoard))
        return true;                  // Win or Lose

    for(const auto &row: gameBoard)
        for(const char &item: row)
            if (item == Player::NONE)
                return false;         // Not over yet

    return true;                      // Tie
}

bool MainWindow::checkWin(Player::Player player,
                          std::vector<std::vector<char>> gameBoard) const
{
    for (int i = 0; i < 3; ++i) {
        if (gameBoard[i][0] == player &&
            gameBoard[i][1] == player &&
            gameBoard[i][2] == player)
            return true;

        if (gameBoard[0][i] == player &&
            gameBoard[1][i] == player &&
            gameBoard[2][i] == player)
            return true;
    }

    if (gameBoard[0][0] == player &&
        gameBoard[1][1] == player &&
        gameBoard[2][2] == player)
        return true;
    else if (gameBoard[0][2] == player &&
             gameBoard[1][1] == player &&
             gameBoard[2][0] == player)
        return true;

    return false;
}

void MainWindow::openMsgBox(QString msg) {
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setText(msg);
    msgBox->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_Game_triggered()
{
    isHumanTurn = true;
    board = std::vector<std::vector<char>>(3, std::vector<char>(3, Player::NONE));
    for (auto &btnsRow: btns)
        for (auto &btn: btnsRow) {
            btn->setText(" ");
            btn->setCheckable(true);
            btn->setChecked(false);
        }
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


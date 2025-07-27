#ifndef DESKTOP_H
#define DESKTOP_H



#include <QMainWindow>
#include <QPushButton>


#include "desktop_snake.h"
#include "desktop_tetris.h"
#include "../../../inc/snake/snake.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class desktop;
}
QT_END_NAMESPACE


class Desktop : public QMainWindow{
    Q_OBJECT

public:
    explicit Desktop(s21::SnakeController &controller, QWidget *parent = nullptr);

private slots:
    void ChooseGame();

private:
    Ui::desktop *ui;

    s21::SnakeController &controller;
    s21::SnakeQT *snake;
    s21::TetrisQT *tetris;


    QPushButton *tetris_button;
    QPushButton *snake_button;
    QPushButton *exit_button;
};



#endif // DESKTOP_H

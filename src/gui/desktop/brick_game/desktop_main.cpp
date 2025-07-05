#include "desktop_main.h"
#include "ui_mainwindow.h"



Desktop::Desktop(s21::SnakeController &controller, QWidget *parent) : QMainWindow(parent), ui(new Ui::desktop), controller(controller), snake(new s21::SnakeQT(controller)), tetris(new s21::TetrisQT){
    ui->setupUi(this);

    tetris->setWindowTitle("TeTrIs");
    snake->setWindowTitle("SnAkE");

    connect(ui->snake_button, &QPushButton::clicked, this, &Desktop::ChooseGame);
    connect(ui->tetris_button, &QPushButton::clicked, this, &Desktop::ChooseGame);
    connect(ui->exit_button, &QPushButton::clicked, this, &Desktop::ChooseGame);

    connect(snake, &s21::SnakeQT::gameClosed, this, &Desktop::show);
    connect(tetris, &s21::TetrisQT::gameClosed, this, &Desktop::show);
}

void Desktop::ChooseGame() {
    //QPushButton *button = qobject_cast<QPushButton *>(sender());
    QObject *senderObject = sender();

    if(senderObject == ui->snake_button){
        hide();
        snake->show();
    }else if(senderObject == ui->tetris_button){
        hide();
        tetris->show();
    }else if(senderObject == ui->exit_button){
        QApplication::quit();
    }
}

#ifndef DESKTOP_SNAKE_H
#define DESKTOP_SNAKE_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTimer>


#include "../../../inc/snake/snake.h"
#include "../../../inc/snake/snake_controller.h"
#include "../../../inc/defines.h"


enum SnakeColor {
    BODY = 1,
    HEAD = 2,
    APPLE = 3,
    EMPTY = 0

};

namespace s21 {

class SnakeQT   : public QWidget {
    Q_OBJECT

signals:
    void gameClosed();

public:
    explicit SnakeQT(SnakeController &controller, QWidget *parent = nullptr);
    ~SnakeQT();

protected:
    void paintEvent(QPaintEvent *event) override;
    void DrawInfoBar(QPainter &painter);
    void DrawField(QPainter &painter);
    void DrawSnake(QPainter &painter);
    void DrawApple(QPainter &painter);
    void PrintMasseges(QPainter &painter);
    QColor GetColor(SnakeColor color);

    void UpdateGame();
    void keyPressEvent(QKeyEvent *event) override;
    void CloseEvent(QCloseEvent *event);
    void ResetGame();

private:
    SnakeController &controller;
    QTimer *gametimer;

};

}

#endif // DESKTOP_SNAKE_H

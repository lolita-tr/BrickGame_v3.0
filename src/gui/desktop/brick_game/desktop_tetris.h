#ifndef DESKTOP_TETRIS_H
#define DESKTOP_TETRIS_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QTimer>



#include "../../../inc/tetris/tetris.h"
#include "../../../inc/tetris/figures.h"
#include "../../../inc/defines.h"
#include "../../../inc/tetris/fsm.h"


namespace s21 {
class TetrisQT : public QWidget{
    Q_OBJECT

public: explicit TetrisQT(QWidget *parent = nullptr);

signals:
    void gameClosed();

protected:
    void paintEvent(QPaintEvent *event) override;

    void DrawInfoBar(QPainter &painter, const GameInfo *game_info);
    void DrawFieldBorder(QPainter &painter);
    void DrawPlayField(QPainter &painter, const GameInfo *game_info);

    void DrawTetromino(QPainter &painter, const Tetromino *tet);
    void DrawNextTetromino(QPainter &painter, const GameInfo *game_info, int infoX, int infoY);

    void keyPressEvent(QKeyEvent *event) override;
    void UpdateGameTetris();
    void CloseEvent(QCloseEvent *event);
    void PrintMasseges(QPainter &painter);
    void ResetGame();

private:
    GameInfo *game_tetris;
    Tetromino *tetromino;
    QTimer *gametimer;
};

}


#endif // DESKTOP_SNAKE_H

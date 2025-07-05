#include "desktop_tetris.h"


namespace s21 {
TetrisQT::TetrisQT(QWidget *parent) : QWidget(parent), gametimer(nullptr){

    game_tetris = get_game_info();
    tetromino = set_tetromino(game_tetris);

    setFixedSize(400, 420);
    gametimer = new QTimer(this);
    connect(gametimer, &QTimer::timeout, this, &TetrisQT::UpdateGameTetris);
    gametimer->start(300);
}

void TetrisQT::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    QWidget::paintEvent(event);

    DrawInfoBar(painter, game_tetris);
    DrawFieldBorder(painter);
    if(game_tetris->pause == STARTED){
    DrawNextTetromino(painter, game_tetris, 245, 315);
    DrawPlayField(painter, game_tetris);
    DrawTetromino(painter, tetromino);}
    PrintMasseges(painter);

}

void TetrisQT::DrawInfoBar(QPainter &painter, const Game_Info *game_info) {
    QPen pen(Qt::black);
    painter.setPen(pen);


    painter.drawRect(230, 10, 100, 90);
    painter.drawRect(230, 110, 100, 90);
    painter.drawRect(230, 210, 100, 90);
    painter.drawRect(230, 310, 100, 100);
    painter.drawText(265, 30, "Level:");
    painter.drawText(275, 70, QString::number(game_info->level));
    painter.drawText(260, 130,"Score:");
    painter.drawText(275, 170, QString::number(game_info->score));
    painter.drawText(245, 230,"High Score:");
    painter.drawText(275, 270, QString::number(game_info->high_score));
}

void TetrisQT::DrawPlayField(QPainter &painter, const Game_Info *game_info){
    painter.save();

    painter.setBrush(QBrush(Qt::red));
    painter.setPen(Qt::NoPen);

    for(int y = 0; y < FIELD_H; y++){
        for(int x = 0; x < FIELD_W; x++){
            if(game_info->field[y][x]){
                painter.drawRect(x*CELL_SIZE + SHIFT_X, y*CELL_SIZE + SHIFT_Y, CELL_SIZE, CELL_SIZE);
            }
        }
    }
    painter.restore();
}

void TetrisQT::DrawFieldBorder(QPainter &painter) {
    QPen pen(Qt::black);
    painter.setPen(pen);
    painter.drawRect(SHIFT_X, SHIFT_Y, FIELD_W * CELL_SIZE, FIELD_H * CELL_SIZE);
}

void TetrisQT::DrawTetromino(QPainter &painter, const Tetromino *tet){
    painter.setBrush(QBrush(Qt::red));
    painter.setPen(Qt::NoPen);

    for (int y = 0; y < MAX_FIGURE_SIZE; ++y) {
        for (int x = 0; x < MAX_FIGURE_SIZE; ++x) {
            if (tet->figure[y][x]) {
                int drawX = (tet->coord.x + x) * CELL_SIZE;
                int drawY = (tet->coord.y + y) * CELL_SIZE;
                painter.drawRect(drawX + SHIFT_X, drawY + SHIFT_Y, CELL_SIZE, CELL_SIZE);
            }
        }
    }

}

void TetrisQT::DrawNextTetromino(QPainter &painter, const Game_Info *game_info, int infoX, int infoY){
    painter.setBrush(QBrush(Qt::blue));
    painter.setPen(Qt::NoPen);

    for (int y = 0; y < MAX_FIGURE_SIZE; ++y) {
        for (int x = 0; x < MAX_FIGURE_SIZE; ++x) {
            if (game_info->next[y][x]) {
                int drawX = infoX + x * CELL_SIZE;
                int drawY = infoY + y * CELL_SIZE;
                painter.drawRect(drawX, drawY, CELL_SIZE, CELL_SIZE);
            }
        }
    }

}

void TetrisQT::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Return:
        get_signal(tetromino, game_tetris, Start);
        break;
    case Qt::Key_Left:
        get_signal(tetromino, game_tetris, Left);
        break;
    case Qt::Key_Right:
        get_signal(tetromino, game_tetris, Right);
        break;
    case Qt::Key_Down:
        get_signal(tetromino, game_tetris, Down);
        break;
    case Qt::Key_Space:
        get_signal(tetromino, game_tetris, Action);
        break;
    case Qt::Key_Escape:
        get_signal(tetromino, game_tetris, Terminate);
        gameClosed();
        close();
        break;
    case Qt::Key_P:
        get_signal(tetromino, game_tetris, Pause);
        break;
    default:
        break;
    }
}

void TetrisQT::UpdateGameTetris(){

    if(game_tetris->pause == STARTED){
    move_tetromino_down_one_row(tetromino, game_tetris);
    if(tetromino->is_placed){
        game_update(tetromino, game_tetris);
    }
    update();
    } else if(game_tetris->pause == LOSED){
        QTimer::singleShot(2000, this, &TetrisQT::ResetGame);
        update();
    }
}

void TetrisQT::PrintMasseges(QPainter &painter){
    QPen pen(Qt::black);
    painter.setPen(pen);

    if(game_tetris->pause == NOT_STARTED){
        painter.drawText(65,200, "Press Enter to Start");
    }else if(game_tetris->pause == LOSED){
        painter.drawText(65, 200, "Bye baby)");
    }else if(game_tetris->pause == PAUSED){
       painter.drawText(100, 200, "Pause");
    }else if(game_tetris->pause == QUIT){
        painter.drawText(65,200, "Press Enter to Continue");
    }
}

void TetrisQT::CloseEvent(QCloseEvent *event){
    emit gameClosed();
    QWidget::closeEvent(event);
}

void TetrisQT::ResetGame(){
    free_tetromino(tetromino);
    free_game(game_tetris);

    game_tetris = get_game_info();
    tetromino = set_tetromino(game_tetris);
    update();
}


}//namespace s21

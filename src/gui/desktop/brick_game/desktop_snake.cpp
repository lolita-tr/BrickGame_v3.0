#include "desktop_snake.h"
//#include "ui_mainwindow.h"


namespace s21 {

SnakeQT::SnakeQT(SnakeController &controller, QWidget *parent) : QWidget(parent), controller(controller), gametimer(nullptr){
    setFixedSize(400,420);

    gametimer = new QTimer(this);
    connect(gametimer, &QTimer::timeout, this, &SnakeQT::UpdateGame);
    gametimer->start(1);
}

SnakeQT::~SnakeQT(){
    emit gameClosed();
}

void SnakeQT::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    QWidget::paintEvent(event);

    DrawField(painter);
    DrawInfoBar(painter);

    if(controller.snake_.GetPauseState() == STARTED){
    DrawSnake(painter);
    DrawApple(painter);
    }

    PrintMasseges(painter);
}

void SnakeQT::DrawInfoBar(QPainter &painter) {
    QPen pen(Qt::black);
    painter.setPen(pen);

    painter.drawRect(230, 10, 100, 90);
    painter.drawRect(230, 110, 100, 90);
    painter.drawRect(230, 210, 100, 90);
    painter.drawRect(230, 310, 100, 100);
    painter.drawText(265, 30, "Level:");
    painter.drawText(275, 70, QString::number(controller.snake_.GetLevel()));
    painter.drawText(260, 130,"Score:");
    painter.drawText(275, 170, QString::number(controller.snake_.GetScore()));
    painter.drawText(245, 230,"High Score:");
    painter.drawText(275, 270, QString::number(controller.snake_.GetHighScore()));
}

void SnakeQT::DrawField(QPainter &painter) {
    QPen pen(Qt::black);
    painter.setPen(pen);
    painter.drawRect(SHIFT_X, SHIFT_Y, FIELD_W * CELL_SIZE, FIELD_H * CELL_SIZE);
}

QColor SnakeQT::GetColor(SnakeColor color) {
    switch (color) {
    case EMPTY:

    case BODY:
        return QColor(119, 141, 69);

    case HEAD:
        return QColor(52, 76, 17);

    case APPLE:
        return QColor(232, 49, 0);

    default:
        return QColor(255, 255, 255);
    }
}

void SnakeQT::DrawSnake(QPainter &painter){
    QColor bodyColor = GetColor(BODY);
    painter.setBrush(bodyColor);
    painter.setPen(Qt::NoPen);

    for(size_t i = 1; i < controller.snake_.snake_coordinates_.size(); ++i){
        const auto &segment = controller.snake_.snake_coordinates_[i];
        painter.drawRect(segment.x * CELL_SIZE, segment.y * CELL_SIZE - SHIFT_Y, CELL_SIZE, CELL_SIZE);
    }

    QColor headColor = GetColor(HEAD);
    painter.setBrush(headColor);

    const auto &head = controller.snake_.snake_coordinates_.front();
    painter.drawRect(head.x * CELL_SIZE, head.y * CELL_SIZE - SHIFT_Y, CELL_SIZE, CELL_SIZE);
}

void SnakeQT::DrawApple(QPainter &painter){
    int appleX = controller.snake_.GetApple()[0][0];
    int appleY = controller.snake_.GetApple()[0][1];

    QColor appleColor = GetColor(APPLE);
    painter.setBrush(appleColor);

    painter.drawEllipse(appleX * CELL_SIZE, appleY * CELL_SIZE - SHIFT_Y, CELL_SIZE, CELL_SIZE);
}

void SnakeQT::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        controller.UserInput(Up, 0);
        break;
    case Qt::Key_Down:
        controller.UserInput(Down, 0);
        break;
    case Qt::Key_Left:
        controller.UserInput(Left, 0);
        break;
    case Qt::Key_Right:
        controller.UserInput(Right, 0);
        break;
    case Qt::Key_Return:
        controller.UserInput(Start, 0);
        break;
    case Qt::Key_P:
        controller.UserInput(Pause, 0);
            break;
    case Qt::Key_Escape:
        controller.UserInput(Terminate, 0);
        gameClosed();
        close();
        break;
    case Qt::Key_Space:
        controller.UserInput(Action, 1);
        break;
    default:
        QWidget::keyPressEvent(event);

    }
}

void SnakeQT::PrintMasseges(QPainter &painter){
    QPen pen(Qt::black);
    painter.setPen(pen);

    if(controller.snake_.GetPauseState() == NOT_STARTED){
        painter.drawText(65,200, "PrEsS eNtEr To StArT");
    }else if(controller.snake_.GetPauseState() == PAUSED){
        painter.drawText(100, 200, "PaUsE");
    }else if(controller.snake_.GetPauseState() == LOSED){
        painter.drawText(100, 200, "YoU lOsT)");
    }else if(controller.snake_.GetPauseState() == WIN){
        painter.drawText(100, 200, "yOu WiN)");
    }else if(controller.snake_.GetPauseState() == QUIT){
        painter.drawText(65,200, "PrEsS eNtEr To CoNtInUe");
    }
}

void SnakeQT::UpdateGame(){
    controller.UpdateCurrentState();

    if(controller.snake_.GetPauseState() == LOSED || controller.snake_.GetPauseState() == WIN){
        QTimer::singleShot(2000, this, &SnakeQT::ResetGame);
        update();
    }else{
        update();
    }

}

void SnakeQT::CloseEvent(QCloseEvent *event){
    emit gameClosed();
    QWidget::closeEvent(event);
}

void SnakeQT::ResetGame(){
    controller.ResetController();
    update();
}

}//namespace s21


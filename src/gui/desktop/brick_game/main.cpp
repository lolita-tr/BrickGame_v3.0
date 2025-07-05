#include "desktop_main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    s21::Snake snakeModel;
    s21::SnakeController controller(snakeModel);

    Desktop window(controller);
    window.setWindowTitle("mEnU");
    window.show();

    return app.exec();
}

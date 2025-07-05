/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_desktop
{
public:
    QWidget *centralwidget;
    QPushButton *snake_button;
    QPushButton *tetris_button;
    QPushButton *exit_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *desktop)
    {
        if (desktop->objectName().isEmpty())
            desktop->setObjectName("desktop");
        desktop->resize(800, 600);
        centralwidget = new QWidget(desktop);
        centralwidget->setObjectName("centralwidget");
        snake_button = new QPushButton(centralwidget);
        snake_button->setObjectName("snake_button");
        snake_button->setGeometry(QRect(150, 70, 461, 111));
        tetris_button = new QPushButton(centralwidget);
        tetris_button->setObjectName("tetris_button");
        tetris_button->setGeometry(QRect(150, 190, 461, 111));
        exit_button = new QPushButton(centralwidget);
        exit_button->setObjectName("exit_button");
        exit_button->setGeometry(QRect(150, 310, 461, 111));
        desktop->setCentralWidget(centralwidget);
        menubar = new QMenuBar(desktop);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        desktop->setMenuBar(menubar);
        statusbar = new QStatusBar(desktop);
        statusbar->setObjectName("statusbar");
        desktop->setStatusBar(statusbar);

        retranslateUi(desktop);

        QMetaObject::connectSlotsByName(desktop);
    } // setupUi

    void retranslateUi(QMainWindow *desktop)
    {
        desktop->setWindowTitle(QCoreApplication::translate("desktop", "MainWindow", nullptr));
        snake_button->setText(QCoreApplication::translate("desktop", "Snake", nullptr));
        tetris_button->setText(QCoreApplication::translate("desktop", "Tetris", nullptr));
        exit_button->setText(QCoreApplication::translate("desktop", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class desktop: public Ui_desktop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

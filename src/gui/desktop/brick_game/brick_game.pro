QT       += core gui widgets
UI       += desktop.ui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    desktop_snake.cpp \
    desktop_tetris.cpp \
    desktop_main.cpp \
    ../../../brick_game/snake/snake.cpp \
    ../../../brick_game/snake/snake_controller.cpp \
    ../../../brick_game/tetris/field.c \
    ../../../brick_game/tetris/figure.c \
    ../../../brick_game/tetris/fsm.c \
    ../../../brick_game/tetris/utility.c \


HEADERS += \
    desktop_snake.h \
    desktop_tetris.h \
    desktop_main.h \
    ../../../inc/snake/snake.h \
    ../../../inc/snake/snake_controller.h \
    ../../../inc/defines.h \
    ../../../inc/tetris/figures.h \
    ../../../inc/tetris/fsm.h \

FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

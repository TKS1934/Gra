TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
INCLUDEPATH += C:\Users\marci\studia\qt_programy\SFML\include
LIBS += -LC:\Users\marci\studia\qt_programy\SFML\lib \
            -lsfml-graphics \
            -lsfml-window \
            -lsfml-system

HEADERS += \
    blok.h \
    byt.h \
    funkcje.h \
    gracz.h
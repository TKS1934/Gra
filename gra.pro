TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
INCLUDEPATH += C:\Users\jarek\Desktop\SFML-2.6.2\include
LIBS += -LC:\Users\jarek\Desktop\SFML-2.6.2\lib \
            -lsfml-graphics \
            -lsfml-window \
            -lsfml-system

HEADERS += \
    blok.h \
    byt.h \
    funkcje.h
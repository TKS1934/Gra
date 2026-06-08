TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH += "$$PWD/SFML/include"


LIBS += -L"$$PWD/SFML/lib"


CONFIG(debug, debug|release) {
    LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lsfml-audio-d -lsfml-network-d
} else {
    LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
}

HEADERS += \
    blok.h \
    byt.h \
    funkcje.h\
    gracz.h\
    skrzynka1.h
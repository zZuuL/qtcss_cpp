TEMPLATE = app
TARGET   = bin/qtcss
QT      += widgets uitools
CONFIG  += c++14
CONFIG  += qt
DEFINES += QT_DEPRECATED_WARNINGS

OBJECTS_DIR = gen
MOC_DIR = gen/.moc
RCC_DIR = gen/.rcc
UI_DIR  = gen/.ui

HEADERS += src/*.h
SOURCES += src/*.cpp

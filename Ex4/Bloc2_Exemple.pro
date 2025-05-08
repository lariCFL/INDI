CONFIG += QMAKE_APPLE_DEVICE_ARCHS=arm64

TARGET = ejecutable
TEMPLATE = app

QT += core gui openglwidgets widgets

INCLUDEPATH += /opt/homebrew/opt/glm/include

# Diretório com os headers e source do model
INCLUDEPATH += $$PWD/../../Models_3D/Model
DEPENDPATH += $$PWD/../../Models_3D/Model

# Adicionando os arquivos ao projeto
SOURCES += $$PWD/../../Models_3D/Model/model.cpp
HEADERS += $$PWD/../../Models_3D/Model/model.h


FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp

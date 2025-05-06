CONFIG += QMAKE_APPLE_DEVICE_ARCHS=arm64

TEMPLATE = app
QT += core gui openglwidgets widgets

INCLUDEPATH += /opt/homebrew/opt/glm/include  \
                ./Model 

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
           BL2GLWidget.cpp MyGLWidget.cpp  \
           ./Model/model.cpp

CONFIG += QMAKE_APPLE_DEVICE_ARCHS=arm64

TEMPLATE = app
QT += core gui openglwidgets widgets

INCLUDEPATH += /opt/homebrew/opt/glm/include

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp \
        MyForm.cpp MyGLWidget.cpp

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert

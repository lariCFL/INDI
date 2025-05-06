#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":"<<text<<std::endl;

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL () {
    // Chama a inicialização de BL2GLWidget
    BL2GLWidget::initializeGL();
    posRick = glm::vec3(0, 0, 0);
}

void MyGLWidget::paintGL() {
    // Chama a implementação de BL2GLWidget
    BL2GLWidget::paintGL();

}


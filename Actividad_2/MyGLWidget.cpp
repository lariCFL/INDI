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
    alcadaRick = 1.5;
    setRickPosition(-5, 0, 0);
}

void MyGLWidget::paintGL() {
    // Chama a implementação de BL2GLWidget
    BL2GLWidget::paintGL();

    glBindVertexArray (VAO_Cub);
    CubTransform();
    glDrawArrays(GL_TRIANGLES, 0, 36);  

}

void MyGLWidget::setRickPosition(float x, float y, float z) {
    posRick = glm::vec3(x, y, z);
}

void MyGLWidget::CubTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3 (0, 0, cubPos));
    TG = glm::scale(TG, glm::vec3(0.5, 2, 3));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
    cubPos *= -1;
}


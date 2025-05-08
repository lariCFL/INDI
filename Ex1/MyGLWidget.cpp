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
    DEBUG("InitializeGL");

    // Obtenha o local do uniforme de projeção
    projLoc = glGetUniformLocation(program->programId(), "proj"); // Agora 'program' é acessível
}

void MyGLWidget::paintGL () {
    // Chama o paintGL de BL2GLWidget
    BL2GLWidget::paintGL();
    DEBUG("PaintGL");

    // Chama o método para aplicar a projeção
    ProjectTransform();
}

void MyGLWidget::ProjectTransform(){
    // Criando a matriz de projeção com os parâmetros solicitados
    glm::mat4 Proj = glm::perspective((float)M_PI / 2.0f, 1.0f, 0.4f, 3.0f);

    // Enviando a matriz de projeção como uniforme para o shader
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}
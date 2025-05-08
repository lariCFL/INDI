#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":"<<text<<std::endl;

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL () {
    alcadaVideoCamera = 0.5;
    alcadaRick = 1.5;
    // Chama a inicialização de BL2GLWidget
    BL2GLWidget::initializeGL();
    setRickPosition(-5, 0, 0);
    angleVideoCamera *= 180;
    VideoCameraTransform();
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

void MyGLWidget::VideoCameraTransform() {
    glm::mat4 TG(1.0f);  

    TG = glm::translate(TG, glm::vec3(0, 2.25f, -1.0f));
    TG = glm::scale(TG, glm::vec3(escalaVideoCamera, escalaVideoCamera, escalaVideoCamera));
    TG = glm::translate(TG, -centreCaixaVideoCamera);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


// ----------------------------------------

// Ángulos iniciales: ψ = M_PI/4.0 y θ = M_PI/4.0.

void MyGLWidget::iniCamera() {
    // Define a posição do observador, o ponto de referência e o vetor "up"
    obs = glm::vec3(10, 5, 0);  
    vrp = glm::vec3(0, 0, 0);  
    up = glm::vec3(0, 1, 0);    
    fov = float(M_PI) / 4.0f;  // Campo de visão em perspectiva
    znear = 1.0f;  // Distância do plano de recorte próximo
    zfar  = 30.0f;  // Distância do plano de recorte distante

    // Aplica a transformação de visualização
    viewTransform();
}



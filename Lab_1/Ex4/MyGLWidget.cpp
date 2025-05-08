// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":"<<text<<std::endl;


MyGLWidget::~MyGLWidget() {
}


void MyGLWidget::initializeGL ( ){

    BL2GLWidget::initializeGL();
    //DEBUG("InitializeGL");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    transLoc = glGetUniformLocation(program->programId(), "trans");

    CreateBuffersModels();
}

void MyGLWidget::CreateBuffersModels() {
    // Carregar o modelo HomerProves.obj antes de criar os buffers
    m.load("../Models_3D/HomerProves.obj");

    // Criar os buffers para o modelo
    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);
    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);

    // Criar o VBO para os vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3,
                 m.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Criar o VBO para o material (informações de cor)
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3,
                 m.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray(0);  // Desvincular o VAO
}


void MyGLWidget::paintGL ( ){

    //BL2GLWidget::paintGL();
    DEBUG("PaintGL");
    // Call the projectTransform method
    projectTransform();
}

void MyGLWidget::projectTransform() {
    ini_camera();
    viewTransform();
   
    // Set the projection matrix
    glm::mat4 Trans = glm::perspective(FOV, ra, znear, zfar);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Trans[0][0]);
    DEBUG("ProjectTransform");
}

void MyGLWidget::viewTransform() {
    // Set the view matrix
    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    
    // Set the view matrix in the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    DEBUG("ViewTransform");
}

void MyGLWidget::ini_camera() {
    // Initialize camera parameters

    // OBS: Posição do observador (câmera) no espaço 3D. Aqui está em (0, 0, 1), ou seja,
    // 1 unidade no eixo Z, olhando para a origem (0, 0, 0).
    OBS = glm::vec3(0, 0, 1);

    // VRP (View Reference Point): Ponto de referência da visão, ou seja, o ponto para onde
    // a câmera está olhando. Aqui está definido como a origem (0, 0, 0).
    VRP = glm::vec3(0, 0, 0);

    // UP: Vetor "para cima", que define a orientação da câmera. Aqui está definido como
    // (0, 1, 0), indicando que o eixo Y é o "cima" da câmera.
    UP = glm::vec3(0, 1, 0);

    // FOV (Field of View): Campo de visão vertical da câmera, em radianos.
    // Aqui está definido como π/2 (90 graus), ou seja, um campo de visão bem amplo.
    FOV = float(M_PI) / 2.0f;

    // ra (Aspect Ratio): Razão de aspecto da câmera, que é a proporção entre largura e altura
    // da tela. Aqui está definido como 1.0, indicando uma tela quadrada.
    ra = 1.0f;

    // znear: Distância do plano de corte próximo (near plane). Objetos mais próximos que 0.4
    // unidades da câmera não serão renderizados.
    znear = 0.4f;

    // zfar: Distância do plano de corte distante (far plane). Objetos mais distantes que 3.0
    // unidades da câmera não serão renderizados.
    zfar = 3.0f;

    // DEBUG: Função para depuração, provavelmente imprime ou registra que a função ini_camera foi chamada.
    DEBUG("ini_camera");
}

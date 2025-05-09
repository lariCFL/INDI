#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

MyGLWidget::~MyGLWidget()
{
}


/*
void MyGLWidget::setRickPosition(float x, float y, float z)
{
    posRick = glm::vec3(x, 0, 0);
}
*/

void MyGLWidget::initializeGL()
{
    alcadaVideoCamera = 0.5;
    alcadaRick = 1.5;

    anglePsi = M_PI / 4.0f;
    angleTheta = M_PI / 4.0f;
    AltCam = false;

    // Chama a inicialização de BL2GLWidget
    BL2GLWidget::initializeGL();


    posRick = glm::vec3(-5, 0, 0);
    angleVideoCamera *= 180;
    VideoCameraTransform();
}

void MyGLWidget::paintGL()
{
    // Chama a implementação de BL2GLWidget
    BL2GLWidget::paintGL();

    if (!AltCam) viewTransform();

    glBindVertexArray(VAO_Cub);
    CubTransform();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MyGLWidget::CubTransform()
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, cubPos));
    TG = glm::scale(TG, glm::vec3(0.5, 2, 3));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
    cubPos *= -1;
}

void MyGLWidget::VideoCameraTransform()
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(0, 2.25f, -1.0f));
    TG = glm::scale(TG, glm::vec3(escalaVideoCamera, escalaVideoCamera, escalaVideoCamera));
    TG = glm::translate(TG, -centreCaixaVideoCamera);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::actCam(){
    obs = glm::vec3(posRick.x, posRick.y + 2.5, posRick.z - 2);
    vrp = glm::vec3(posRick.x, posRick.y, posRick.z);
    up = glm::vec3(0, 1, 0);
}

// ----------------------------------------

// Ángulos iniciales: ψ = M_PI/4.0 y θ = M_PI/4.0.

void MyGLWidget::iniCamera()
{
    // Define a posição do observador, o ponto de referência e o vetor "up"
    obs = glm::vec3(16, 5, 0);
    vrp = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    fov = fovIni; // Campo de visão em perspectiva
    znear = 1.0f;             // Distância do plano de recorte próximo
    zfar = 30.0f;             // Distância do plano de recorte distante

    // Aplica a transformação de visualização
    viewTransform();
}

// ----------------------------------------

void MyGLWidget::resizeGL(int width, int height)
{
    BL2GLWidget::resizeGL(width, height);
    // Ajusta o FOV vertical se a janela for mais alta do que larga
    if (ra < 1.0f)
        fov = 2.0f * atan(tan(fovIni / 2.0f) / ra);
    else
        fov = fovIni;
}

void MyGLWidget::viewTransform()
{
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(0, 0, -radiEscena * 2));
    View = glm::rotate(View, anglePsi, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -angleTheta, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEscena);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    
}


// ----------------------------------------

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (DoingInteractive == ROTATE) {
        angleTheta += (event->x() - xClick) / float(alt);
        anglePsi += (event->y() - yClick) / float(alt);
        xClick = event->x();
        yClick = event->y();
       // DEBUG("angleTheta: " << angleTheta << " anglePsi: " << anglePsi);
        
        viewTransform(); // RE-APLICA a matriz de visualização com os novos ângulos
        update();
    }
}


//teclas left and right
void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_Left:
            if (posRick.x > -6) { posRick.x -= 1; angleRick = (3*M_PI)/2;}
            break;
        case Qt::Key_Right:
            if (posRick.x < 6) { posRick.x += 1; angleRick = M_PI /2; }
            break;
        default:
            event->ignore();
            break;
    }
    viewTransform();
    update();
}
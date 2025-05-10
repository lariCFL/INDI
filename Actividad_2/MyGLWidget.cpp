#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

// Destructor de la clase
MyGLWidget::~MyGLWidget()
{
}

// Inicializa los parámetros de OpenGL y la cámara
void MyGLWidget::initializeGL()
{
    alcadaVideoCamera = 0.5;
    alcadaRick = 1.5;
    cubPos = 2.5;
    cubSizeX = 0.5;
    cubSizeZ = 3;

    Camera1 = true;

    BL2GLWidget::initializeGL();

    posRick = glm::vec3(-5, 0, 0);
    emit(posRickSlider(posRick.x));
    angleVideoCamera *= 180;
    VideoCameraTransform();
}

// Renderiza la escena
void MyGLWidget::paintGL()
{
    glUniform1i(usaColorUniformLoc, false);  // usa cor do VBO

    cubPos = 2.5;
    cubSizeX = 0.5;
    cubSizeZ = 3;

    BL2GLWidget::paintGL();

    cubPos *= -1;

    glBindVertexArray(VAO_Cub);
    CubTransform();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUniform3f(colLoc, 1.0, 0.0, 0.0);  // vermelho
    glUniform1i(usaColorUniformLoc, true);  // usa cor do VBO

    cubSizeX = 0.25;
    cubSizeZ = 2;

    if (posRick.x <= 2 && posRick.x >= -2)
    {
        cubPos = 2;
    }
    else { cubPos = 0; }

    glBindVertexArray(VAO_Cub);
    CubTransform();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    viewTransform();
}

// Carga los shaders y crea los buffers para los modelos
void MyGLWidget::carregaShaders()
{
    BL2GLWidget::carregaShaders();
    colLoc = glGetUniformLocation(program->programId(), "col");
    usaColorUniformLoc = glGetUniformLocation(program->programId(), "usaColorUniform");
}

// ------------ Transformaciones

// Aplica la transformación al cubo
void MyGLWidget::CubTransform()
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(0, 0, cubPos));
    TG = glm::scale(TG, glm::vec3(cubSizeX, 2, cubSizeZ));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// Aplica la transformación a la cámara de video
void MyGLWidget::VideoCameraTransform()
{
    glm::mat4 TG(1.0f);

    TG = glm::translate(TG, glm::vec3(0, 2.25f, -1.0f));
    TG = glm::scale(TG, glm::vec3(escalaVideoCamera, escalaVideoCamera, escalaVideoCamera));
    TG = glm::translate(TG, -centreCaixaVideoCamera);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// ------------ Control Camera

// Inicializa los parámetros de la cámara
void MyGLWidget::iniCamera()
{
    anglePsi = M_PI / 4.0f;
    angleTheta = M_PI / 4.0f;
    actualizarCamera();
    BL2GLWidget::iniCamera();
}

// Actualiza los parámetros de la cámara dependiendo de Camera1
void MyGLWidget::actualizarCamera()
{
    emit(isCamera1(Camera1));
    emit(isCamera2(!Camera1));
    if (Camera1) // Configuración para la cámara principal
    {
        obs = glm::vec3(10, 5, 0);
        vrp = glm::vec3(0, 0, 0);
        fov = fov2;
        znear = 10.0f;
    }
    else // Configuración para la cámara secundaria
    {
        obs = glm::vec3(0, 2.25, -1);
        vrp = glm::vec3(posRick.x, posRick.y, posRick.z);
        fov = M_PI / 2.0f;
        znear = 0.1f;
    }
    BL2GLWidget::projectTransform();
}

// Aplica la transformación de vista según la cámara activa
void MyGLWidget::viewTransform()
{
    if (Camera1) // Transformación para la cámara principal
    {
        glm::mat4 View(1.0f);
        View = glm::translate(View, glm::vec3(0, 0, -radiEscena * 2));
        View = glm::rotate(View, anglePsi, glm::vec3(1, 0, 0));
        View = glm::rotate(View, -angleTheta, glm::vec3(0, 1, 0));
        View = glm::translate(View, -centreEscena);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    else // Transformación para la cámara secundaria
    {
        actualizarCamera();
        glm::mat4 View(1.0f);
        View = glm::lookAt(obs, vrp, up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    }
}

// Ajusta el campo de visión al cambiar el tamaño de la ventana
void MyGLWidget::resizeGL(int width, int height)
{
    BL2GLWidget::resizeGL(width, height);
    if (ra < 1.0f)
        fov = 2.0f * atan(tan(M_PI / 4.0f / 2.0f) / ra);
    else
        fov = M_PI / 4.0f;

    fov2 = fov;
}

// ------------ Interacciones

// Maneja el movimiento del ratón para rotar la cámara
void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (DoingInteractive == ROTATE)
    {
        if (Camera1) // Rotación solo para la cámara principal
        {
            angleTheta += (event->x() - xClick) / float(alt);
            anglePsi += (event->y() - yClick) / float(alt);
            xClick = event->x();
            yClick = event->y();
        }
        update();
    }
}

// Maneja los eventos de teclado
void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    makeCurrent();
    switch (event->key())
    {
    case Qt::Key_Down: // Mueve a Rick hacia abajo
        if (posRick.x > -6)
        {
            posRick.x -= 1;
            angleRick = (3 * M_PI) / 2;
            emit(posRickSlider(posRick.x));
        }
        break;
    case Qt::Key_Up: // Mueve a Rick hacia arriba
        if (posRick.x < 6)
        {
            posRick.x += 1;
            angleRick = M_PI / 2;
            emit(posRickSlider(posRick.x));
        }
        break;
    case Qt::Key_C: // Cambia entre cámaras
        Camera1 = !Camera1;
        if (Camera1)
            actualizarCamera();
        break;
    case Qt::Key_R: // Reinicia la posición de Rick y la cámara
        Camera1 = true;
        posRick = glm::vec3(-5, 0, 0);
        fov = fov2;
        iniCamera();
        break;

    default: // Ignora otros eventos
        event->ignore();
        break;
    }
    viewTransform();
    update();
}

// ------------ Interfaces
void MyGLWidget::Cam1(bool cam)
{
    Camera1 = true;
    actualizarCamera();
    viewTransform();
    paintGL();
    update();
}

void MyGLWidget::Cam2(bool cam)
{
    Camera1 = false;
    actualizarCamera();
    viewTransform();
    paintGL();
    update();
}

void MyGLWidget::MoveRick(int value)
{
    if (posRick.x > value){
        angleRick = M_PI / 2;
    }
    else{
        angleRick = (3 * M_PI) / 2;
    }
    posRick.x = value;
    viewTransform();
    paintGL();
    update();
}
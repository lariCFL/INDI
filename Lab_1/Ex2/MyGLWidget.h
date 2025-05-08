#include "BL2GLWidget.h"
#include <glm/glm.hpp>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    void initializeGL() override;
    void paintGL() override;

    void viewTransform();    // Configura a view matrix
    void projectTransform(); // Configura a projection matrix
    void iniCamera();        // Inicializa parâmetros da câmera

  private:
    glm::vec3 OBS;  // Posição do observador (câmera)
    glm::vec3 VRP;  // Ponto para onde a câmera está olhando
    glm::vec3 UP;   // Vetor que define a orientação "para cima"
    float FOV;      // Campo de visão (field of view) em radianosFOV;      // Campo de visão (field of view) em radianos
    float ra;       // Aspect ratio (proporção da janela)
    float znear;    // Plano near
    float zfar;     // Plano far
};
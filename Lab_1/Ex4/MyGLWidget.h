// MyGLWidget.h
#include "BL2GLWidget.h"

#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    void initializeGL( )  override;
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    void paintGL( ) override;

    void projectTransform();

    void viewTransform();

    void ini_camera();

    void CreaBuffersModels();

    GLuint viewLoc, transLoc;

    glm::vec3 OBS, VRP, UP;
    float FOV, ra, znear, zfar;

    Model m;
    GLuint VAO_Homer;
};

#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    // Métodos principales
    void initializeGL() override;
    void paintGL() override;

  private:
    // Transformaciones
    void CubTransform() override;
    void VideoCameraTransform() override;

    // Cámara
    void iniCamera() override;
    void viewTransform() override;
    void resizeGL(int width, int height) override;
    void actualizarCamera();

    // Atributos de la cámara
    bool Camera1;
    float fov2;
    float anglePsi;
    float angleTheta;

    // Interacción
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    // Otros
    float cubPos = 2.5;
};
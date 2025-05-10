#include "BL2GLWidget.h"
#include "model.h"

// ------------------------------------------------------------------------
// Clase principal
// ------------------------------------------------------------------------
class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  // ----------------------------------------------------------------------
  // Sección pública
  // ----------------------------------------------------------------------
  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  // ----------------------------------------------------------------------
  // Sección protegida (métodos principales)
  // ----------------------------------------------------------------------
  protected:
    void initializeGL() override;
    void paintGL() override;

  // ----------------------------------------------------------------------
  // Sección privada
  // ----------------------------------------------------------------------
  private:
    // --------------------------------------------------------------------
    // Transformaciones
    // -----------------------------------------------------------------
    void CubTransform() override;
    void VideoCameraTransform() override;

    // -----------------------------------------------------------------
    // Manejo de cámara
    // -----------------------------------------------------------------
    void iniCamera() override;
    void viewTransform() override;
    void resizeGL(int width, int height) override;
    void actualizarCamera();

    // -----------------------------------------------------------------
    // Atributos de cámara
    // -----------------------------------------------------------------
    bool Camera1;
    float fov2;
    float anglePsi;
    float angleTheta;

    // -----------------------------------------------------------------
    // Interacción
    // -----------------------------------------------------------------
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    // -----------------------------------------------------------------
    // Atributos del cubo
    // -----------------------------------------------------------------
    float cubPos;
    float cubSizeX, cubSizeZ;

    // -----------------------------------------------------------------
    // Shaders
    // -----------------------------------------------------------------
    void carregaShaders() override;
    GLuint colLoc, usaColorUniformLoc;

  // -----------------------------------------------------------------
  // Señales
  // -----------------------------------------------------------------
  signals:
    void isCamera1(bool cam);
    void isCamera2(bool cam);
    void posRickSlider(int value);

  // -----------------------------------------------------------------
  // Slots públicos
  // -----------------------------------------------------------------
  public slots:
    void Cam1(bool cam);
    void Cam2(bool cam);
    void MoveRick(int value);
};

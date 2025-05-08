#include "BL2GLWidget.h"

#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();


  protected:
    void initializeGL( )  override;
    void paintGL( )      override;
    void setRickPosition(float x, float y, float z);

  private:
    float cubPos = 2.5;
    void CubTransform() override;
    void VideoCameraTransform() override;

    // Camara
    void iniCamera() override;

};
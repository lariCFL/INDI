#include "BL2GLWidget.h"
#include <glm/glm.hpp>

#include <QFile>
#include <QDebug>

#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    void initializeGL() override;    // OpenGL initialization
    void paintGL() override;         // OpenGL rendering

    void viewTransform();    // Configures the view matrix
    void projectTransform(); // Configures the projection matrix
    void iniCamera();        // Initializes camera parameters

    Model m;  // 3D model loader/storage

  private:
    // OpenGL buffers:
    GLuint VAO;          // Vertex Array Object
    GLuint VBO_vertices; // Vertex Buffer Object for vertex data
    GLuint VBO_colors;   // Vertex Buffer Object for color data
    
    // Camera parameters:
    glm::vec3 OBS;  // Observer (camera) position
    glm::vec3 VRP;  // View Reference Point (camera target)
    glm::vec3 UP;   // Up vector (camera orientation)
    float FOV;      // Field of view (in radians)
    float ra;       // Aspect ratio (window proportion)
    float znear;    // Near clipping plane
    float zfar;     // Far clipping plane
    void loadModel(); 

};
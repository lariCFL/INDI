#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class BL2GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    BL2GLWidget (QWidget *parent=0);
    ~BL2GLWidget ();

  protected:
    int printOglError(const char file[], int line, const char func[]);
  
    // initializeGL - Aquí incluimos las inicializaciones del contexto gráfico.
    virtual void initializeGL ( );
    // paintGL - Método llamado cada vez que es necesario refrescar la ventana.
    // Todo lo que se dibuja se dibuja aquí.
    virtual void paintGL ( );
    // resizeGL - Es llamado cuando cambia el tamaño del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es llamado cuando se presiona una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Son llamados cuando se realiza el evento
    // correspondiente del ratón
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    // iniEscena - Aquí incluimos las inicializaciones de la escena
    virtual void iniEscena ();
    // iniCamera - Aquí incluimos las inicializaciones de la cámara
    virtual void iniCamera ();    
    // creaBuffersModels - Aquí cargamos los archivos obj y hacemos la inicialización de los diferentes VAOs y VBOs
    virtual void creaBuffersModels ();
    // creaBuffersTerra - Aquí inicializamos el VAO y los VBO de un modelo hecho a mano que representa un cubo
    virtual void creaBuffersCub ();
    // creaBuffersTerra - Aquí inicializamos el VAO y los VBO de un modelo hecho a mano que representa un suelo
    virtual void creaBuffersTerra ();
    // carregaShaders - Aquí cargamos los shaders, los compilamos y los enlazamos. También buscamos las uniform locations que utilizamos.
    virtual void carregaShaders ();
    
    // viewTransform y projecTransform - Se utilizan para construir y enviar a los shaders las matrices de cámara (View y Projection respectivamente).
    virtual void viewTransform ();
    virtual void projectTransform ();
    
    // {Rick/VideoCamera/Cub/ident}Transform - Funciones para construir y enviar al shader diferentes matrices de transformación geométrica TG
    virtual void RickTransform();
    virtual void VideoCameraTransform();
    virtual void CubTransform();
    virtual void identTransform();
    
    // calculaCapsaModel - Calcula la caja contenedora de un Modelo p retornando el centro de su base en CentreBase, y el factor de escala necesario para que su altura sea alcadaDesitjada.
    virtual void calculaCapsaModel (Model &p, float &escala, float alcadaDesitjada, glm::vec3 &CentreBase);

    // attribute locations
    GLuint vertexLoc, colorLoc;

    // uniform locations
    GLuint transLoc, viewLoc, projLoc;

    // VAO names
    GLuint VAO_Rick;
    GLuint VAO_VideoCamera;
    GLuint VAO_Terra;
    GLuint VAO_Cub;

    // Program
    QOpenGLShaderProgram *program;

    // Viewport
    GLint ample, alt;

    // Mouse interaction
    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float factorAngleX, factorAngleY;

    // Internal vars
    float radiEscena;
    float fov, ra, znear, zfar;
    glm::vec3 centreEscena, obs, vrp, up;

    // Modelo, posición, escala y orientación de Rick y de la videocámara
    Model rick,videoCamera;
    glm::vec3 posRick=glm::vec3(0,0,-3);
    float alcadaRick=2, alcadaVideoCamera=1;
    float angleRick = M_PI/2, angleVideoCamera=M_PI/2
    float escalaRick, escalaVideoCamera;
    glm::vec3 centreCaixaRick, centreCaixaVideoCamera;
    
    QTimer *timer;
    
    public slots:
    
    virtual void updatePosition();

};


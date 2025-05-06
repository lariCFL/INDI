#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream> // Adicione esta linha no início do arquivo


#ifndef __MyGLWidget__
#define __MyGLWidget__
class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
    void resetTime();

    float getTime();

    bool esModeManual(){
        return modeManual;
    }

  private:

    /// Tiempo (no tocar, se modifica solo en modo automático).
    float t = 0;

    /// Indica si el modo es manual (por teclas) o automático (por tiempo)
    bool modeManual=true;

  public slots:

    void tick();
    void setModeManual(bool esManual){
        modeManual=esManual;
    }

  signals:
    void timeUpdate(double time);

  protected:
    // initializeGL - Aquí incluimos las inicializaciones del contexto gráfico.
    virtual void initializeGL ();

    // paintGL - Método llamado cada vez que es necesario refrescar la ventana.
    // Todo lo que se dibuja se dibuja aquí.
    virtual void paintGL ();
 
    // resize - Se llama cuando cambia el tamaño del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Se llama cuando se presiona una tecla
     virtual void keyPressEvent (QKeyEvent *event);

  private:

    // Variables de estado =======================
    float rotation1 = 0, rotation2 = 0;

    const int CIRCLE_SAMPLES = 40;

    //-------------------------------------------------
    // Funciones generales de renderizado para objetos

    void pintaTerra();
    void pintaSuport(float d, float angle);
    void pintaBola(float d, float angle);

    void pintaEstrella();
    //-------------------------------------------------
    //  Creación de buffers
    void creaBuffers();
    void creaBufferBola();
    void creaBufferTerra();
    void creaBufferSuport();
    void creaBufferEstrella();

    //-------------------------------------------------
    // Funciones de transformación para los objetos
    void transformacioBola(float d, float angle);
    void transformacioSuport(float d, float angle);
    void transformacioTerra();

    //-------------------------------------------------
    void carregaShaders ();
    void createVBOs( int size, glm::vec3 Colors[], glm::vec3 Vertices[] );
    //-------------------------------------------------
    // Ubicación de atributos
    GLuint vertexLoc, colorLoc;
    // Uniforms
    GLuint transLoc;

    // Programa
    QOpenGLShaderProgram *program;

    GLuint VAO_TERRA, VAO_SUPORT, VAO_BOLA, VAO_ESTRELLA;
    GLint ample, alt;
    glm::vec3 COLOR_BLANC=glm::vec3(1.0,1.0,1.0);
    glm::vec3 COLOR_VERMELL=glm::vec3(1.0,0.5,0.3);
    glm::vec3 COLOR_BLAU=glm::vec3(48.0/255,58.0/255,158.0/255);
    glm::vec3 COLOR_BLAU_FLUIX=glm::vec3(109.0/255,117.0/255,201.0/255);
    glm::vec3 COLOR_GROC=glm::vec3(239.0/255,251.0/255,38.0/255);
    glm::vec3 COLOR_MARRO=glm::vec3(153.0/255,121.0/255,52.0/255);

    //-------------------------------------------------
    // Variables de control del modo automaático
    int fase = 1;
    float rotation1_timer = -40, rotation2_timer = 0;
    const float angle_min = 0, angle_max = 40;

    void moveBola();


};
#endif

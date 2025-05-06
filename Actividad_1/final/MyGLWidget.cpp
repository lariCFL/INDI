//#include <GL/glew.h>
#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <QTimer>
#include "MyForm.h"

#include <iostream>

// Constructor de la clase MyGLWidget
MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // Establece la política de enfoque para recibir eventos de teclado
}

// Destructor de la clase MyGLWidget
MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

// Método que actualiza el tiempo y refresca la interfaz gráfica
void MyGLWidget::tick(){
    t+=MyForm::CLOCK_PERIOD_MILIS/1000.0f;
    emit timeUpdate(t);
    update(); // Actualiza la interfaz gráfica (repinta)
}

// Método que reinicia el tiempo a cero
void MyGLWidget::resetTime(){
    t=0;
    emit timeUpdate(t);   

    // Resetea las variables de rotación para que siempre empiece en la misma posición
    // Bola de la punta izquierda en el punto más alto de su rotación
    fase = 1;
    rotation1_timer = -40;
    rotation2_timer = 0;
}

// Método que devuelve el tiempo actual
float MyGLWidget::getTime(){
    return t;
}

// Método que inicializa OpenGL
void MyGLWidget::initializeGL ()
{
    // Inicializa las funciones de OpenGL
    initializeOpenGLFunctions();
    
    // Configuração do OpenGL antes do loop principal
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glClearColor (200/255.0, 220/255.0, 255/255.0, 1.0); // Define el color de fondo
    carregaShaders();
    creaBuffers();
    // Inicialización de variables de estado
}

// Método para renderizar la escena
void MyGLWidget::paintGL ()
{
// Código específico para pantallas Retina en Mac
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

    glClear (GL_COLOR_BUFFER_BIT);  // Borra el frame-buffer
    program->bind();                // Activa el shader en el mac

    pintaTerra();

    float angle1, angle2;
    
    if(esModeManual()) {
        angle1 = rotation1;
        angle2 = rotation2;
    } else {
        angle1 = rotation1_timer;
        angle2 = rotation2_timer;
        moveBola();
    }
    float offsetX = 0.25/2;             //radio de la bola

    //Bola del medio derecho
    pintaBola(offsetX, 0);          
    pintaSuport(offsetX, 0);

    //Bola del medio izquierda
    pintaBola(-offsetX, 0);
    pintaSuport(-offsetX, 0);

    //Bola de la punta izquierda
    pintaBola(-3*offsetX, angle1);       
    pintaSuport(-3*offsetX, angle1); 
    
    //Bola de la punta derecho
    pintaBola(3*offsetX, angle2);         
    pintaSuport(3*offsetX, angle2);

    // Desactiva el VAO activo
    glBindVertexArray(0);
}

//Funcion que controla el movimiento de la bola si esta en modo automatico
void MyGLWidget::moveBola()
{
    float t = angle_max / 10;   //Velocidad de rotación

    //Fase 1: Bola de la punta izquierda baja
    //Fase 2: Bola de la punta derecha sube
    //Fase 3: Bola de la punta derecha baja
    //Fase 4: Bola de la punta izquierda sube

    switch (fase)
    {
        case 1:
            rotation1_timer += t;
            if (rotation1_timer >= angle_min){
                rotation1_timer = 0;
                fase += 1;
            }        break;
        case 2:
            rotation2_timer += t;
            if (rotation2_timer >= angle_max){
                rotation2_timer = 40;
                fase += 1;
            }        break;
        case 3:
            rotation2_timer -= t;
            if (rotation2_timer <= angle_min){
                rotation2_timer = 0;
                fase += 1;
            }        break;
        case 4:
            rotation1_timer -= t;
            if (rotation1_timer <= -angle_max){
                rotation1_timer = -40;
                fase = 1;
            }        break;
    }
}

// Método que ajusta el tamaño de la ventana
void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

// Métodos para pintar los diferentes objetos en la escena
void MyGLWidget::pintaBola(float d, float angle){
    transformacioBola(d,angle);
    glBindVertexArray(VAO_BOLA);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SAMPLES+1);
    pintaEstrella();        //Pinta la estrella en la bola
}

void MyGLWidget::pintaEstrella(){
    glBindVertexArray(VAO_ESTRELLA);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_INT, 0); // Posee EBO
}

void MyGLWidget::pintaSuport(float d, float angle){
    transformacioSuport(d,angle);
    glBindVertexArray(VAO_SUPORT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void MyGLWidget::pintaTerra() {
    transformacioTerra();
    glUniform1i(glGetUniformLocation(program->programId(), "isTerra"), true);   //Uniform del shader para diseñar los mimbres
    glBindVertexArray(VAO_TERRA);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    glUniform1i(glGetUniformLocation(program->programId(), "isTerra"), false);
}

// Método que maneja eventos de teclado
void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    if (esModeManual()) {
        makeCurrent();
        switch (event->key()) {
            case Qt::Key_A: {
                if (abs(rotation1) < 40) { rotation1 -= 5;}
                break;
            }
            case Qt::Key_D: {
                if (abs(rotation1) > 0) { rotation1 += 5;}
                break;
            }
            case Qt::Key_E: {
                if (abs(rotation2) < 40) { rotation2 += 5;}
                break;
            }
            case Qt::Key_Q: {
                if (abs(rotation2) > 0) { rotation2 -= 5;}
                break;
            }
            default: event->ignore(); break;
        }
        update(); 
    }
}

// Métodos de transformación de los objetos
void MyGLWidget::transformacioTerra(){
    glm::mat4 transform (1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioBola(float d, float angle){
    glm::mat4 transform (1.0f);
    

    // Vuelve a su posicion inicial
    transform = glm::translate(transform, glm::vec3(d, 0.5,  0.0));


    // Lo escala
    // Radio inicial en 0.5
    // Radio final en 0.125 -- Diametro: 0.25
    transform = glm::scale(transform, glm::vec3(0.25, 0.25, 0.25));

    // Rota
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));

    // Mueve a la posicion 0, 0, 0 para que las otras transformaciones no sean afectadas por su posicion
    transform = glm::translate(transform, glm::vec3(0, -0.5-2, 0.0f));


    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioSuport(float d, float angle){
    glm::mat4 transform (1.0f);

    // Vuelve a su posicion inicial
    transform = glm::translate(transform, glm::vec3(d, 0.5,  0.0));

    // Lo escala
    // Altura inical de 1.0 -- Ancho inicial de 0.8
    // Altura final de 0.5 -- Ancho final de 0.4
    transform = glm::scale(transform, glm::vec3(0.5, 0.5, 1));

    // Rota
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));

    // Mueve a la posicion 0, 0, 0 para que las otras transformaciones no sean afectadas por su posicion
    transform = glm::translate(transform, glm::vec3(0, -0.5, 0.0f));


    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

// Métodos para crear buffers
void MyGLWidget::creaBuffers (){
    creaBufferBola();
    creaBufferSuport();
    creaBufferTerra();
    creaBufferEstrella();
    glBindVertexArray(0);   //Desactiva el VAO
}

// Método para crear los VBOs (Vertex Buffer Objects)
void MyGLWidget::createVBOs(int size,  glm::vec3 Colors[], glm::vec3 Vertices[] ){
    GLuint VBO[2];   

    //Crea el buffer de vertices
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, size, Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    //Crea el buffer de colores
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, size, Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    //Desactiva el VAO
    glBindVertexArray(0);
}

// Métodos para crear buffers específicos para cada objeto
void MyGLWidget::creaBufferTerra(){

    glm::vec3 Vertices[6]; 
    Vertices[0] = glm::vec3(-1.00,  0.75, 0.0);
    Vertices[1] = glm::vec3(-0.50,  0.5, 0.0);
    Vertices[2] = glm::vec3(-1.00,  1.0, 0.0);
    Vertices[3] = glm::vec3( 0.5,  0.5, 0.0);
    Vertices[4] = glm::vec3( 1.00,  1.0, 0.0);
    Vertices[5] = glm::vec3( 1.00,  0.75, 0.0);

    // Crea el VAO usado para pintar
    glGenVertexArrays(1, &VAO_TERRA);
    glBindVertexArray(VAO_TERRA);

    glm::vec3 Colors[6];
    for(int i=0;i<6;i++) {
        Colors[i] = COLOR_MARRO;
    }

    createVBOs( sizeof(Vertices), Colors, Vertices );
}

void MyGLWidget::creaBufferSuport(){
    glm::vec3 Vertices[4];  

    Vertices[0] = glm::vec3(+0.04,  -0.5,  0.0);
    Vertices[1] = glm::vec3(-0.04,  -0.5,  0.0);
    Vertices[2] = glm::vec3(+0.04,  +0.5,  0.0);
    Vertices[3] = glm::vec3(-0.04,  +0.5,  0.0);


    glm::vec3 Colors[4];
    for(int i=0;i<4;i++) {
        Colors[i] = COLOR_BLAU;
    }

    // Crea el VAO usado para pintar
    glGenVertexArrays(1, &VAO_SUPORT);
    glBindVertexArray(VAO_SUPORT);

    createVBOs( sizeof(Vertices), Colors, Vertices );
}

void MyGLWidget::creaBufferBola(){
    glm::vec3   Colors[CIRCLE_SAMPLES+1];
    glm::vec3 Vertices[CIRCLE_SAMPLES+1];
    Vertices[0] = glm::vec3(0.0f , 0.0f, 0.0f);
    Colors[0]   = glm::vec3( 1.0,1.0,1.0);

    QColor q(244,247, 17);
    float h,s,v,a;                  // En el mac no funcina qreal y debe ser substituido por float
    q.getHsvF(&h,&s,&v,&a);

    float radi      = 0.5;
    float alfa      = 0;
    float deltaAlfa = 2.0*M_PI / (CIRCLE_SAMPLES-1);
    float deltaV    = 1.0 / (CIRCLE_SAMPLES-1);

    for(int k=1;k<=CIRCLE_SAMPLES; k++, alfa+=deltaAlfa){
        // calculo de las coordenadas de los vertex
        Vertices[k] = glm::vec3( radi * cos(alfa), radi * sin(alfa), 0.0);
        // calculo del color del gradiante
        q.setHsvF(h,s,v,a);
        Colors[k] = glm::vec3(q.red()/255.0, q.green()/255.0, q.blue()/255.0);
        if(k<CIRCLE_SAMPLES/2) {v-=deltaV;} else {v+=deltaV;}
    }

    // Crea el VAO usado para pintar

    glGenVertexArrays(1, &VAO_BOLA);
    glBindVertexArray(VAO_BOLA);

    createVBOs(sizeof(Vertices),  Colors, Vertices );
}

void MyGLWidget::creaBufferEstrella()
{
    const int sizee = 10;           //Vertices
    glm::vec3 Vertices[sizee];


    float start_angle = M_PI / 2.0f;
    float radio_ext =   0.25; 
    float radio_int = 0.125;

    for (int i = 0; i < sizee; i++) {
        float angulo = start_angle + i * (M_PI / 5.0f);         // 10 vértices, espacio de 36º 
        float raio = (i % 2 == 0) ? radio_ext : radio_int;      // Flag para vertice interno o externo
        Vertices[i] = glm::vec3(raio * cos(angulo), raio * sin(angulo), 0.0f);
    }

    glm::vec3 Colors[sizee];
    for (int i = 0; i < sizee; i++){
        Colors[i] = glm::vec3(236.0f / 255.0f, 107.0f / 255.0f, 70.0f / 255.0f);
    }

    GLuint Indices[] = {
        //Triangulos de las puntas de la estrella
        9, 0, 1,
        1, 2, 3,
        3, 4, 5,
        5, 6, 7,
        7, 8, 9,
        //Triangulos internos de la estrella
        1, 5, 3,
        1, 7, 9,
        1, 5, 7
    };


     // Crea el VAO usado para pintar
     glGenVertexArrays(1, &VAO_ESTRELLA);
     glBindVertexArray(VAO_ESTRELLA);
     
     // EBO para garantizar que los vertices sean pintados en el orden determinado por Indices[]
     // Reutiliza puntos
     GLuint EBO;
     glGenBuffers(1, &EBO);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

      createVBOs(sizeof(Vertices), Colors, Vertices);
}

void MyGLWidget::carregaShaders()
{
  // Creamos los shaders para el fragment shader y el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Cargamos el código de los archivos y los compilamos
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creamos el programa
  program = new QOpenGLShaderProgram(this);
  // Le añadimos los shaders correspondientes
  program->addShader(&fs);
  program->addShader(&vs);
  // Enlazamos el programa
  program->link();
  // Indicamos que este es el programa que queremos usar
  program->bind();

  // Obtenemos el identificador para el atributo “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation(program->programId(), "TG");
}
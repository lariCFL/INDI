// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include<utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":"<<text<<std::endl;


MyGLWidget::~MyGLWidget() {
}


void MyGLWidget::initializeGL ( ){

    BL2GLWidget::initializeGL();
    //DEBUG("InitializeGL");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    transLoc = glGetUniformLocation(program->programId(), "trans");

}

void MyGLWidget::paintGL ( ){

    BL2GLWidget::paintGL();
    DEBUG("PaintGL");
    // Call the projectTransform method
    projectTransform();
}

void MyGLWidget::projectTransform() {
    ini_camera();
    viewTransform();
   
    // Set the projection matrix
    glm::mat4 Trans = glm::perspective(FOV, ra, znear, zfar);

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &Trans[0][0]);
    DEBUG("ProjectTransform");
}

void MyGLWidget::viewTransform() {
    // Set the view matrix
    glm::mat4 View = glm::lookAt(OBS, VRP, UP);
    
    // Set the view matrix in the shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
    DEBUG("ViewTransform");
}

void MyGLWidget::ini_camera() {
    // Initialize camera parameters
    OBS = glm::vec3(0, 0, 1);
    VRP = glm::vec3(0, 0, 0);
    UP = glm::vec3(0, 1, 0);
    FOV = float(M_PI) / 2.0f;
    ra = 1.0f;
    znear = 0.4f;
    zfar = 3.0f;

    DEBUG("ini_camera");
}


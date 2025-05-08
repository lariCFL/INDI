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
    projLoc = glGetUniformLocation(program->programId(), "proj");
}

void MyGLWidget::paintGL ( ){

    BL2GLWidget::paintGL();
    DEBUG("PaintGL");
    // Call the projectTransform method
    projectTransform();
}

void MyGLWidget::projectTransform() {

    float FOV = float(M_PI) / 2.0f; // Field of View
    float ra = 1.0f; // Aspect Ratio
    float znear = 0.4f; // Near plane
    float zfar = 3.0f; // Far plane
   
    // Set the projection matrix
    glm::mat4 Proj = glm::perspective(FOV, ra, znear, zfar);

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
    DEBUG("ProjectTransform");
}


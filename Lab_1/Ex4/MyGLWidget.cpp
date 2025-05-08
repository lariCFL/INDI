#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <utility>

#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG(text) std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << ":" << text << std::endl;

MyGLWidget::~MyGLWidget() {}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL();
    DEBUG("InitializeGL");

    glEnable(GL_DEPTH_TEST); // Enable Z-buffer for hidden surface removal

    iniCamera(); // Initialize camera parameters and matrices
    loadModel(); // Load the model and create buffers
}

void MyGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    program->bind();

    glBindVertexArray(VAO); // Bind the VAO
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3); // 3 vertices per face
    glBindVertexArray(0);
}

void MyGLWidget::viewTransform() {
    glm::mat4 View = glm::lookAt(OBS, VRP, UP); // Use OBS, VRP, and UP
    glUniformMatrix4fv(glGetUniformLocation(program->programId(), "view"), 
                       1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform() {
    glm::mat4 proj = glm::perspective(FOV, ra, znear, zfar); // Use FOV, ra, znear, zfar
    glUniformMatrix4fv(glGetUniformLocation(program->programId(), "proj"), 
                       1, GL_FALSE, &proj[0][0]);
}

void MyGLWidget::iniCamera() {
    // Initialize camera parameters
    OBS = glm::vec3(0.0f, 0.0f, 1.0f); // Observer position
    VRP = glm::vec3(0.0f, 0.0f, 0.0f); // Look-at point
    UP = glm::vec3(1.0f, 0.0f, 0.0f);  // Up vector

    FOV = glm::radians(60.0f);         // Field of view in radians
    ra = width() / float(height());    // Aspect ratio
    znear = 0.1f;                      // Near plane
    zfar = 100.0f;                     // Far plane

    // Initialize matrices
    viewTransform();
    projectTransform();
}

void MyGLWidget::loadModel() {
    // createBuffers()
    // Load the model after creating buffers
    m.load("../models/HomerProves.obj"); // Load the model

    m.VBO_vertices(); //position
    m.VBO_matdiff(); //color

    m.faces().size(); //number of faces
}
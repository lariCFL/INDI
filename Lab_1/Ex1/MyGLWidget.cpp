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
    DEBUG("InitializeGL");
}

void MyGLWidget::paintGL ( ){

    BL2GLWidget::paintGL();
    DEBUG("PaintGL");

}



#pragma once
#include "..\include\freeglut.h"
#include <string>

extern int exhaustP;
void drawSquare(GLfloat length, GLfloat centreX, GLfloat centreY, float *color);
void drawBoxLine(int width, int height, float *color, int x, int y);
void drawRectangle(GLfloat length, GLfloat height, GLfloat centreX, GLfloat centreY, float* color);
void drawGrid(int length, int height,int startX, int startY, int gap, float* color, bool lines);
void drawCar(GLfloat centreX, GLfloat centreY);
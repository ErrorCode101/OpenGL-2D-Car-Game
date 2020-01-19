#pragma once
#include <vector> 
#include <string>
#include "..\include\freeglut.h"

int getRandomNumber(int max, int min);
std::vector<float> getRadomColorAvoid(int X, int Y, int Z);
void drawText(std::string text, void* font, GLfloat* color, int x, int y);
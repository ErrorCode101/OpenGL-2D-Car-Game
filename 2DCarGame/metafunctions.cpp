#include "metafunctions.h"
#include "..\include\freeglut.h"
#include <cstdlib>
#include <vector> 
#include <time.h> 
#include <string>

int getRandomNumber(int max, int min)
{
	//not truly random prduce biased results
	return (rand() % (max + 1 - min)) + min;
}

std::vector<float> getRadomColorAvoid(int X, int Y, int Z)
{
	bool valid = false;
	int x, y, z;
	std::vector<float> color;
	while (!valid)
	{
		x = getRandomNumber(1, 0);
		y = getRandomNumber(1, 0);
		z = getRandomNumber(1, 0);

		if (x != X || y != Y || z != Z)
		{
			valid = true;
			color.push_back(x);
			color.push_back(y);
			color.push_back(z);
		}
	}

	return color;
}

void drawText(std::string text, void* font, GLfloat* color, int x, int y)
{

	glColor3f(color[0], color[1], color[2]);
	glPushMatrix();
	glRasterPos2i(x, y);
	for (std::string::iterator i = text.begin(); i != text.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	glPopMatrix();
}